#include "trackgraph.h"

namespace ThreatLevel
{

/// Класс виджета графика количественного состава налёта
TrackGraph::TrackGraph(QWidget *_parent) : Grapher2D(_parent), numArea(0)
{
    setCSOrdMeasure(ORD_MEASURE);
    setCSAngles(false, true, false, false);
    setCSOrdExpansion(ORD_EXPANSION);
    setCSAbsValues(false);
    setCSZoomStep(STEP_ZOOM);
    setCSZoom(DEF_ZOOM);
    setCSZoomCenter(true);
}

TrackGraph::~TrackGraph()
{
}

/// Загрузка количественного состава налета
void TrackGraph::loadTrackCount(const int _numArea, const int _trackCount)
{
    /// Добавление количественного состава налета в список
    trackCount[_numArea].push_front(_trackCount);

    /// Проверка переполнения списка
    if(trackCount[_numArea].count() > MAX_SUM_TRACKS)
        trackCount[_numArea].removeLast();

    repaint();
}

void TrackGraph::showGraph(const int _numArea)
{
    if(trackCount.contains(_numArea))
        numArea = _numArea;

    repaint();
}

/// Очистка графика количественного состава налёта
void TrackGraph::resetGraph()
{
    QMap <int, QVector <int> >::iterator count = trackCount.begin();
    for(; count != trackCount.end(); ++count)
        count.value().clear();

    trackCount.clear();

    repaint();
}

void TrackGraph::paintEvent(QPaintEvent *_pEvent)
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

    if(trackCount.contains(numArea))
    {
        for(int i = trackCount[numArea].count() - 1; i > 0; --i)
        {
            p.drawLine(QPointF(- i      - SHIFT, trackCount[numArea].at(i)),
                       QPointF(- i + 1  - SHIFT, trackCount[numArea].at(i - 1)));
        }
    }

    p.end();
}

}
