#ifndef MARATONASCORE_GUI_MAINWINDOW_H
#define MARATONASCORE_GUI_MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <memory>

#include "MaratonaScore/core/services/RatingSystem.h"

namespace MaratonaScore {
namespace gui {

// Forward declarations
class RankingTableWidget;
class ContestsListWidget;
class ContestDetailWidget;

/**
 * @brief Main window of the application
 *
 * Contains tabs for:
 * - Ranking general
 * - List of contests
 * - Contest details
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    // Menu actions
    void onAddContest();
    void onAddMultipleContests();
    void onAddHomework();
    void onUpdateContest();
    void onDeleteContest();
    void onImportXLSX();
    void onManageBlacklist();
    void onEditConfig();
    void onRefresh();
    void onExportRanking();
    void onAbout();

    // Contest selection
    void onContestSelected(const QString& contestId);

private:
    void setupUI();
    void setupMenuBar();
    void createConnections();
    void loadDatabase();
    void saveDatabase();
    void updateAll();

    // Core system
    std::unique_ptr<core::services::RatingSystem> ratingSystem_;
    QString databasePath_;

    // UI components
    QTabWidget* tabWidget_;
    RankingTableWidget* rankingTable_;
    ContestsListWidget* contestsList_;
    ContestDetailWidget* contestDetail_;
    QStatusBar* statusBar_;
};

}  // namespace gui
}  // namespace MaratonaScore

#endif  // MARATONASCORE_GUI_MAINWINDOW_H
