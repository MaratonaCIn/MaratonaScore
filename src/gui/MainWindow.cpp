#include "MaratonaScore/gui/MainWindow.h"
#include "MaratonaScore/gui/RankingTableWidget.h"
#include "MaratonaScore/gui/ContestsListWidget.h"
#include "MaratonaScore/gui/ContestDetailWidget.h"
#include "MaratonaScore/gui/AddContestDialog.h"
#include "MaratonaScore/gui/BlacklistDialog.h"
#include "MaratonaScore/gui/ConfigDialog.h"
#include "MaratonaScore/core/persistence/DatabaseManager.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QFileInfo>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>

namespace MaratonaScore {
namespace gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ratingSystem_(std::make_unique<core::services::RatingSystem>()),
      databasePath_("ratings_v2.json") {
    setupUI();
    setupMenuBar();
    createConnections();
    loadDatabase();
    updateAll();

    setWindowTitle("MaratonaScore - Rating System");
    resize(1200, 800);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    tabWidget_ = new QTabWidget(this);

    // Create widgets for each tab
    rankingTable_ = new RankingTableWidget(this);
    contestsList_ = new ContestsListWidget(this);
    contestDetail_ = new ContestDetailWidget(this);

    // Add tabs
    tabWidget_->addTab(rankingTable_, "Ranking Geral");
    tabWidget_->addTab(contestsList_, "Contests");
    tabWidget_->addTab(contestDetail_, "Detalhes do Contest");

    setCentralWidget(tabWidget_);

    // Status bar
    statusBar_ = statusBar();
    statusBar_->showMessage("Pronto");
}

void MainWindow::setupMenuBar() {
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&Arquivo");

    QAction* addContestAction = fileMenu->addAction("Adicionar &Contest");
    connect(addContestAction, &QAction::triggered, this, &MainWindow::onAddContest);

    QAction* addMultipleAction = fileMenu->addAction("Adicionar &Múltiplos Contests");
    connect(addMultipleAction, &QAction::triggered, this, &MainWindow::onAddMultipleContests);

    QAction* addHomeworkAction = fileMenu->addAction("Adicionar &Homework");
    connect(addHomeworkAction, &QAction::triggered, this, &MainWindow::onAddHomework);

    fileMenu->addSeparator();

    QAction* updateContestAction = fileMenu->addAction("At&ualizar Contest");
    connect(updateContestAction, &QAction::triggered, this, &MainWindow::onUpdateContest);

    QAction* deleteContestAction = fileMenu->addAction("&Deletar Contest");
    connect(deleteContestAction, &QAction::triggered, this, &MainWindow::onDeleteContest);

    fileMenu->addSeparator();

    QAction* importAction = fileMenu->addAction("&Importar XLSX");
    connect(importAction, &QAction::triggered, this, &MainWindow::onImportXLSX);

    QAction* exportAction = fileMenu->addAction("&Exportar Ranking");
    connect(exportAction, &QAction::triggered, this, &MainWindow::onExportRanking);

    fileMenu->addSeparator();

    QAction* exitAction = fileMenu->addAction("&Sair");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Edit menu
    QMenu* editMenu = menuBar()->addMenu("&Editar");

    QAction* configAction = editMenu->addAction("&Configurações de Pontuação");
    connect(configAction, &QAction::triggered, this, &MainWindow::onEditConfig);

    editMenu->addSeparator();

    QAction* blacklistAction = editMenu->addAction("Gerenciar &Blacklist");
    connect(blacklistAction, &QAction::triggered, this, &MainWindow::onManageBlacklist);

    editMenu->addSeparator();

    QAction* refreshAction = editMenu->addAction("&Atualizar");
    refreshAction->setShortcut(QKeySequence::Refresh);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::onRefresh);

    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("&Ajuda");

