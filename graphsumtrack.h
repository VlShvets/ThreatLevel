#ifndef GRAPHSUMTRACK_H
#define GRAPHSUMTRACK_H

#include "Grapher2D.h"

namespace ThreatLevel
{

/// Класс виджета графика количественного состава налёта
class GraphSumTrack : public Grapher2D
{
public:
    explicit GraphSumTrack(QWidget *_parent = 0);
    ~GraphSumTrack();

    /// Загрузка количественного состава налета
    void loadGraph(const int _sumTrackCount);

    /// Очистка графика количественного состава налёта
    void resetGraph();

protected:
    void paintEvent(QPaintEvent *_pEvent);

private:
    QVector <int> sumTrack; /// Список количественного состава налета по всем ПР во времени

    static const int MAX_SUM_TRACKS = 10000;    /// Максимальный размер списка количественного состава налета

    static const int SHIFT = 5;                 /// Сдвиг график относительно правого края

    static const float ORD_MEASURE = 1.0;       /// Масштаб оси ординат
    static const float ORD_EXPANSION = 2.0;     /// Параметр растяжения оси ординат
    static const int STEP_ZOOM = 3;             /// Шаг масштабирования
    static const int DEF_ZOOM = 10;             /// Масштаб отображения по умолчанию
};

}

#endif // GRAPHSUMTRACK_H
