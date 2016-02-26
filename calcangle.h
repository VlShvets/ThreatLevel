#ifndef CALCANGLE_H
#define CALCANGLE_H

#include <QtCore/qglobal.h>
#include <QApplication>
#include <QMainWindow>
#include <QTextCodec>

#include "tool.h"

class CalcAngle : public QMainWindow
{
public:
    CalcAngle(QWidget *_parent = 0);
    ~CalcAngle();

private:
    Painter *painter;
    Tool *tool;
};

#endif // CALCANGLE_H