    QAction* aboutAction = helpMenu->addAction("&Sobre");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::createConnections() {
    connect(contestsList_, &ContestsListWidget::contestSelected,
            this, &MainWindow::onContestSelected);
}

void MainWindow::loadDatabase() {
    auto config = ratingSystem_->getConfig();
    std::map<std::string, core::models::CompetitorData> competitors;
    std::map<std::string, core::models::ProcessedContest> processedContests;

    bool success = core::persistence::DatabaseManager::loadDatabase(
        databasePath_.toStdString(), config, competitors, processedContests);

    if (success || !competitors.empty()) {
        ratingSystem_->setConfig(config);
        ratingSystem_->setCompetitors(competitors);
        ratingSystem_->setProcessedContests(processedContests);
        ratingSystem_->recalculateAllScores();
        statusBar_->showMessage(
            QString("Carregado: %1 competidores, %2 contests")
                .arg(competitors.size())
                .arg(processedContests.size()),
            5000
        );
    } else {
        statusBar_->showMessage("Banco de dados vazio ou não encontrado", 5000);
    }
}

void MainWindow::saveDatabase() {
    bool success = core::persistence::DatabaseManager::saveDatabase(
        databasePath_.toStdString(),
        ratingSystem_->getConfig(),
        ratingSystem_->getCompetitors(),
        ratingSystem_->getProcessedContests()
    );

    if (success) {
        statusBar_->showMessage("Banco de dados salvo com sucesso", 3000);
    } else {
        QMessageBox::warning(this, "Erro", "Falha ao salvar banco de dados");
    }
}

void MainWindow::updateAll() {
    rankingTable_->setRankingData(ratingSystem_->getCompetitors());
    contestsList_->setContestsData(ratingSystem_->getProcessedContests());
}

void MainWindow::onAddContest() {
    AddContestDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString contestId = dialog.getContestId();
        QString xlsxPath = dialog.getXlsxPath();
        QString contestType = dialog.getContestType();
        int duration = dialog.getDuration();

        bool success = ratingSystem_->processScoreboard(
            contestId.toStdString(),
            xlsxPath.toStdString(),
            contestType.toStdString(),
            duration
        );

        if (success) {
            saveDatabase();
            updateAll();
            QMessageBox::information(this, "Sucesso", "Contest adicionado com sucesso!");
        } else {
            QMessageBox::warning(this, "Erro", "Falha ao processar contest");
        }
    }
}

void MainWindow::onAddHomework() {
    AddContestDialog dialog(this);
    // Pre-select homework type
    if (dialog.exec() == QDialog::Accepted) {
        onAddContest();  // Same logic
    }
}

void MainWindow::onImportXLSX() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Selecionar arquivo XLSX",
        QString(),
        "Excel Files (*.xlsx *.xls);;JSON Files (*.json);;All Files (*)"
    );

    if (!fileName.isEmpty()) {
        // TODO: Implement XLSX import logic
        QMessageBox::information(this, "Info", "Arquivo: " + fileName);
    }
}

void MainWindow::onManageBlacklist() {
    BlacklistDialog dialog(ratingSystem_->getCompetitors(), this);
    if (dialog.exec() == QDialog::Accepted) {
        auto changes = dialog.getChanges();
        // Apply changes to competitors
        auto& competitors = const_cast<std::map<std::string, core::models::CompetitorData>&>(
            ratingSystem_->getCompetitors()
        );
        for (const auto& [username, newStatus] : changes) {
            if (competitors.find(username) != competitors.end()) {
                competitors[username].status = newStatus;
            }
        }
        saveDatabase();
        updateAll();
        QMessageBox::information(this, "Sucesso", "Blacklist atualizada!");
    }
}

void MainWindow::onEditConfig() {
    ConfigDialog dialog(ratingSystem_->getConfig(), this);
    if (dialog.exec() == QDialog::Accepted) {
        auto newConfig = dialog.getConfig();

        // Update config
        ratingSystem_->setConfig(newConfig);

        // Recalculate all scores with new config
        // Need to reprocess all contests with new parameters
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Recalcular Pontos",
            "Deseja recalcular todos os pontos com a nova configuração?\n\n"
            "Isso irá reprocessar todos os contests com os novos pesos e bônus.",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            // TODO: Implement reprocessing all contests
            // For now, just recalculate scores
            ratingSystem_->recalculateAllScores();
            saveDatabase();
            updateAll();
            QMessageBox::information(this, "Sucesso",
                "Configuração atualizada e pontos recalculados!");
        } else {
            // Just save the new config without recalculating
            saveDatabase();
            QMessageBox::information(this, "Sucesso",
                "Configuração salva! Os novos valores serão usados nos próximos contests.");
        }

        statusBar_->showMessage("Configuração atualizada", 3000);
    }
}

void MainWindow::onRefresh() {
    loadDatabase();
    updateAll();
    statusBar_->showMessage("Atualizado", 2000);
}

void MainWindow::onExportRanking() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Exportar Ranking",
        "ranking.csv",
        "CSV Files (*.csv);;All Files (*)"
    );

    if (!fileName.isEmpty()) {
        // TODO: Implement export logic
        QMessageBox::information(this, "Info", "Export para: " + fileName);
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(
        this,
        "Sobre MaratonaScore",
        "MaratonaScore v2.0\n\n"
        "Sistema de ranking para competições de programação competitiva.\n\n"
        "Desenvolvido com Qt6 e C++17"
    );
}

