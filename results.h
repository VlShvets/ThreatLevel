#ifndef RESULTS_H
#define RESULTS_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QTableWidget>

#include "area.h"
#include "track.h"
#include "graphoftrackscount.h"

namespace ThreatLevel
{

/// Класс виджета отображения таблицы результатов
class Results : public QWidget
{
    Q_OBJECT

public:
    explicit        Results(class GraphOfTracksCount *_graphOfTrackCount, QWidget *parent = 0);
    ~Results();

    /// Загрузка результатов
    void            loadTable(const QMap <int, Area> &_areas, const QMap <int, Track> &_tracks);

    /// Очистка таблицы результатов
    void            resetTable();

private slots:
    /// Выбор строки в таблице
    inline void     tableSelectionChanged(QItemSelection, QItemSelection);

private:
    /// Указатели на виджеты
    QLCDNumber      *lcdMaxSumTrack;    /// Виджет отображения числа количественного состава налета по всем ЗКВ
    QTableWidget    *tResults;          /// Виджет отображения таблицы результатов

    /// --------------------------------------------------
    /// Указатели на объекты классов
    /// --------------------------------------------------

    GraphOfTracksCount  *graphOfTrackCount;        /// Класс графика количественного состава налёта

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Параметр поля отображения максимального количественного состава налета
    static const int    LCD_DIGIT_COUNT = 4;    /// Число цифр в поле отображения максимального количественного состава налета
};

/// Выбор строки в таблице
void Results::tableSelectionChanged(QItemSelection, QItemSelection)
{
    graphOfTrackCount->showGraph(tResults->selectedItems().first()->text().toInt());
}

}

#endif // RESULTS_H
