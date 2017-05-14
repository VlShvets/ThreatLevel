#ifndef GRAPHOFTRACKSCOUNT_H
#define GRAPHOFTRACKSCOUNT_H

#include "Grapher2D.h"

namespace ThreatLevel
{

/// Класс виджета графика количественного состава налёта
class GraphOfTracksCount : public Grapher2D
{
public:
    explicit    GraphOfTracksCount(QWidget *_parent = 0);
    ~GraphOfTracksCount();

    /// Загрузка количественного состава налета
    void        loadTrackCount(const int _numArea, const int _trackCount);

    /// Выбор ЗКВ для отображения
    void        showGraph(const int _numArea);

    /// Очистка графика количественного состава налёта
    void        resetGraph();

protected:
    /// Обновление отрисокви
    void        timerEvent(QTimerEvent *);

    /// Отрисовка графика количественного состава налета
    void        paintEvent(QPaintEvent *_pEvent);

private:
    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Выбранный ЗКВ
    int                         numArea;        /// Выбранный для отображения в данный момент времени ЗКВ

    /// История количественного состава налета
    QMap <int, QVector <int> >  tracksCount;     /// Список количественного состава налета во времени

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    static const int    MAX_SUM_TRACKS  = 1000;     /// Максимальный размер списка количественного состава налета во времени
    static const int    GRAPH_INTERVAL  = 100;      /// Интервал обновления графика (в мс)

    /// Параметры отрисовки
    static const int    SHIFT           = 5;        /// Сдвиг график относительно правого края
    static const int    STEP_ZOOM       = 3;        /// Шаг масштабирования
    static const int    DEF_ZOOM        = 10;       /// Масштаб отображения по умолчанию
    static constexpr float  ORD_MEASURE     = 1.0;      /// Масштаб оси ординат
    static constexpr float  ORD_EXPANSION   = 2.0;      /// Параметр растяжения оси ординат
};

}

#endif // GRAPHOFTRACKSCOUNT_H
