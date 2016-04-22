#ifndef SETTINGBASE_H
#define SETTINGBASE_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

namespace ThreatLevel
{

class SettingArea : public QWidget
{
    Q_OBJECT

public:
    explicit SettingArea(Painter *_painter, QWidget *parent = 0);
    ~SettingArea();

private slots:
    void loadTable(int _number);        /// Загрузка таблицы с начальными данными
    void changeAreaPar(int _i, int _j); /// Изменение параметров баз

private:
    class Painter *painter;

    QTableWidget *tAreaPar;     /// Таблица параметров баз
};

}

#endif // SETTINGBASE_H
