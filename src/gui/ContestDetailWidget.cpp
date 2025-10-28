#include "MaratonaScore/gui/ContestDetailWidget.h"

#include <QHeaderView>
#include <algorithm>
#include <vector>

namespace MaratonaScore {
namespace gui {

ContestDetailWidget::ContestDetailWidget(QWidget* parent)
    : QWidget(parent) {
    setupUI();
}

void ContestDetailWidget::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    titleLabel_ = new QLabel("<h2>Selecione um contest</h2>");
    layout->addWidget(titleLabel_);

    infoLabel_ = new QLabel();
    layout->addWidget(infoLabel_);

    table_ = new QTableWidget();
    table_->setColumnCount(5);
    table_->setHorizontalHeaderLabels({
        "Rank", "Nome", "Problemas", "Penalidade", "Pontos"
    });
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->verticalHeader()->setVisible(false);
    table_->setAlternatingRowColors(true);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(table_);
}

void ContestDetailWidget::setContestData(
    const std::string& contestId,
    const core::models::ProcessedContest& contest,
    const std::map<std::string, core::models::CompetitorData>& competitors) {

    currentContestId_ = contestId;
    currentContest_ = contest;
    competitors_ = competitors;

    titleLabel_->setText(QString("<h2>%1</h2>")
                            .arg(QString::fromStdString(contestId)));

    infoLabel_->setText(QString(
        "<b>Tipo:</b> %1 | <b>Participantes:</b> %2 | "
        "<b>Máximo de problemas:</b> %3 | <b>Data:</b> %4")
                           .arg(QString::fromStdString(contest.type))
                           .arg(contest.total_participants)
                           .arg(contest.max_problems_solved)
                           .arg(QString::fromStdString(contest.first_processed_date)));

    populateTable();
}

void ContestDetailWidget::clear() {
    titleLabel_->setText("<h2>Selecione um contest</h2>");
    infoLabel_->clear();
    table_->setRowCount(0);
}

void ContestDetailWidget::populateTable() {
    // Collect performances for this contest
    struct Performance {
        std::string username;
        int rank;
        int problems_solved;
        int total_participants;
        double points;
    };

    std::vector<Performance> performances;

    for (const auto& [username, comp] : competitors_) {
        // Check if competitor participated in this contest
        const auto* perf = [&]() -> const core::models::ContestPerformance* {
            if (currentContest_.type == "ontime") {
                auto it = comp.contests.find(currentContestId_);
                return it != comp.contests.end() ? &it->second : nullptr;
            } else {
                auto it = comp.homeworks.find(currentContestId_);
                return it != comp.homeworks.end() ? &it->second : nullptr;
            }
        }();

        if (perf) {
            performances.push_back({
                username,
                perf->rank,
                perf->problems_solved,
                perf->total_participants,
                perf->points_earned
            });
        }
    }

    // Sort by rank
    std::sort(performances.begin(), performances.end(),
              [](const auto& a, const auto& b) {
                  return a.rank < b.rank;
              });

    // Populate table
    table_->setRowCount(static_cast<int>(performances.size()));

    for (size_t i = 0; i < performances.size(); ++i) {
        const auto& perf = performances[i];

        table_->setItem(i, 0, new QTableWidgetItem(
            QString::number(perf.rank)));

        table_->setItem(i, 1, new QTableWidgetItem(
            QString::fromStdString(perf.username)));

        table_->setItem(i, 2, new QTableWidgetItem(
            QString::number(perf.problems_solved)));

        table_->setItem(i, 3, new QTableWidgetItem("-"));  // Penalty not stored

        table_->setItem(i, 4, new QTableWidgetItem(
            QString::number(perf.points, 'f', 1)));
    }
}

}  // namespace gui
}  // namespace MaratonaScore
