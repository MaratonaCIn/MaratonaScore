#include "MaratonaScore/gui/ConfigDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>

namespace MaratonaScore {
namespace gui {

ConfigDialog::ConfigDialog(const core::models::ScoringConfig& config,
                           QWidget* parent)
    : QDialog(parent), config_(config) {
    setupUI();
    populateFields();
    setWindowTitle("Configuração de Pontuação");
    resize(600, 500);
}

void ConfigDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QTabWidget* tabWidget = new QTabWidget();

    // ===== CONTEST TAB =====
    QWidget* contestTab = new QWidget();
    QVBoxLayout* contestLayout = new QVBoxLayout(contestTab);

    QGroupBox* contestGroup = new QGroupBox("Contest (OnTime)");
    QFormLayout* contestForm = new QFormLayout(contestGroup);

    contestWeightSpin_ = new QDoubleSpinBox();
    contestWeightSpin_->setRange(0, 1000);
    contestWeightSpin_->setDecimals(1);
    contestWeightSpin_->setSuffix(" pontos");
    contestForm->addRow("Peso Base (problemas):", contestWeightSpin_);

    contestsToDoubleSpin_ = new QSpinBox();
    contestsToDoubleSpin_->setRange(1, 100);
    contestsToDoubleSpin_->setSuffix(" contests");
    contestForm->addRow("Contests para dobrar:", contestsToDoubleSpin_);

    QLabel* contestExplanation = new QLabel(
        "<i>Multiplicador = 2^(índice / contests_para_dobrar)</i>");
    contestExplanation->setWordWrap(true);
    contestForm->addRow("", contestExplanation);

    contestTopNSpin_ = new QSpinBox();
    contestTopNSpin_->setRange(0, 100);
    contestTopNSpin_->setSuffix(" pessoas");
    contestForm->addRow("Top N recebem bônus:", contestTopNSpin_);

    contestMaxBonusSpin_ = new QDoubleSpinBox();
    contestMaxBonusSpin_->setRange(0, 500);
    contestMaxBonusSpin_->setDecimals(1);
    contestMaxBonusSpin_->setSuffix(" pontos");
    contestForm->addRow("Bônus máximo (1º lugar):", contestMaxBonusSpin_);

    QLabel* contestBonusExplanation = new QLabel(
        "<i>Decremento linear: 1º → max, Nº → 0</i>");
    contestBonusExplanation->setWordWrap(true);
    contestForm->addRow("", contestBonusExplanation);

    contestLayout->addWidget(contestGroup);
    contestLayout->addStretch();

    tabWidget->addTab(contestTab, "Contest");

    // ===== HOMEWORK TAB =====
    QWidget* homeworkTab = new QWidget();
    QVBoxLayout* homeworkLayout = new QVBoxLayout(homeworkTab);

    QGroupBox* homeworkGroup = new QGroupBox("Homework (Semanal)");
    QFormLayout* homeworkForm = new QFormLayout(homeworkGroup);

    homeworkWeightSpin_ = new QDoubleSpinBox();
    homeworkWeightSpin_->setRange(0, 1000);
    homeworkWeightSpin_->setDecimals(1);
    homeworkWeightSpin_->setSuffix(" pontos");
    homeworkForm->addRow("Peso Base (problemas):", homeworkWeightSpin_);

    homeworkToDoubleSpin_ = new QSpinBox();
    homeworkToDoubleSpin_->setRange(1, 100);
    homeworkToDoubleSpin_->setSuffix(" homeworks");
    homeworkForm->addRow("Homeworks para dobrar:", homeworkToDoubleSpin_);

    QLabel* homeworkExplanation = new QLabel(
        "<i>Multiplicador = 2^(índice / homeworks_para_dobrar)</i>");
    homeworkExplanation->setWordWrap(true);
    homeworkForm->addRow("", homeworkExplanation);

    homeworkTopNSpin_ = new QSpinBox();
    homeworkTopNSpin_->setRange(0, 100);
    homeworkTopNSpin_->setSuffix(" pessoas");
    homeworkForm->addRow("Top N recebem bônus:", homeworkTopNSpin_);

    homeworkMaxBonusSpin_ = new QDoubleSpinBox();
    homeworkMaxBonusSpin_->setRange(0, 500);
    homeworkMaxBonusSpin_->setDecimals(1);
    homeworkMaxBonusSpin_->setSuffix(" pontos");
    homeworkForm->addRow("Bônus máximo (1º lugar):", homeworkMaxBonusSpin_);

    QLabel* homeworkBonusExplanation = new QLabel(
        "<i>Decremento linear: 1º → max, Nº → 0</i>");
    homeworkBonusExplanation->setWordWrap(true);
    homeworkForm->addRow("", homeworkBonusExplanation);

    homeworkLayout->addWidget(homeworkGroup);
    homeworkLayout->addStretch();

    tabWidget->addTab(homeworkTab, "Homework");