void MainWindow::onAddMultipleContests() {
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        "Selecionar múltiplos arquivos de contest",
        QString(),
        "Excel/JSON Files (*.xlsx *.xls *.json);;All Files (*)"
    );

    if (fileNames.isEmpty()) {
        return;
    }

    int successCount = 0;
    int failCount = 0;
    QStringList errors;

    for (const QString& fileName : fileNames) {
        // Extract contest ID from filename
        QFileInfo fileInfo(fileName);
        QString baseName = fileInfo.completeBaseName();

        // Try to infer contest type and ID from filename
        // Expected formats: contest-01.json, homework-05.xlsx, etc.
        QString contestType = "contest";
        QString contestId = baseName;

        if (baseName.startsWith("homework-", Qt::CaseInsensitive)) {
            contestType = "homework";
            contestId = baseName;
        } else if (baseName.startsWith("contest-", Qt::CaseInsensitive)) {
            contestType = "contest";
            contestId = baseName;
        }

        // Default duration based on type
        int duration = (contestType == "homework") ? 60 * 24 * 7 : 300;

        bool success = ratingSystem_->processScoreboard(
            contestId.toStdString(),
            fileName.toStdString(),
            contestType.toStdString(),
            duration
        );

        if (success) {
            successCount++;
        } else {
            failCount++;
            errors.append(QString("%1: Falha ao processar").arg(baseName));
        }
    }

    saveDatabase();
    updateAll();

    QString message = QString("Processados: %1 sucesso, %2 falhas")
                          .arg(successCount).arg(failCount);
    if (!errors.isEmpty()) {
        message += "\n\nErros:\n" + errors.join("\n");
    }

    QMessageBox::information(this, "Resultado", message);
}

void MainWindow::onUpdateContest() {
    // Get list of contests to choose from
    const auto& contests = ratingSystem_->getProcessedContests();
    if (contests.empty()) {
        QMessageBox::information(this, "Info", "Nenhum contest cadastrado");
        return;
    }

    // Create a dialog to select which contest to update
    QStringList contestIds;
    for (const auto& [id, contest] : contests) {
        contestIds.append(QString::fromStdString(id));
    }

    bool ok;
    QString selectedId = QInputDialog::getItem(
        this,
        "Atualizar Contest",
        "Selecione o contest para atualizar (recarregar upsolving):",
        contestIds,
        0,
        false,
        &ok
    );

    if (!ok || selectedId.isEmpty()) {
        return;
    }

    // Get the original file path
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Selecionar arquivo atualizado do contest",
        QString(),
        "Excel/JSON Files (*.xlsx *.xls *.json);;All Files (*)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    // Get the current contest data to preserve type and duration
    auto it = contests.find(selectedId.toStdString());
    if (it == contests.end()) {
        return;
    }

    std::string contestType = it->second.type;
    int duration = it->second.duration_minutes;

    // Reprocess the contest
    bool success = ratingSystem_->processScoreboard(
        selectedId.toStdString(),
        fileName.toStdString(),
        contestType,
        duration
    );

    if (success) {
        saveDatabase();
        updateAll();
        QMessageBox::information(this, "Sucesso",
            QString("Contest '%1' atualizado com sucesso!").arg(selectedId));
    } else {
        QMessageBox::warning(this, "Erro", "Falha ao atualizar contest");
    }
}

void MainWindow::onDeleteContest() {
    const auto& contests = ratingSystem_->getProcessedContests();
    if (contests.empty()) {
        QMessageBox::information(this, "Info", "Nenhum contest cadastrado");
        return;
    }

    // Create a dialog to select which contest to delete
    QStringList contestIds;
    for (const auto& [id, contest] : contests) {
        contestIds.append(QString::fromStdString(id));
    }

    bool ok;
    QString selectedId = QInputDialog::getItem(
        this,
        "Deletar Contest",
        "Selecione o contest para deletar:",
        contestIds,
        0,
        false,
        &ok
    );

    if (!ok || selectedId.isEmpty()) {
        return;
    }

    // Confirm deletion
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmar Deleção",
        QString("Tem certeza que deseja deletar o contest '%1'?\n\n"
                "Esta ação não pode ser desfeita e todos os dados\n"
                "de participação e upsolving serão removidos.").arg(selectedId),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        // Remove contest from rating system
        auto& mutableContests = const_cast<std::map<std::string, core::models::ProcessedContest>&>(
            ratingSystem_->getProcessedContests()
        );
        mutableContests.erase(selectedId.toStdString());

        // Remove contest data from all competitors
        auto& competitors = const_cast<std::map<std::string, core::models::CompetitorData>&>(
            ratingSystem_->getCompetitors()
        );
        for (auto& [username, comp] : competitors) {
            comp.contests.erase(selectedId.toStdString());
            comp.homeworks.erase(selectedId.toStdString());
        }

        // Recalculate all scores
        ratingSystem_->recalculateAllScores();
        saveDatabase();
        updateAll();

        QMessageBox::information(this, "Sucesso",
            QString("Contest '%1' deletado com sucesso!").arg(selectedId));
    }
}

void MainWindow::onContestSelected(const QString& contestId) {
    const auto& contests = ratingSystem_->getProcessedContests();
    auto it = contests.find(contestId.toStdString());

    if (it != contests.end()) {
        contestDetail_->setContestData(
            it->first,
            it->second,
            ratingSystem_->getCompetitors()
        );
        tabWidget_->setCurrentIndex(2);  // Switch to detail tab
    }
}

}  // namespace gui
}  // namespace MaratonaScore
