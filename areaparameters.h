#ifndef AREAPARAMETERS_H
#define AREAPARAMETERS_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

namespace ThreatLevel
{

const int MAXNUMAREAS = 5;  /// Максимальное количество ПР
const int DEFNUMAREAS = 3;  /// Количество ПР по умолчанию

class AreaParameters : public QWidget  /// Класс позиционных районов
{
    Q_OBJECT

public:
    explicit AreaParameters(QWidget *parent = 0);
    ~AreaParameters();

    inline int getCount();                          /// Получить количество ПР
    inline float getPar(int _row, int _column);     /// Получить параметр ПР

private slots:
    void initPar(int _number);                      /// Начальная инициализация параметров ПР

private:
    QTableWidget *tAreaPar;                         /// Таблица параметров ПР
};

int AreaParameters::getCount()
{
    return tAreaPar->rowCount();
}

float AreaParameters::getPar(int _row, int _column)
{
    return tAreaPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

}

#endif // AREAPARAMETERS_H
