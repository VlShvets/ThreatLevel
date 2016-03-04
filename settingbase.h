#ifndef SETTINGBASE_H
#define SETTINGBASE_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>

#include "painter.h"

namespace ThreatLevel
{

class SettingBase : public QWidget
{
    Q_OBJECT

public:
    explicit SettingBase(Painter *_painter, QWidget *parent = 0);
    ~SettingBase();

private slots:
    void loadTable(int _number);        /// Загрузка таблицы с начальными данными
    void changeParBase(int _i, int _j); /// Изменение параметров баз

private:
    class Painter *painter;

    QTableWidget *tParBase;     /// Таблица параметров баз
};

}

#endif // SETTINGBASE_H