    // ===== UPSOLVING TAB =====
    QWidget* upsolvingTab = new QWidget();
    QVBoxLayout* upsolvingLayout = new QVBoxLayout(upsolvingTab);

    QGroupBox* upsolvingGroup = new QGroupBox("Upsolving");
    QFormLayout* upsolvingForm = new QFormLayout(upsolvingGroup);

    upsolvingPointsSpin_ = new QDoubleSpinBox();
    upsolvingPointsSpin_->setRange(0, 100);
    upsolvingPointsSpin_->setDecimals(1);
    upsolvingPointsSpin_->setSuffix(" pontos");
    upsolvingForm->addRow("Pontos por problema:", upsolvingPointsSpin_);

    QLabel* upsolvingExplanation = new QLabel(
        "<i>Pontos fixos por problema resolvido após o contest</i>");
    upsolvingExplanation->setWordWrap(true);
    upsolvingForm->addRow("", upsolvingExplanation);

    upsolvingLayout->addWidget(upsolvingGroup);
    upsolvingLayout->addStretch();

    tabWidget->addTab(upsolvingTab, "Upsolving");

    // ===== INFO TAB =====
    QWidget* infoTab = new QWidget();
    QVBoxLayout* infoLayout = new QVBoxLayout(infoTab);

    QLabel* infoLabel = new QLabel(
        "<h3>Sistema de Pontuação Híbrido</h3>"
        "<p><b>Fórmula:</b></p>"
        "<p>Pontos = (problemas/max × peso_base × 2^(índice/n)) + bônus_rank</p>"
        "<br>"
        "<p><b>Componentes:</b></p>"
        "<ul>"
        "<li><b>Peso Base:</b> Pontos máximos por resolver todos os problemas</li>"
        "<li><b>Juros Compostos:</b> Multiplicador exponencial que cresce ao longo dos contests</li>"
        "<li><b>Bônus de Rank:</b> Pontos extras para top N competidores (decrescente linear)</li>"
        "</ul>"
        "<br>"
        "<p><b>Exemplo:</b></p>"
        "<p>Contest 1 (índice 0): peso=100, dobrar=11, bônus_max=20, top=10</p>"
        "<ul>"
        "<li>1º lugar, 10/10 problemas: (10/10 × 100 × 2^(0/11)) + 20 = <b>120 pts</b></li>"
        "<li>2º lugar, 10/10 problemas: (10/10 × 100 × 2^(0/11)) + 18 = <b>118 pts</b></li>"
        "<li>10º lugar, 8/10 problemas: (8/10 × 100 × 2^(0/11)) + 2 = <b>82 pts</b></li>"
        "</ul>"
        "<p>Contest 11 (índice 10): multiplicador = 2^(10/11) ≈ 1.95×</p>"
        "<ul>"
        "<li>1º lugar, 10/10 problemas: (10/10 × 100 × 1.95) + 20 = <b>215 pts</b></li>"
        "</ul>"
    );
    infoLabel->setWordWrap(true);
    infoLayout->addWidget(infoLabel);
    infoLayout->addStretch();

    tabWidget->addTab(infoTab, "Info");

    mainLayout->addWidget(tabWidget);

    // Buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    );
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addWidget(buttonBox);
}

void ConfigDialog::populateFields() {
    // Contest
    contestWeightSpin_->setValue(config_.contest_problems_weight);
    contestsToDoubleSpin_->setValue(config_.contests_to_double);
    contestTopNSpin_->setValue(config_.rank_bonus_top_n);
    contestMaxBonusSpin_->setValue(config_.rank_bonus_max);

    // Homework
    homeworkWeightSpin_->setValue(config_.homework_problems_weight);
    homeworkToDoubleSpin_->setValue(config_.homework_contests_to_double);
    homeworkTopNSpin_->setValue(config_.homework_rank_bonus_top_n);
    homeworkMaxBonusSpin_->setValue(config_.homework_rank_bonus_max);

    // Upsolving
    upsolvingPointsSpin_->setValue(config_.upsolving_points_per_problem);
}

core::models::ScoringConfig ConfigDialog::getConfig() const {
    core::models::ScoringConfig newConfig;

    // Contest
    newConfig.contest_problems_weight = contestWeightSpin_->value();
    newConfig.contests_to_double = contestsToDoubleSpin_->value();
    newConfig.rank_bonus_top_n = contestTopNSpin_->value();
    newConfig.rank_bonus_max = contestMaxBonusSpin_->value();

    // Homework
    newConfig.homework_problems_weight = homeworkWeightSpin_->value();
    newConfig.homework_contests_to_double = homeworkToDoubleSpin_->value();
    newConfig.homework_rank_bonus_top_n = homeworkTopNSpin_->value();
    newConfig.homework_rank_bonus_max = homeworkMaxBonusSpin_->value();

    // Upsolving
    newConfig.upsolving_points_per_problem = upsolvingPointsSpin_->value();

    return newConfig;
}

}  // namespace gui
}  // namespace MaratonaScore
