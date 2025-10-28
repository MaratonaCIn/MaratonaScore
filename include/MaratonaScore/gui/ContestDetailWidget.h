#ifndef MARATONASCORE_GUI_CONTESTDETAILWIDGET_H
#define MARATONASCORE_GUI_CONTESTDETAILWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <map>
#include <string>

#include "MaratonaScore/core/models/CompetitorData.h"
#include "MaratonaScore/core/models/ProcessedContest.h"

namespace MaratonaScore {
namespace gui {

/**
 * @brief Widget displaying detailed information about a contest
 */
class ContestDetailWidget : public QWidget {
    Q_OBJECT

public:
    explicit ContestDetailWidget(QWidget* parent = nullptr);

    void setContestData(
        const std::string& contestId,
        const core::models::ProcessedContest& contest,
        const std::map<std::string, core::models::CompetitorData>& competitors
    );
    void clear();

private:
    void setupUI();
    void populateTable();

    QLabel* titleLabel_;
    QLabel* infoLabel_;
    QTableWidget* table_;

    std::string currentContestId_;
    core::models::ProcessedContest currentContest_;
    std::map<std::string, core::models::CompetitorData> competitors_;
};

}  // namespace gui
}  // namespace MaratonaScore

#endif  // MARATONASCORE_GUI_CONTESTDETAILWIDGET_H
