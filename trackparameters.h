#ifndef TRACKPARAMETERS_H
#define TRACKPARAMETERS_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

namespace ThreatLevel
{

/// Класс виджета редактирования параметров трасс
class TrackParameters : public QWidget
{
    Q_OBJECT

public:
    explicit TrackParameters(QWidget *parent = 0);
    ~TrackParameters();

    /// Получить количество КР
    inline int getCMCount() const;

    /// Получить параметр КР по номеру строки и номеру столбца
    inline float getCMPar(int _row, int _column) const;

    /// Получить количество БЦ
    inline int getBGCount() const;

    /// Получить параметр БЦ по номеру строки и номеру столбца
    inline float getBGPar(int _row, int _column) const;

private slots:
    /// Изменение количества групп КР
    void countGroupsOfCMChanged(int _count);

    /// Изменение количества КР в группе
    void countCMinGroupChanged(int _count);

    /// Изменение количества БЦ
    void countBGChanged(int _count);

private:
    /// Начальная инициализация параметров КР
    void initCMPar(int _countGroupsOfCM, int _countCMinGroup);

    /// Начальная инициализация параметров БЦ
    void initBGPar(int _countBG);

    QLCDNumber      *lCountGroupsOfCM;  /// Виджет отображения количества групп КР
    QLCDNumber      *lCountCMinGroup;   /// Виджет отображения количества КР в группе
    QTableWidget    *tCMPar;            /// Виджет таблицы параметров КР
    QLCDNumber      *lCountBG;          /// Виджет отображения количества БЦ
    QTableWidget    *tBGPar;            /// Виджет таблицы параметров БЦ

    static const int    MAX_COUNT_GROUPS_OF_CM  = 10;   /// Максимальное количество групп КР
    static const int    DEF_COUNT_GROUPS_OF_CM  = 5;    /// Количество групп КР по умолчанию

    static const int    MAX_COUNT_CM_IN_GROUP   = 12;   /// Максимальное количество КР в группе
    static const int    DEF_COUNT_CM_IN_GROUP   = 1;    /// Количество КР в группе по умолчанию

    static const int    DIST_BETWEEN_TRACKS    = 6000;  /// Расстояние между КР в группе

    static const int    MAX_COUNT_BG            = 5;    /// Максимальное количество БЦ
    static const int    DEF_COUNT_BG            = 2;    /// Количество БЦ по умолчанию
};

/// Получить количество КР
int TrackParameters::getCMCount() const
{
    return tCMPar->rowCount();
}

/// Получить параметр КР по номеру строки и номеру столбца
float TrackParameters::getCMPar(int _row, int _column) const
{
    return tCMPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

/// Получить количество БЦ
int TrackParameters::getBGCount() const
{
    return tBGPar->rowCount();
}

/// Получить параметр БЦ по номеру строки и номеру столбца
float TrackParameters::getBGPar(int _row, int _column) const
{
    return tBGPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

}

#endif // SETTINGTRACK_H
