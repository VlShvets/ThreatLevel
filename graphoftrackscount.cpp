#include "graphoftrackscount.h"

namespace ThreatLevel
{

/// Класс виджета графика количественного состава налёта
GraphOfTracksCount::GraphOfTracksCount(QWidget *_parent) : Grapher2D(_parent), numArea(0)
{
    setCSOrdMeasure(ORD_MEASURE);
    setCSAngles(false, true, false, false);
    setCSOrdExpansion(ORD_EXPANSION);
    setCSAbsValues(false);
    setCSZoomStep(STEP_ZOOM);
    setCSZoom(DEF_ZOOM);
    setCSZoomCenter(true);
}

GraphOfTracksCount::~GraphOfTracksCount()
{
}

/// Загрузка количественного состава налета
void GraphOfTracksCount::loadTrackCount(const int _numArea, const int _trackCount)
{
    /// Добавление количественного состава налета в список
    tracksCount[_numArea].push_front(_trackCount);

    /// Проверка переполнения списка
    if(tracksCount[_numArea].count() > MAX_SUM_TRACKS)
        tracksCount[_numArea].removeLast();

    repaint();
}

void GraphOfTracksCount::showGraph(const int _numArea)
{
    if(tracksCount.contains(_numArea))
        numArea = _numArea;

    repaint();
}

/// Очистка графика количественного состава налёта
void GraphOfTracksCount::resetGraph()
{
    QMap <int, QVector <int> >::iterator count = tracksCount.begin();
    for(; count != tracksCount.end(); ++count)
        count.value().clear();

    tracksCount.clear();

    repaint();
}

void GraphOfTracksCount::paintEvent(QPaintEvent *_pEvent)
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

    if(tracksCount.contains(numArea))
    {
        for(int i = tracksCount[numArea].count() - 1; i > 0; --i)
        {
            p.drawLine(QPointF(- i      - SHIFT, tracksCount[numArea].at(i)),
                       QPointF(- i + 1  - SHIFT, tracksCount[numArea].at(i - 1)));
        }
    }

    p.end();
}

}
