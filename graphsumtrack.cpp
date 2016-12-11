#include "graphsumtrack.h"

namespace ThreatLevel
{

/// Класс виджета графика количественного состава налёта
GraphSumTrack::GraphSumTrack(QWidget *_parent) : Grapher2D(_parent)
{
    setCSOrdMeasure(ORD_MEASURE);
    setCSAngles(false, true, false, false);
    setCSOrdExpansion(ORD_EXPANSION);
    setCSAbsValues(false);
    setCSZoomStep(STEP_ZOOM);
    setCSZoom(DEF_ZOOM);
    setCSZoomCenter(true);
}

GraphSumTrack::~GraphSumTrack()
{
}

/// Загрузка количественного состава налета
void GraphSumTrack::loadGraph(const int _sumTrackCount)
{
    /// Добавление количественного состава налета в список
    sumTrack.push_front(_sumTrackCount);

    /// Проверка переполнения списка
    if(sumTrack.count() > MAX_SUM_TRACKS)
        sumTrack.removeLast();

    repaint();
}

/// Очистка графика количественного состава налёта
void GraphSumTrack::resetGraph()
{
    sumTrack.clear();

    repaint();
}

void GraphSumTrack::paintEvent(QPaintEvent *_pEvent)
{
    Grapher2D::paintEvent(_pEvent);

    QPainter p(this);
    p.translate(getCSAbsTranslate(), getCSOrdTranslate());
    p.scale(getCSAbsScale(), getCSOrdScale());

    QPen pen;
    pen.setCosmetic(true);

    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::darkRed);
    pen.setWidth(3);
    p.setPen(pen);

    for(int i = sumTrack.count() - 1; i > 0; --i)
        p.drawLine(QPointF(- i - SHIFT, sumTrack.at(i)), QPointF(- i + 1 - SHIFT, sumTrack.at(i - 1)));
}

}
