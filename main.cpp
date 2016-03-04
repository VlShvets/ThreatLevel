#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ThreatLevel::MainWindow mainWindow;
    mainWindow.showMaximized();

    return a.exec();
}
