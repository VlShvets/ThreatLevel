#ifndef CALCANGLE_H
#define CALCANGLE_H

#include <QApplication>
#include <QMainWindow>
#include <QTextCodec>
#include <QToolBar>
#include <QDockWidget>

#include "areaparameters.h"
#include "trackparameters.h"
#include "settings.h"

namespace ThreatLevel
{

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *_parent = 0);
    ~MainWindow();

private:
    Results *results;
    Painter *painter;
    AreaParameters *areaParameters;
    TrackParameters *trackParameters;
    Settings *settings;
};

}

#endif // CALCANGLE_H
