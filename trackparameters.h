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
    explicit            TrackParameters(QWidget *parent = 0);
    ~TrackParameters();

    /// Получить количество КР
    inline int          getCMCount() const;

    /// Получить параметр КР по номеру строки и номеру столбца
    inline float        getCMPar(int _row, int _column) const;

    /// Получить количество БЦ
    inline int          getBGCount() const;

    /// Получить параметр БЦ по номеру строки и номеру столбца
    inline float        getBGPar(int _row, int _column) const;

private slots:
    /// Изменение количества КР в группе
    void                CMInGroupCountChanged(int _count);

    /// Начальная инициализация параметров КР
    void                initCMPar(int _count);

    /// Начальная инициализация параметров БЦ
    void                initBGPar(int _count);

private:
    int                 CMInGroupCount;                 /// Количество групп КР

    QLCDNumber          *lGroupsOfCMCount;              /// Виджет отображения количества групп КР
    QLCDNumber          *lCMInGroupCount;               /// Виджет отображения количества КР в группе
    QTableWidget        *tCMPar;                        /// Виджет таблицы параметров КР
    QLCDNumber          *lBGCount;                      /// Виджет отображения количества БЦ
    QTableWidget        *tBGPar;                        /// Виджет таблицы параметров БЦ

    static const int    GROUPS_OF_CM_MAX_COUNT  = 10;   /// Максимальное количество групп КР
    static const int    GROUPS_OF_CM_DEF_COUNT  = 5;    /// Количество групп КР по умолчанию

    static const int    CM_IN_GROUP_MAX_COUNT   = 15;   /// Максимальное количество КР в группе
    static const int    CM_IN_GROUP_DEF_COUNT   = 1;    /// Количество КР в группе по умолчанию

    static const int    BG_MAX_COUNT            = 5;    /// Максимальное количество БЦ
    static const int    BG_DEF_COUNT            = 2;    /// Количество БЦ по умолчанию
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
