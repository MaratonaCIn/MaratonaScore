#include "MaratonaScore/gui/BlacklistDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QGroupBox>

namespace MaratonaScore {
namespace gui {

BlacklistDialog::BlacklistDialog(
    const std::map<std::string, core::models::CompetitorData>& competitors,
    QWidget* parent)
    : QDialog(parent), competitors_(competitors) {
    setupUI();
    refreshLists();
    setWindowTitle("Gerenciar Blacklist/Whitelist");
    resize(800, 600);
}

void BlacklistDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* listsLayout = new QHBoxLayout();

    // Active list
    QGroupBox* activeGroup = new QGroupBox("Ativos");
    QVBoxLayout* activeLayout = new QVBoxLayout(activeGroup);
    activeList_ = new QListWidget();
    activeLayout->addWidget(activeList_);
    QPushButton* blacklistBtn = new QPushButton("→ Blacklist");
    QPushButton* guestBtn = new QPushButton("→ Guest");
    connect(blacklistBtn, &QPushButton::clicked, this, &BlacklistDialog::onAddToBlacklist);
    connect(guestBtn, &QPushButton::clicked, this, &BlacklistDialog::onSetAsGuest);
    activeLayout->addWidget(blacklistBtn);
    activeLayout->addWidget(guestBtn);
    listsLayout->addWidget(activeGroup);

    // Blacklisted list
    QGroupBox* blacklistedGroup = new QGroupBox("Blacklist");
    QVBoxLayout* blacklistedLayout = new QVBoxLayout(blacklistedGroup);
    blacklistedList_ = new QListWidget();
    blacklistedLayout->addWidget(blacklistedList_);
    QPushButton* unblacklistBtn = new QPushButton("← Ativar");
    connect(unblacklistBtn, &QPushButton::clicked, this, &BlacklistDialog::onSetAsActive);
    blacklistedLayout->addWidget(unblacklistBtn);
    listsLayout->addWidget(blacklistedGroup);

    // Guest list
    QGroupBox* guestGroup = new QGroupBox("Guests");
    QVBoxLayout* guestLayout = new QVBoxLayout(guestGroup);
    guestList_ = new QListWidget();
    guestLayout->addWidget(guestList_);
    QPushButton* unguestBtn = new QPushButton("← Ativar");
    connect(unguestBtn, &QPushButton::clicked, this, &BlacklistDialog::onSetAsActive);
    guestLayout->addWidget(unguestBtn);
    listsLayout->addWidget(guestGroup);

    mainLayout->addLayout(listsLayout);

    // Reason
    QHBoxLayout* reasonLayout = new QHBoxLayout();
    reasonLayout->addWidget(new QLabel("Motivo (opcional):"));
    reasonEdit_ = new QLineEdit();
    reasonLayout->addWidget(reasonEdit_);
    mainLayout->addLayout(reasonLayout);

    // Buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    );
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addWidget(buttonBox);
}

void BlacklistDialog::refreshLists() {
    activeList_->clear();
    blacklistedList_->clear();
    guestList_->clear();

    for (const auto& [username, comp] : competitors_) {
        QListWidgetItem* item = new QListWidgetItem(
            QString::fromStdString(username)
        );
        item->setData(Qt::UserRole, QString::fromStdString(username));

        core::models::CompetitorStatus status = comp.status;

        // Check if there's a pending change
        if (changes_.find(username) != changes_.end()) {
            status = changes_[username];
        }

        switch (status) {
            case core::models::CompetitorStatus::ACTIVE:
                activeList_->addItem(item);
                break;
            case core::models::CompetitorStatus::BLACKLISTED:
                blacklistedList_->addItem(item);
                break;
            case core::models::CompetitorStatus::GUEST:
                guestList_->addItem(item);
                break;
        }
    }
}

void BlacklistDialog::onAddToBlacklist() {
    auto* item = activeList_->currentItem();
    if (item) {
        QString username = item->data(Qt::UserRole).toString();
        changes_[username.toStdString()] = core::models::CompetitorStatus::BLACKLISTED;
        refreshLists();
    }
}

void BlacklistDialog::onRemoveFromBlacklist() {
    auto* item = blacklistedList_->currentItem();
    if (item) {
        QString username = item->data(Qt::UserRole).toString();
        changes_[username.toStdString()] = core::models::CompetitorStatus::ACTIVE;
        refreshLists();
    }
}

void BlacklistDialog::onSetAsGuest() {
    auto* item = activeList_->currentItem();
    if (item) {
        QString username = item->data(Qt::UserRole).toString();
        changes_[username.toStdString()] = core::models::CompetitorStatus::GUEST;
        refreshLists();
    }
}

void BlacklistDialog::onSetAsActive() {
    QListWidget* sourceList = nullptr;
    if (blacklistedList_->currentItem()) {
        sourceList = blacklistedList_;
    } else if (guestList_->currentItem()) {
        sourceList = guestList_;
    }

    if (sourceList) {
        auto* item = sourceList->currentItem();
        QString username = item->data(Qt::UserRole).toString();
        changes_[username.toStdString()] = core::models::CompetitorStatus::ACTIVE;
        refreshLists();
    }
}

std::map<std::string, core::models::CompetitorStatus> BlacklistDialog::getChanges() const {
    return changes_;
}

}  // namespace gui
}  // namespace MaratonaScore
