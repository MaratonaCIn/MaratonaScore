#ifndef MARATONASCORE_GUI_ADDCONTESTDIALOG_H
#define MARATONASCORE_GUI_ADDCONTESTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QString>

namespace MaratonaScore {
namespace gui {

/**
 * @brief Dialog for adding a new contest from XLSX file
 */
class AddContestDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddContestDialog(QWidget* parent = nullptr);

    QString getContestId() const;
    QString getXlsxPath() const;
    QString getContestType() const;
    int getDuration() const;

private slots:
    void onBrowse();
    void onOk();

private:
    void setupUI();

    QLineEdit* contestIdEdit_;
    QLineEdit* xlsxPathEdit_;
    QComboBox* typeCombo_;
    QComboBox* durationCombo_;
    QPushButton* browseButton_;
};

}  // namespace gui
}  // namespace MaratonaScore

#endif  // MARATONASCORE_GUI_ADDCONTESTDIALOG_H
