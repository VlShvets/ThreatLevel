#ifndef TRACKGRAPH_H
#define TRACKGRAPH_H

#include "Grapher2D.h"

namespace ThreatLevel
{

/// Класс виджета графика количественного состава налёта
class TrackGraph : public Grapher2D
{
public:
    explicit    TrackGraph(QWidget *_parent = 0);
    ~TrackGraph();

    /// Загрузка количественного состава налета
    void        loadTrackCount(const int _numArea, const int _trackCount);

    /// Выбор ПР для отображения
    void        showGraph(const int _numArea);

    /// Очистка графика количественного состава налёта
    void        resetGraph();

protected:
    void        paintEvent(QPaintEvent *_pEvent);

private:

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Выбранный позиционный район
    int                         numArea;        /// Выбранный для отображения в данный момент времени ПР

    /// История количественного состава налета
    QMap <int, QVector <int> >  trackCount;     /// Список количественного состава налета во времени


    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Размер истории количественного состава налета
    static const int    MAX_SUM_TRACKS  = 1000;     /// Максимальный размер списка количественного состава налета во времени

    /// Параметры отрисовки
    static const int    SHIFT           = 5;        /// Сдвиг график относительно правого края
    static const int    STEP_ZOOM       = 3;        /// Шаг масштабирования
    static const int    DEF_ZOOM        = 10;       /// Масштаб отображения по умолчанию
    static const float  ORD_MEASURE     = 1.0;      /// Масштаб оси ординат
    static const float  ORD_EXPANSION   = 2.0;      /// Параметр растяжения оси ординат
};

}

#endif // TRACKGRAPH_H
