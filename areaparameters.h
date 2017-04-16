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
    explicit        AreaParameters(QWidget *parent = 0);
    ~AreaParameters();

    /// Получить количество ПР
    inline int      getCount() const;

    /// Получить параметр ПР по номеру строки и номеру столбца
    inline float    getPar(int _row, int _column) const;

private slots:
    /// Начальная инициализация параметров ПР
    void            initPar(int _count);

private:
    /// Указатели на виджеты
    QTableWidget    *tAreaPar;  /// Виджет таблицы параметров ПР

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Количество позиционных районов
    static const int    AREAS_DEF_COUNT = 5;    /// Количество ПР по умолчанию
    static const int    AREAS_MAX_COUNT = 5;    /// Максимальное количество ПР
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
