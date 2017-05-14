#ifndef PARAMETERSOFETALONS_H
#define PARAMETERSOFETALONS_H

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>
#include <QtMath>

namespace ThreatLevel
{

/// Класс виджета редактирования параметров эталонов
class ParametersOfEtalons : public QWidget
{
    Q_OBJECT

public:
    explicit        ParametersOfEtalons(QWidget *parent = 0);
    ~ParametersOfEtalons();

    /// Возвращение количества КР
    inline int      getCMCount() const;

    /// Возвращение параметра КР по номеру строки и номеру столбца
    inline float    getCMPar(int _row, int _column) const;

    /// Возвращение количества БЦ
    inline int      getBGCount() const;

    /// Возвращение параметра БЦ по номеру строки и номеру столбца
    inline float    getBGPar(int _row, int _column) const;

private slots:
    /// Изменение количества КР в группе
    void            CMInGroupCountChanged(int _count);

    /// Начальная инициализация параметров КР
    void            initCMPar(int _count);

    /// Начальная инициализация параметров БЦ
    void            initBGPar(int _count);

private:
    /// Указатели на виджеты
    QLCDNumber      *lGroupsOfCMCount;  /// Виджет отображения количества групп КР
    QLCDNumber      *lCMInGroupCount;   /// Виджет отображения количества КР в группе
    QTableWidget    *tCMPar;            /// Виджет таблицы параметров КР
    QLCDNumber      *lBGCount;          /// Виджет отображения количества БЦ
    QTableWidget    *tBGPar;            /// Виджет таблицы параметров БЦ

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Количество групп крылатых ракет
    int     CMInGroupCount;     /// Количество групп КР

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Количество групп крылатых ракет
    static const int    GROUPS_OF_CM_MIN_COUNT  = 1;    /// Минимальное количество групп КР
    static const int    GROUPS_OF_CM_DEF_COUNT  = 5;    /// Количество групп КР по умолчанию
    static const int    GROUPS_OF_CM_MAX_COUNT  = 10;   /// Максимальное количество групп КР

    /// Количество крылатых ракет в группе
    static const int    CM_IN_GROUP_MIN_COUNT   = 1;    /// Минимальное количество КР в группе
    static const int    CM_IN_GROUP_DEF_COUNT   = 1;    /// Количество КР в группе по умолчанию
    static const int    CM_IN_GROUP_MAX_COUNT   = 15;   /// Максимальное количество КР в группе

    /// Количество балистических целей
    static const int    BG_MIN_COUNT            = 1;    /// Минимальное количество БЦ
    static const int    BG_DEF_COUNT            = 2;    /// Количество БЦ по умолчанию
    static const int    BG_MAX_COUNT            = 5;    /// Максимальное количество БЦ
};

/// Возвращение количества КР
int ParametersOfEtalons::getCMCount() const
{
    return tCMPar->rowCount();
}

/// Возвращение параметра КР по номеру строки и номеру столбца
float ParametersOfEtalons::getCMPar(int _row, int _column) const
{
    return tCMPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

/// Возвращение количества БЦ
int ParametersOfEtalons::getBGCount() const
{
    return tBGPar->rowCount();
}

/// Возвращение параметра БЦ по номеру строки и номеру столбца
float ParametersOfEtalons::getBGPar(int _row, int _column) const
{
    return tBGPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

}

#endif // PARAMETERSOFETALONS_H
