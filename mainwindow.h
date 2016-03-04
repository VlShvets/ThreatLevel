#ifndef CALCANGLE_H
#define CALCANGLE_H

#include <QApplication>
#include <QMainWindow>
#include <QTextCodec>
#include <QToolBar>
#include <QDockWidget>

#include "settingbase.h"
#include "settingtrack.h"
#include "settingtotal.h"

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
    SettingBase *settingBase;
    SettingTrack *settingTrack;
    SettingTotal *settingTotal;
};

}

#endif // CALCANGLE_H
