/**
 * @file main_gui.cpp
 * @brief Entry point for MaratonaScore GUI application
 */

#include <QApplication>
#include "MaratonaScore/gui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("MaratonaScore");
    app.setApplicationVersion("2.0.0");
    app.setOrganizationName("MaratonaCIn");

    MaratonaScore::gui::MainWindow window;
    window.show();

    return app.exec();
}
