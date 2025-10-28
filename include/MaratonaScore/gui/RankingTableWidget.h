#ifndef MARATONASCORE_GUI_RANKINGTABLEWIDGET_H
#define MARATONASCORE_GUI_RANKINGTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <map>
#include <string>

#include "MaratonaScore/core/models/CompetitorData.h"

namespace MaratonaScore {
namespace gui {

/**
 * @brief Custom QTableWidgetItem that sorts numerically
 */
class NumericTableWidgetItem : public QTableWidgetItem {
public:
    explicit NumericTableWidgetItem(const QString& text, double value)
        : QTableWidgetItem(text), numericValue_(value) {}

    bool operator<(const QTableWidgetItem& other) const override {
        const NumericTableWidgetItem* otherNumeric =
            dynamic_cast<const NumericTableWidgetItem*>(&other);
        if (otherNumeric) {
            return numericValue_ < otherNumeric->numericValue_;
        }
        return QTableWidgetItem::operator<(other);
    }

private:
    double numericValue_;
};

/**
 * @brief Widget displaying the general ranking table
 *
 * Shows:
 * - Rank
 * - Name
 * - Total points
 * - Contest points
 * - Homework points
 * - Upsolving points
 */
class RankingTableWidget : public QWidget {
    Q_OBJECT

public:
    explicit RankingTableWidget(QWidget* parent = nullptr);

    void setRankingData(const std::map<std::string, core::models::CompetitorData>& competitors);
    void refresh();

private slots:
    void onFilterChanged();
    void onSearchChanged(const QString& text);

private:
    void setupUI();
    void populateTable();
    void applyFilters();

    // UI components
    QTableWidget* table_;
    QComboBox* filterCombo_;
    QLineEdit* searchBox_;
    QLabel* statsLabel_;

    // Data
    std::map<std::string, core::models::CompetitorData> competitors_;
    std::vector<std::pair<std::string, core::models::CompetitorData>> filteredData_;
};

}  // namespace gui
}  // namespace MaratonaScore

#endif  // MARATONASCORE_GUI_RANKINGTABLEWIDGET_H
