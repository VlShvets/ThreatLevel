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
    static constexpr float  ORD_MEASURE     = 1.0;      /// Масштаб оси ординат
    static constexpr float  ORD_EXPANSION   = 2.0;      /// Параметр растяжения оси ординат
};

}

#endif // GRAPHOFTRACKSCOUNT_H
