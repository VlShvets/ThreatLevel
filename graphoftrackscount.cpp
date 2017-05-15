#include "graphoftrackscount.h"

namespace ThreatLevel
{

/// Класс виджета графика количественного состава налёта
GraphOfTracksCount::GraphOfTracksCount(QWidget *_parent)
    : Grapher2D(_parent), numArea(0)
{
    setCSOrdMeasure(ORD_MEASURE);
    setCSAngles(false, true, false, false);
    setCSOrdExpansion(ORD_EXPANSION);
    setCSAbsValues(false);
    setCSZoomStep(STEP_ZOOM);
    setCSZoom(DEF_ZOOM);
    setCSZoomCenter(true);

    startTimer(GRAPH_INTERVAL);
}

GraphOfTracksCount::~GraphOfTracksCount()
{
    resetGraph();
}

/// Загрузка количественного состава налета
void GraphOfTracksCount::loadTrackCount(const QMap <int, Area> &_areas)
{
    QMap <int, Area>::const_iterator area = _areas.constBegin();
    for(; area != _areas.end(); ++area)
    {
        /// Добавление количественного состава налета в список
        tracksCount[area.key()].push_front(area.value().raidCount);

        /// Проверка переполнения списка
        if(tracksCount[area.key()].count() > MAX_SUM_TRACKS)
            tracksCount[area.key()].removeLast();
    }

    /// Добавление количественного состава налета в список
    tracksCount[0].push_front(Area::raidSumCount + Area::detectTracksCount);

    /// Проверка переполнения списка
    if(tracksCount[area.key()].count() > MAX_SUM_TRACKS)
        tracksCount[area.key()].removeLast();
}

/// Выбор ЗКВ для отображения
void GraphOfTracksCount::showGraph(const int _numArea)
{
    if(tracksCount.contains(_numArea))
        numArea = _numArea;
}

/// Очистка графика количественного состава налёта
void GraphOfTracksCount::resetGraph()
{
    numArea = 0;

    QMap <int, QVector <int> >::iterator count = tracksCount.begin();
    for(; count != tracksCount.end(); ++count)
        count.value().clear();

    tracksCount.clear();
}

/// Обновление отрисокви
void GraphOfTracksCount::timerEvent(QTimerEvent *)
{
    repaint();
}

/// Отрисовка графика количественного состава налета
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
    pen.setWidth(WIDTH);
    p.setPen(pen);

    if(tracksCount.contains(numArea))
    {
        for(int i = tracksCount.value(numArea).count() - 1; i > 0; --i)
        {
            p.drawLine(QPointF(- i      - SHIFT, tracksCount.value(numArea).at(i)),
                       QPointF(- i + 1  - SHIFT, tracksCount.value(numArea).at(i - 1)));
        }
    }

    p.end();
}

}
