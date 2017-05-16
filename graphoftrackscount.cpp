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

/// Очистка графика количественного состава налёта
void GraphOfTracksCount::resetGraph()
{
    QMap <int, QVector <int> >::iterator count = tracksCount.begin();
    for(; count != tracksCount.end(); ++count)
        count.value().clear();

    tracksCount.clear();

    numArea = 0;
}

/// Загрузка количественного состава налета
void GraphOfTracksCount::loadTracksCount()
{
    QMap <int, Area>::const_iterator area = areas->constBegin();
    for(; area != areas->end(); ++area)
    {
        /// Добавление количественного состава налета по ЗКВ
        addTrackCount(area.key(), area.value().raidCount);
    }

    /// Добавление суммарного количественного состава налета
    addTrackCount(0, Area::raidSumCount + Area::detectTracksCount);
}

/// Добавление количественного состава налета
void GraphOfTracksCount::addTrackCount(const int _num, const int _count)
{
    QMutexLocker locker(&mutex);

    tracksCount[_num].push_front(_count);

    /// Проверка переполнения списка
    if(tracksCount[_num].count() > MAX_SUM_TRACKS)
        tracksCount[_num].removeLast();
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

    QMutexLocker locker(&mutex);

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
