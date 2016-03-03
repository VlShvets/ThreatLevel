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

class CalcAngle : public QMainWindow
{
public:
    CalcAngle(QWidget *_parent = 0);
    ~CalcAngle();

private:
    Results *results;
    Painter *painter;
    SettingBase *settingBase;
    SettingTrack *settingTrack;
    SettingTotal *settingTotal;
};

#endif // CALCANGLE_H
