#include "painter.h"

#include <QDebug>

namespace ThreatLevel
{

Painter::Painter(Results *_results, QWidget *_parent)
    : Grapher2D(_parent), results(_results)
{
    setCSAbsMeasure(10000);
    setCSOrdMeasure(10000);
    setCSZoom(2);

    nArea = 3;
    nTrack = 3;

    speedFactor = 1.0;
    totalTime = 60.0;

    initAreaPar();
    initTrackPar();
}

Painter::~Painter()
{
    area.clear();
    track.clear();
}

void Painter::initAreaPar()
{
    area.push_back(Area());         area.push_back(Area());         area.push_back(Area());         area.push_back(Area());         area.push_back(Area());
    area[0].pos.setX(0.0);          area[1].pos.setX(-100000.0);    area[2].pos.setX(100000.0);     area[3].pos.setX(0.0);          area[4].pos.setX(0.0);
    area[0].pos.setY(100000.0);     area[1].pos.setY(0.0);          area[2].pos.setY(0.0);          area[3].pos.setY(-100000.0);    area[4].pos.setY(0.0);
    area[0].radius = 50000.0;       area[1].radius = 50000.0;       area[2].radius = 50000.0;       area[3].radius = 50000.0;       area[4].radius = 50000.0;
}

void Painter::initTrackPar()
{
    for(int j = 0; j < NUMTRACKINGROUP; ++j)
    {
        /// Группа №1
        track.push_back(Track());
        track.last().pos.setX(0.0 + 6000 * (int) (j / qSqrt(NUMTRACKINGROUP)));
        track.last().pos.setY(-500000.0 - 6000 * (j % (int) qSqrt(NUMTRACKINGROUP)));
        track.last().modV = 3000.0;
        track.last().angV = 0;

        /// Группа №2
        track.push_back(Track());
        track.last().pos.setX(-500000.0 - 6000 * (int) (j / qSqrt(NUMTRACKINGROUP)));
        track.last().pos.setY(500000.0 + 6000 * (j % (int) qSqrt(NUMTRACKINGROUP)));
        track.last().modV = 3000.0;
        track.last().angV = 3 * M_PI_4;

        /// Группа №3
        track.push_back(Track());
        track.last().pos.setX(500000.0 + 6000 * (int) (j / qSqrt(NUMTRACKINGROUP)));
        track.last().pos.setY(500000.0 + 6000 * (j % (int) qSqrt(NUMTRACKINGROUP)));
        track.last().modV = 3000.0;
        track.last().angV = 5 * M_PI_4;
    }
}

void Painter::timerEvent(QTimerEvent *)
{
    /// Время
    if(time < totalTime)
        time += 0.1 * speedFactor;
    else
        time = 0;

    repaint();
}

void Painter::resetTime()
{
    time = 0.0;
    repaint();
}

//void Painter::timerEvent(QTimerEvent *_tEvent)
//{
//    /// Определение угла видимости базы
//    track.angleBase = qAsin(base.radius / track.distBase);

//    /// Условие окончания
//    if(qAbs(base.coord.x() - track.coord.x()) < base.radius ||
//       qAbs(base.coord.y() - track.coord.x()) < base.radius ||
//       ++time == TOTALTIME)
//        killTimer(_tEvent->timerId());
//}

float Painter::calcTanPoints(const QPointF *_track, const QPointF *_base, const float _radius, QPointF *_p1, QPointF *_p2)
{
    float distance = qSqrt((_base->x() - _track->x()) * (_base->x() - _track->x()) +
                           (_base->y() - _track->y()) * (_base->y() - _track->y()));
    if(distance < _radius)
    {
        _p1 = new QPointF();
        _p2 = new QPointF();
    }
    else
    {
        QPointF midDistTargetBase((*_track - *_base) / 2.0);

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

        *_p1 += *_base;
        *_p2 += *_base;
    }

    return distance;
}

float Painter::normalDistribution(float _mean, float _dev)
{
    if(_dev == 0.0)
        return _mean;

    float y = (float) qrand() / (RAND_MAX * _dev * qSqrt(2 * M_PI));
    float ln = qLn(y * _dev * qSqrt(2 * M_PI));
    return (qrand() % 2 == 0 ? 1.0 : -1.0) * qSqrt((ln < 0 ? -2.0 : 2.0) * _dev * _dev * ln) + _mean;
}

void Painter::paintEvent(QPaintEvent * _pEvent)
{
    Grapher2D::paintEvent(_pEvent);

    QPainter p(this);
    p.translate(getCSAbsTranslate(), getCSOrdTranslate());
    p.scale(getCSAbsScale(), getCSOrdScale());

    QPen pen;
    pen.setCosmetic(true);

    /// Базы
    pen.setColor(Qt::darkBlue);
    pen.setWidth(3);
    p.setPen(pen);
    for(int i = 0; i < nArea; ++i)
        p.drawEllipse(area.at(i).pos, area.at(i).radius, area.at(i).radius);

    /// Трассы
    QVector <QPointF> pTrack;
    pen.setColor(Qt::darkGreen);
    pen.setWidth(6);
    p.setPen(pen);
    for(int j = 0; j < nTrack; ++j)
    {
        pTrack.push_back(track.at(j).pos + QPoint(track.at(j).modV * qCos(M_PI_2 - track.at(j).angV) * time + 1000 * normalDistribution(0, 0.2),
                                                  track.at(j).modV * qSin(M_PI_2 - track.at(j).angV) * time + 1000 * normalDistribution(0, 0.2)));
        p.drawPoint(pTrack.at(j));
    }

    /// Определение расстояния до базы и угла видимости
    pen.setWidth(1);
    for(int j = 0; j < nTrack; ++j)
    {
        float maxDist = 0.0;        /// Максимальное расстояние до центра позиционного района
        float angToMaxDist = 0.0;   /// Угол между вектором скорости и прямой до центра наиболее удаленного района

        for(int i = 0; i < nArea; ++i)
        {
            track[j].target.push_back(Track::Target());
            track[j].target[i].dist = calcTanPoints(&pTrack.at(j), &area.at(i).pos, area.at(i).radius,
                                                    &track[j].target[i].p1, &track[j].target[i].p2);
            track[j].target[i].angToV = track.at(j).angV + qAtan2(area.at(i).pos.y() - pTrack.at(j).y(),
                                                                  area.at(i).pos.x() - pTrack.at(j).x()) - M_PI_2;

            if(maxDist < track.at(j).target.at(i).dist)
            {
                maxDist = track.at(j).target.at(i).dist;
                angToMaxDist = track.at(j).target.at(i).angToV;
            }

            /// Угол видимости
//            pen.setColor(Qt::darkBlue);
//            p.setPen(pen);
//            p.drawLine(pTrack.at(j), track.at(j).target.at(i).p1);
//            p.drawLine(pTrack.at(j), track.at(j).target.at(i).p2);

            /// Кратчайшее растояние
//            pen.setColor(Qt::darkRed);
//            p.setPen(pen);
//            p.drawLine(area.at(i).pos, pTrack.at(j));
        }

        /// Вектор скорости
        pen.setColor(Qt::darkGreen);
        p.setPen(pen);
        p.drawLine(pTrack.at(j), pTrack.at(j) + QPointF(maxDist * qSin(track.at(j).angV) / qCos(angToMaxDist),
                                                        maxDist * qCos(track.at(j).angV) / qCos(angToMaxDist)));
    }

    /// Расчет времени с ошибками
    QVector <QVector <float> > timesWithError;
    for(int i = 0; i < nArea; ++i)
    {
        timesWithError.push_back(QVector <float>());
        for(int j = 0; j < nTrack; ++j)
            timesWithError[i].push_back((track.at(j).target.at(i).dist - area.at(i).radius) /
                                        (track.at(j).modV * qCos(track.at(j).target.at(i).angToV)));
    }

    /// Расчет эталонного времени
    QVector <QVector <float> > timesNoError;
    for(int i = 0; i < nArea; ++i)
    {
        timesNoError.push_back(QVector <float>());
        for(int j = 0; j < nTrack; ++j)
            timesNoError[i].push_back((track.at(j).target.at(i).dist - area.at(i).radius) /
                                      (track.at(j).modV * qCos(track.at(j).target.at(i).angToV)));
    }

    results->loadTable(&timesWithError, nArea, nTrack);

    pTrack.clear();

    p.end();
}

}
