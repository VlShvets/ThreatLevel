#include "painter.h"

namespace ThreatLevel
{

Painter::Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent)
    : Grapher2D(_parent), areaParameters(_areaParameters), trackParameters(_trackParameters), results(_results)
{
    setCSAbsMeasure(10000);
    setCSOrdMeasure(10000);
    setCSZoom(5);

    loadAreaPar();
    loadTrackPar();
}

Painter::~Painter()
{
    area.clear();
    track.clear();
}

void Painter::reStart()
{
    loadAreaPar();
    loadTrackPar();

    repaint();
}

void Painter::paintEvent(QPaintEvent * _pEvent)
{
    Grapher2D::paintEvent(_pEvent);

    QPainter p(this);
    p.translate(getCSAbsTranslate(), getCSOrdTranslate());
    p.scale(getCSAbsScale(), getCSOrdScale());

    QPen pen;
    pen.setCosmetic(true);

    /// Позиционные районы
    pen.setColor(Qt::darkBlue);
    pen.setWidth(3);
    p.setPen(pen);
    for(int i = 0; i < area.count(); ++i)
        p.drawEllipse(area.at(i).pos, area.at(i).radius, area.at(i).radius);

    /// Трассы
    pen.setColor(Qt::darkGreen);
    for(int j = 0; j < track.count(); ++j)
    {
        /// Отрисовка трассы
        pen.setWidth(6);
        p.setPen(pen);
        p.drawPoint(track.at(j).pos);

        /// Отрисовка курса
        if(track.at(j).farTarget != NULL)
        {
            pen.setWidth(1);
            p.setPen(pen);
            p.drawLine(track.at(j).pos, track.at(j).pos + QPointF(track.at(j).farTarget->dist *
                                                                  qSin(track.at(j).angV) / qCos(track.at(j).nearTarget->angToV),
                                                                  track.at(j).farTarget->dist *
                                                                  qCos(track.at(j).angV) / qCos(track.at(j).nearTarget->angToV)));
        }

    }

    /// Отрисовка расстояний от трасс до центров ПР и углов видимости ПР
//    pen.setWidth(1);
//    for(int j = 0; j < track.count(); ++j)
//    {
//        for(int i = 0; i < area.count(); ++i)
//        {
//            /// Кратчайшее растояние до ПР
//            pen.setColor(Qt::darkRed);
//            p.setPen(pen);
//            p.drawLine(area.at(i).pos, track.at(j).pos);

//            /// Угол видимости
//            if(track.at(j).target.count() != 0)
//            {
//                pen.setColor(Qt::darkBlue);
//                p.setPen(pen);
//                if(track.at(j).target.at(i).p1 != NULL)
//                    p.drawLine(track.at(j).pos, track.at(j).target.at(i).p1);
//                if(track.at(j).target.at(i).p2 != NULL)
//                    p.drawLine(track.at(j).pos, track.at(j).target.at(i).p2);
//            }
//        }
//    }

    p.end();
}

void Painter::timerEvent(QTimerEvent *)
{
    /// Трассы
    for(int j = 0; j < track.count(); ++j)
    {
        track[j].pos += QPoint(track.at(j).modV * qCos(M_PI_2 - track.at(j).angV) * DELTA + 1000 * normalDistribution(0, 0.2),
                               track.at(j).modV * qSin(M_PI_2 - track.at(j).angV) * DELTA + 1000 * normalDistribution(0, 0.2));
    }

    /// Определение расстояний от трасс до центров ПР
    /// и углов между векторами скорости и прямыми до центров ПР
    for(int j = 0; j < track.count(); ++j)
    {
        for(int i = 0; i < area.count(); ++i)
        {
            track[j].target.push_back(Track::Target());

            /// Определение расстояния от трассы до центра ПР
            track[j].target[i].dist = calcDistance(&track.at(j).pos, &area.at(i).pos);

            /// Определение угла между вектором скорости и прямой до центра ПР
            track[j].target[i].angToV = track.at(j).angV + qAtan2(area.at(i).pos.y() - track.at(j).pos.y(),
                                                                  area.at(i).pos.x() - track.at(j).pos.x()) - M_PI_2;
        }

        /// Определение ближайшего и наиболее удаленного ПР
        track[j].farTarget = &track[j].target[0];
        track[j].nearTarget = &track[j].target[0];
        for(int i = 1; i < area.count(); ++i)
        {
            /// Определение ближайшего ПР
            if(track.at(j).nearTarget->dist > track.at(j).target.at(i).dist)
                track[j].nearTarget = &track[j].target[i];

            /// Определение наиболее удаленного ПР
            if(track.at(j).farTarget->dist < track.at(j).target.at(i).dist)
                track[j].farTarget = &track[j].target[i];
        }
    }

    /// Расчет эталонного времени и времени с погрешностями
    for(int i = 0; i < area.count(); ++i)
    {
        for(int j = 0; j < track.count(); ++j)
        {
            /// Расчет эталонного времени
            track[j].target[i].time = (track.at(j).target.at(i).dist - area.at(i).radius) /
                                      (track.at(j).modV * qCos(track.at(j).target.at(i).angToV));

            /// Расчет времени с погрешностями
            track[j].target[i].errTime = (track.at(j).target.at(i).dist - area.at(i).radius) /
                                         (track.at(j).modV * qCos(track.at(j).target.at(i).angToV));
        }
    }

    results->loadTable(&area, &track);

    repaint();
}

