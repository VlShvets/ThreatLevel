#ifndef RESULTS_H
#define RESULTS_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QTableWidget>

#include "area.h"
#include "track.h"
#include "trackgraph.h"

namespace ThreatLevel
{

/// Класс виджета отображения результатов
class Results : public QWidget
{
    Q_OBJECT

public:
    explicit            Results(TrackGraph *_trackGraph, QWidget *parent = 0);
    ~Results();

    /// Загрузка результатов
    void                loadTable(const QMap <int, Area> &_areas, const QMap <int, Track> &_tracks);

    /// Очистка таблицы результатов
    void                resetTable();

private slots:
    /// Выбор строки в таблице
    void tableSelectionChanged(QItemSelection, QItemSelection);

private:
    QLCDNumber          *lcdMaxSumTrack;        /// Виджет отображающий максимимальный количественный состав налета
    QTableWidget        *tResults;              /// Виджет отображения результатов

    static const int    LCD_DIGIT_COUNT = 4;    /// Число цифр в поле отображения максимального количественного состава налета

    TrackGraph          *trackGraph;            /// Виджет графика количественного состава налёта
};

}

#endif // RESULTS_H
