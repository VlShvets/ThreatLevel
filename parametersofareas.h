#ifndef PARAMETERSOFAREAS_H
#define PARAMETERSOFAREAS_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>
#include <QLabel>

namespace ThreatLevel
{

/// Класс виджета редактирования параметров ЗКВ
class ParametersOfAreas : public QWidget
{
    Q_OBJECT

public:
    explicit        ParametersOfAreas(QWidget *parent = 0);
    ~ParametersOfAreas();

    /// Возвращение количества ЗКВ
    inline int      getCount() const;

    /// Возвращение параметра ЗКВ по номеру строки и номеру столбца
    inline float    getPar(int _row, int _column) const;

private slots:
    /// Начальная инициализация параметров ЗКВ
    void            initPar(int _count);

private:
    /// Указатели на виджеты
    QTableWidget    *tAreaPar;  /// Виджет таблицы параметров ЗКВ

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Количество ЗКВ
    static const int    AREAS_MIN_COUNT     = 1;    /// Минимальное количество ЗКВ
    static const int    AREAS_DEF_COUNT     = 5;    /// Количество ЗКВ по умолчанию
    static const int    AREAS_MAX_COUNT     = 5;    /// Максимальное количество ЗКВ
};

/// Возвращение количества ЗКВ
int ParametersOfAreas::getCount() const
{
    return tAreaPar->rowCount();
}

/// Возвращение параметра ЗКВ по номеру строки и номеру столбца
float ParametersOfAreas::getPar(int _row, int _column) const
{
    return tAreaPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

}

#endif // PARAMETERSOFAREAS_H