void Painter::loadAreaPar()
{
    area.resize(areaParameters->getCount());

    for(int i = 0; i < area.count(); ++i)
    {
        area[i].num = i;
        area[i].pos.setX(areaParameters->getPar(i, 0));
        area[i].pos.setY(areaParameters->getPar(i, 1));
        area[i].radius = areaParameters->getPar(i, 2);
    }
}

void Painter::loadTrackPar()
{
    track.resize(trackParameters->getCount());

    for(int j = 0; j < track.count(); ++j)
    {
        track[j].num = j;
        track[j].pos.setX(trackParameters->getPar(j, 0));
        track[j].pos.setY(trackParameters->getPar(j, 1));
        track[j].modV = trackParameters->getPar(j, 2);
        track[j].angV = qDegreesToRadians(trackParameters->getPar(j, 3));
    }
}

float Painter::calcDistance(const QPointF *_area, const QPointF *_track)
{
    return qSqrt((_area->x() - _track->x()) * (_area->x() - _track->x()) +
                 (_area->y() - _track->y()) * (_area->y() - _track->y()));
}

float Painter::normalDistribution(float _mean, float _dev)
{
    if(_dev == 0.0)
        return _mean;

    float y = (float) qrand() / (RAND_MAX * _dev * qSqrt(2 * M_PI));
    float ln = qLn(y * _dev * qSqrt(2 * M_PI));
    return (qrand() % 2 == 0 ? 1.0 : -1.0) * qSqrt((ln < 0 ? -2.0 : 2.0) * _dev * _dev * ln) + _mean;
}

void Painter::calcTanPoints(const QPointF *_track, const QPointF *_area, const float _radius, QPointF *_p1, QPointF *_p2)
{
    float distance = calcDistance(_area, _track);
    if(distance < _radius)
    {
        _p1 = NULL;
        _p2 = NULL;

        return;
    }

    QPointF midDistTargetBase((*_track - *_area) / 2.0);

    float a = midDistTargetBase.x() * midDistTargetBase.x() +
            midDistTargetBase.y() * midDistTargetBase.y();
    float E = 0.5 * (_radius * _radius - 0.25 * distance * distance + a);

    if(qAbs(midDistTargetBase.x()) < 1.e-6)
    {
        _p1->setX(qSqrt(_radius * _radius - E * E / (midDistTargetBase.y() * midDistTargetBase.y())));
        _p1->setY(E / midDistTargetBase.y());

        _p2->setX(- qSqrt(_radius * _radius - E * E / (midDistTargetBase.y() * midDistTargetBase.y())));
        _p2->setY(E / midDistTargetBase.y());
    }
    else
    {
        float b = - E * midDistTargetBase.y();
        float c = E * E - (_radius * _radius * midDistTargetBase.x() * midDistTargetBase.x());
        float D = b * b - a * c;

        _p1->setY((- b + qSqrt(D)) / a);
        _p2->setY((- b - qSqrt(D)) / a);

        _p1->setX((E - _p1->y() * midDistTargetBase.y()) / midDistTargetBase.x());
        _p2->setX((E - _p2->y() * midDistTargetBase.y()) / midDistTargetBase.x());
    }

    *_p1 += *_area;
    *_p2 += *_area;
}

}
