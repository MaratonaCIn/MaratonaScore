#ifndef MARATONASCORE_GUI_BLACKLISTDIALOG_H
#define MARATONASCORE_GUI_BLACKLISTDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <map>
#include <string>

#include "MaratonaScore/core/models/CompetitorData.h"

namespace MaratonaScore {
namespace gui {

/**
 * @brief Dialog for managing competitor blacklist/whitelist
 */
class BlacklistDialog : public QDialog {
    Q_OBJECT

public:
    explicit BlacklistDialog(
        const std::map<std::string, core::models::CompetitorData>& competitors,
        QWidget* parent = nullptr
    );

    std::map<std::string, core::models::CompetitorStatus> getChanges() const;

private slots:
    void onAddToBlacklist();
    void onRemoveFromBlacklist();
    void onSetAsGuest();
    void onSetAsActive();

private:
    void setupUI();
    void refreshLists();

    QListWidget* activeList_;
    QListWidget* blacklistedList_;
    QListWidget* guestList_;
    QLineEdit* reasonEdit_;

    std::map<std::string, core::models::CompetitorData> competitors_;
    std::map<std::string, core::models::CompetitorStatus> changes_;
};

}  // namespace gui
}  // namespace MaratonaScore

#endif  // MARATONASCORE_GUI_BLACKLISTDIALOG_H
