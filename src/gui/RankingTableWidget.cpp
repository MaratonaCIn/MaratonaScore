#include "MaratonaScore/gui/RankingTableWidget.h"

#include <QHeaderView>
#include <QPushButton>
#include <algorithm>

namespace MaratonaScore {
namespace gui {

RankingTableWidget::RankingTableWidget(QWidget* parent)
    : QWidget(parent) {
    setupUI();
}

void RankingTableWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Filter controls
    QHBoxLayout* filterLayout = new QHBoxLayout();

    filterLayout->addWidget(new QLabel("Filtrar:"));
    filterCombo_ = new QComboBox();
    filterCombo_->addItem("Todos", "all");
    filterCombo_->addItem("Somente Contest", "contest");
    filterCombo_->addItem("Somente Homework", "homework");
    filterCombo_->addItem("Somente Upsolving", "upsolving");
    filterLayout->addWidget(filterCombo_);

    filterLayout->addWidget(new QLabel("Buscar:"));
    searchBox_ = new QLineEdit();
    searchBox_->setPlaceholderText("Nome do competidor...");
    filterLayout->addWidget(searchBox_);

    filterLayout->addStretch();

    mainLayout->addLayout(filterLayout);

    // Table
    table_ = new QTableWidget();
    table_->setColumnCount(9);
    table_->setHorizontalHeaderLabels({
        "Rank", "Nome", "Problemas", "Total", "Contest", "Homework", "Upsolving",
        "Contest %", "Homework %"
    });

    table_->horizontalHeader()->setStretchLastSection(false);
    table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table_->verticalHeader()->setVisible(false);
    table_->setAlternatingRowColors(true);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setSortingEnabled(true);

    mainLayout->addWidget(table_);

    // Stats label
    statsLabel_ = new QLabel("Total: 0 competidores");
    mainLayout->addWidget(statsLabel_);

    // Connections
    connect(filterCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RankingTableWidget::onFilterChanged);
    connect(searchBox_, &QLineEdit::textChanged,
            this, &RankingTableWidget::onSearchChanged);
}

void RankingTableWidget::setRankingData(
    const std::map<std::string, core::models::CompetitorData>& competitors) {
    competitors_ = competitors;
    applyFilters();
    populateTable();
}

void RankingTableWidget::refresh() {
    populateTable();
}

void RankingTableWidget::applyFilters() {
    filteredData_.clear();

    QString filterType = filterCombo_->currentData().toString();
    QString searchText = searchBox_->text().toLower();

    for (const auto& [username, comp] : competitors_) {
        // Skip blacklisted/guests based on status
        if (comp.status == core::models::CompetitorStatus::BLACKLISTED) {
            continue;
        }

        // Apply search filter
        if (!searchText.isEmpty()) {
            QString name = QString::fromStdString(comp.user_name).toLower();
            QString team = QString::fromStdString(comp.team_name).toLower();
            if (!name.contains(searchText) && !team.contains(searchText)) {
                continue;
            }
        }

        // Apply type filter (show only if they have points in that category)
        if (filterType == "contest" && comp.total_contest_points <= 0.0) continue;
        if (filterType == "homework" && comp.total_homework_points <= 0.0) continue;
        if (filterType == "upsolving" && comp.total_upsolving_points <= 0.0) continue;

        filteredData_.push_back({username, comp});
    }

    // Sort by final score descending
    std::sort(filteredData_.begin(), filteredData_.end(),
              [](const auto& a, const auto& b) {
                  return a.second.final_score > b.second.final_score;
              });
}

void RankingTableWidget::populateTable() {
    table_->setSortingEnabled(false);
    table_->setRowCount(static_cast<int>(filteredData_.size()));

    for (size_t i = 0; i < filteredData_.size(); ++i) {
        const auto& [username, comp] = filteredData_[i];

        // Rank
        table_->setItem(i, 0, new NumericTableWidgetItem(
            QString::number(i + 1), static_cast<double>(i + 1)));

        // Name (text only, no numeric sort)
        QString displayName = QString::fromStdString(comp.user_name);
        if (!comp.team_name.empty() && comp.team_name != comp.user_name) {
            displayName += QString(" (%1)").arg(QString::fromStdString(comp.team_name));
        }
        table_->setItem(i, 1, new QTableWidgetItem(displayName));

        // Total problems solved (contests + homeworks + upsolving)
        int totalProblems = 0;
        for (const auto& [cid, perf] : comp.contests) {
            totalProblems += perf.problems_solved;
        }
        for (const auto& [hid, perf] : comp.homeworks) {
            totalProblems += perf.problems_solved;
        }
        totalProblems += comp.total_upsolving;  // Add upsolving problems
        table_->setItem(i, 2, new NumericTableWidgetItem(
            QString::number(totalProblems), static_cast<double>(totalProblems)));

        // Total points
        table_->setItem(i, 3, new NumericTableWidgetItem(
            QString::number(comp.final_score, 'f', 1), comp.final_score));

        // Contest points
        table_->setItem(i, 4, new NumericTableWidgetItem(
            QString::number(comp.total_contest_points, 'f', 1), comp.total_contest_points));

        // Homework points
        table_->setItem(i, 5, new NumericTableWidgetItem(
            QString::number(comp.total_homework_points, 'f', 1), comp.total_homework_points));

        // Upsolving points
        table_->setItem(i, 6, new NumericTableWidgetItem(
            QString::number(comp.total_upsolving_points, 'f', 1), comp.total_upsolving_points));

        // Contest percentage
        double contestPct = comp.final_score > 0
            ? (comp.total_contest_points / comp.final_score) * 100.0
            : 0.0;
        table_->setItem(i, 7, new NumericTableWidgetItem(
            QString::number(contestPct, 'f', 1) + "%", contestPct));

        // Homework percentage
        double homeworkPct = comp.final_score > 0
            ? (comp.total_homework_points / comp.final_score) * 100.0
            : 0.0;
        table_->setItem(i, 8, new NumericTableWidgetItem(
            QString::number(homeworkPct, 'f', 1) + "%", homeworkPct));
    }

    table_->setSortingEnabled(true);

    // Sort by rank (column 0) ascending by default
    table_->sortItems(0, Qt::AscendingOrder);

    statsLabel_->setText(QString("Total: %1 competidores")
                            .arg(filteredData_.size()));
}

void RankingTableWidget::onFilterChanged() {
    applyFilters();
    populateTable();
}

void RankingTableWidget::onSearchChanged(const QString& /*text*/) {
    applyFilters();
    populateTable();
}

}  // namespace gui
}  // namespace MaratonaScore
