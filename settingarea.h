#ifndef SETTINGAREA_H
#define SETTINGAREA_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

namespace ThreatLevel
{

const int MAXNUMAREAS = 5;
const int DEFNUMAREAS = 3;

class SettingArea : public QWidget  /// Класс позиционных районов
{
    Q_OBJECT

public:
    explicit SettingArea(QWidget *parent = 0);
    ~SettingArea();

    inline int getCount();                          /// Полчить количество
    inline float getPar(int _row, int _column);     /// Получить параметр

private slots:
    void initPar(int _number);                      /// Начальная инициализация параметров

private:
    QTableWidget *tAreaPar;                         /// Таблица параметров
};

int SettingArea::getCount()
{
    return tAreaPar->rowCount();
}

float SettingArea::getPar(int _row, int _column)
{
    return tAreaPar->item(_row, _column)->text().toFloat();
}

}

#endif // SETTINGAREA_H
