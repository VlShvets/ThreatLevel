#ifndef AREAPARAMETERS_H
#define AREAPARAMETERS_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

namespace ThreatLevel
{

/// Класс виджета редактирования параметров позиционных районов
class AreaParameters : public QWidget
{
    Q_OBJECT

public:
    explicit AreaParameters(QWidget *parent = 0);
    ~AreaParameters();

    /// Получить количество ПР
    inline int getCount() const;

    /// Получить параметр ПР по номеру строки и номеру столбца
    inline float getPar(int _row, int _column) const;

private slots:
    /// Начальная инициализация параметров ПР
    void initPar(int _count);

private:
    QTableWidget    *tAreaPar; /// Виджет таблицы параметров ПР

    static const int    MAX_COUNT_AREAS = 5;    /// Максимальное количество ПР
    static const int    DEF_COUNT_AREAS = 5;    /// Количество ПР по умолчанию
};

/// Получить количество ПР
int AreaParameters::getCount() const
{
    return tAreaPar->rowCount();
}

/// Получить параметр ПР по номеру строки и номеру столбца
float AreaParameters::getPar(int _row, int _column) const
{
    return tAreaPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

}

#endif // AREAPARAMETERS_H
