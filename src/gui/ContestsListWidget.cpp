#include "MaratonaScore/gui/ContestsListWidget.h"

namespace MaratonaScore {
namespace gui {

ContestsListWidget::ContestsListWidget(QWidget* parent)
    : QWidget(parent) {
    setupUI();
}

void ContestsListWidget::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    statsLabel_ = new QLabel("Total: 0 contests");
    layout->addWidget(statsLabel_);

    listWidget_ = new QListWidget();
    layout->addWidget(listWidget_);

    connect(listWidget_, &QListWidget::itemClicked,
            this, &ContestsListWidget::onItemClicked);
}

void ContestsListWidget::setContestsData(
    const std::map<std::string, core::models::ProcessedContest>& contests) {
    contests_ = contests;
    populateList();
}

void ContestsListWidget::refresh() {
    populateList();
}

void ContestsListWidget::populateList() {
    listWidget_->clear();

    for (const auto& [contestId, contest] : contests_) {
        QString itemText = QString("%1 (%2) - %3 participantes")
            .arg(QString::fromStdString(contestId))
            .arg(QString::fromStdString(contest.type))
            .arg(contest.total_participants);

        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, QString::fromStdString(contestId));
        listWidget_->addItem(item);
    }

    statsLabel_->setText(QString("Total: %1 contests").arg(contests_.size()));
}

void ContestsListWidget::onItemClicked(QListWidgetItem* item) {
    QString contestId = item->data(Qt::UserRole).toString();
    emit contestSelected(contestId);
}

}  // namespace gui
}  // namespace MaratonaScore
