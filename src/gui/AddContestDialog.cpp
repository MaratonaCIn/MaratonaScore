#include "MaratonaScore/gui/AddContestDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QFileDialog>
#include <QDialogButtonBox>

namespace MaratonaScore {
namespace gui {

AddContestDialog::AddContestDialog(QWidget* parent)
    : QDialog(parent) {
    setupUI();
    setWindowTitle("Adicionar Contest");
    resize(500, 250);
}

void AddContestDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();

    // Contest ID
    contestIdEdit_ = new QLineEdit();
    contestIdEdit_->setPlaceholderText("ex: contest-01");
    formLayout->addRow("Contest ID:", contestIdEdit_);

    // XLSX Path
    QHBoxLayout* xlsxLayout = new QHBoxLayout();
    xlsxPathEdit_ = new QLineEdit();
    xlsxPathEdit_->setPlaceholderText("Selecione o arquivo XLSX...");
    xlsxLayout->addWidget(xlsxPathEdit_);

    browseButton_ = new QPushButton("Procurar...");
    connect(browseButton_, &QPushButton::clicked, this, &AddContestDialog::onBrowse);
    xlsxLayout->addWidget(browseButton_);

    formLayout->addRow("Arquivo XLSX:", xlsxLayout);

    // Type
    typeCombo_ = new QComboBox();
    typeCombo_->addItem("Contest (OnTime)", "ontime");
    typeCombo_->addItem("Homework", "homework");
    formLayout->addRow("Tipo:", typeCombo_);

    // Duration
    durationCombo_ = new QComboBox();
    durationCombo_->addItem("5 horas (300 min)", 300);
    durationCombo_->addItem("1 semana (10080 min)", 10080);
    durationCombo_->addItem("Custom...", 0);
    formLayout->addRow("Duração:", durationCombo_);

    mainLayout->addLayout(formLayout);

    // Buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    );
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddContestDialog::onOk);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addWidget(buttonBox);
}

QString AddContestDialog::getContestId() const {
    return contestIdEdit_->text();
}

QString AddContestDialog::getXlsxPath() const {
    return xlsxPathEdit_->text();
}

QString AddContestDialog::getContestType() const {
    return typeCombo_->currentData().toString();
}

int AddContestDialog::getDuration() const {
    return durationCombo_->currentData().toInt();
}

void AddContestDialog::onBrowse() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Selecionar XLSX",
        QString(),
        "Excel/JSON Files (*.xlsx *.xls *.json);;All Files (*)"
    );

    if (!fileName.isEmpty()) {
        xlsxPathEdit_->setText(fileName);
    }
}

void AddContestDialog::onOk() {
    // Basic validation
    if (contestIdEdit_->text().isEmpty()) {
        return;  // Could show error message
    }
    if (xlsxPathEdit_->text().isEmpty()) {
        return;
    }

    accept();
}

}  // namespace gui
}  // namespace MaratonaScore
