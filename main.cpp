#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Integral Software");
    app.setApplicationName("CamGrab");

    MainWindow window;
    window.show();

    return app.exec();
}
