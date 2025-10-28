#ifndef MARATONASCORE_GUI_CONTESTSLISTWIDGET_H
#define MARATONASCORE_GUI_CONTESTSLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <map>
#include <string>

#include "MaratonaScore/core/models/ProcessedContest.h"

namespace MaratonaScore {
namespace gui {

/**
 * @brief Widget displaying the list of contests
 */
class ContestsListWidget : public QWidget {
    Q_OBJECT

public:
    explicit ContestsListWidget(QWidget* parent = nullptr);

    void setContestsData(const std::map<std::string, core::models::ProcessedContest>& contests);
    void refresh();

signals:
    void contestSelected(const QString& contestId);

private slots:
    void onItemClicked(QListWidgetItem* item);

private:
    void setupUI();
    void populateList();

    QListWidget* listWidget_;
    QLabel* statsLabel_;
    std::map<std::string, core::models::ProcessedContest> contests_;
};

}  // namespace gui
}  // namespace MaratonaScore

#endif  // MARATONASCORE_GUI_CONTESTSLISTWIDGET_H
