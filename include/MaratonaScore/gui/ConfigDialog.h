#ifndef MARATONASCORE_GUI_CONFIGDIALOG_H
#define MARATONASCORE_GUI_CONFIGDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTabWidget>

#include "MaratonaScore/core/models/ScoringConfig.h"

namespace MaratonaScore {
namespace gui {

/**
 * @brief Dialog for editing scoring configuration
 *
 * Allows editing:
 * - Contest: base weight, contests to double, rank bonus settings
 * - Homework: base weight, contests to double, rank bonus settings
 * - Upsolving: points per problem
 */
class ConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfigDialog(const core::models::ScoringConfig& config,
                          QWidget* parent = nullptr);

    core::models::ScoringConfig getConfig() const;

private:
    void setupUI();
    void populateFields();

    core::models::ScoringConfig config_;

    // Contest settings
    QDoubleSpinBox* contestWeightSpin_;
    QSpinBox* contestsToDoubleSpin_;
    QSpinBox* contestTopNSpin_;
    QDoubleSpinBox* contestMaxBonusSpin_;

    // Homework settings
    QDoubleSpinBox* homeworkWeightSpin_;
    QSpinBox* homeworkToDoubleSpin_;
    QSpinBox* homeworkTopNSpin_;
    QDoubleSpinBox* homeworkMaxBonusSpin_;

    // Upsolving settings
    QDoubleSpinBox* upsolvingPointsSpin_;
};

}  // namespace gui
}  // namespace MaratonaScore

#endif  // MARATONASCORE_GUI_CONFIGDIALOG_H
