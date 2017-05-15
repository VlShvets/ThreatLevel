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

    /// Очистка таблицы результатов
    void            resetTable();

    /// Установление ЗКВ
    inline void     setAreas(const  QMap <int, Area> &_areas);

    /// Установление трасс
    inline void     setTracks(const QMap <int, Track> &_tracks);

protected:
    /// Обновление таблицы результатов
    void            timerEvent(QTimerEvent *);

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

    GraphOfTracksCount  *graphOfTrackCount;     /// Класс графика количественного состава налёта

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Указатели на словари параметров ЗКВ, эталонов и трасс
    const QMap <int, Area>      *areas;     /// Словарь ЗКВ         <номер, структура параметров>
    const QMap <int, Track>     *tracks;    /// Словарь трасс       <номер, структура параметров>

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Параметры обновления таблицы результатов
    static const int    RESULTS_INTERVAL    = 100;  /// Интервал обновления таблицы результатов (в мс)

    /// Параметры поля отображения количественного состава налета
    static const int    LCD_DIGIT_COUNT     = 4;    /// Число цифр в поле отображения количественного состава налета
};

/// Установление ЗКВ
void Results::setAreas(const QMap <int, Area> &_areas)
{
    areas = &_areas;
}

/// Установление трасс
void Results::setTracks(const QMap <int, Track> &_tracks)
{
    tracks = &_tracks;
}

/// Выбор строки в таблице
void Results::tableSelectionChanged(QItemSelection, QItemSelection)
{
    graphOfTrackCount->showGraph(tResults->selectedItems().first()->text().toInt());
}

}

#endif // RESULTS_H
