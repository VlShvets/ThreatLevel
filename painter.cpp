#include "painter.h"

namespace ThreatLevel
{

Painter::Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent)
    : Grapher2D(_parent), areaParameters(_areaParameters), trackParameters(_trackParameters), results(_results)
{
    setCSAbsMeasure(10000);
    setCSOrdMeasure(10000);
    setCSZoom(10);

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
    {
        /// Отрисовка местоположения
        p.drawEllipse(area.at(i).pos, area.at(i).radius, area.at(i).radius);

        /// Отображение номера
        p.save();
        p.resetTransform();
        p.translate(width() / 2.0 + getCSAbsShift(), height() / 2.0 + getCSOrdShift());
        p.drawText(area.at(i).pos.x() * getCSAbsScale() - 20, area.at(i).pos.y() * getCSOrdScale() - 5, QString::number(i + 1));
        p.restore();
    }

    /// Трассы
    pen.setColor(Qt::darkGreen);
    for(int j = 0; j < track.count(); ++j)
    {
        /// Отрисовка местоположения
        pen.setWidth(6);
        p.setPen(pen);
        p.drawPoint(track.at(j).pos);

        /// Отображение номера
        p.save();
        p.resetTransform();
        p.translate(width() / 2.0 + getCSAbsShift(), height() / 2.0 + getCSOrdShift());
        p.drawText(track.at(j).pos.x() * getCSAbsScale() - 20, track.at(j).pos.y() * getCSOrdScale() - 5, QString::number(j + 1));
        p.restore();

        /// Отрисовка курса
        if(track.at(j).nNearTarget != -1)
        {
            pen.setWidth(1);
            p.setPen(pen);
            p.drawLine(track.at(j).startPos, track[j].endPos);;
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
//                if(!track.at(j).target.at(i).p1.isNull())
//                    p.drawLine(track.at(j).pos, track.at(j).target.at(i).p1);
//                if(!track.at(j).target.at(i).p2.isNull())
//                    p.drawLine(track.at(j).pos, track.at(j).target.at(i).p2);
//            }
//        }
//    }

    p.end();
}

void Painter::timerEvent(QTimerEvent *_tEvent)
{
    /// Точные значения
    /// ==================================================
    for(int j = 0; j < track.count(); ++j)
    {        
        ++track[j].countP;

        /// Перемещение
        track[j].pos += QPointF(track.at(j).modV * qSin(track.at(j).angV),
                                track.at(j).modV * qCos(track.at(j).angV)) * DELTAT;

        /// Определение расстояний от проекции трассы до центров ПР
        for(int i = 0; i < area.count(); ++i)
        {
            track[j].target[i].dist = track.at(j).target.at(i).startDist -
                                      calcDistance(&track.at(j).startPos, &track.at(j).pos) * qCos(track.at(j).target.at(i).angToV);

            /// Условие окончания
            if(track.at(j).target.at(i).dist < 0)
            {
                killTimer(_tEvent->timerId());
                idTimer = -1;
            }
        }
    }

    /// Расчет времени поражения ПР
    for(int i = 0; i < area.count(); ++i)
    {
        for(int j = 0; j < track.count(); ++j)
            track[j].target[i].time = track.at(j).target.at(i).dist /
                                      (track.at(j).modV * qCos(track.at(j).target.at(i).angToV));
    }
    /// ==================================================

    /// Значения с погрешностями
    /// ==================================================
    for(int j = 0; j < track.count(); ++j)
    {
        /// Внесение погрешностей
        track[j].errModV = track.at(j).modV + gaussDistribution(0, ERRMODV);
        track[j].errAngV = track.at(j).angV + qDegreesToRadians(uniformDistribution(0, ERRANGV));

        /// Сглаживание погрешностей через проекции вектора скорости
        if(track.at(j).errVx == 0.0)
            track[j].errVx = track.at(j).errModV * qSin(track.at(j).errAngV);
        else
            track[j].errVx = track.at(j).errVx                                  * WEIGHT +
                            (track.at(j).errModV * qSin(track.at(j).errAngV))   * (1.0 - WEIGHT);

        if(track.at(j).errVy == 0.0)
            track[j].errVy = track.at(j).errModV * qCos(track.at(j).errAngV);
        else
            track[j].errVy = track.at(j).errVy                                  * WEIGHT +
                            (track.at(j).errModV * qCos(track.at(j).errAngV))   * (1.0 - WEIGHT);

        /// Возврат к модулю скорости и курсу
        track[j].errModV = qSqrt(track.at(j).errVx * track.at(j).errVx + track.at(j).errVy * track.at(j).errVy);
        track[j].errAngV = qAtan2(track.at(j).errVx, track.at(j).errVy);

        /// Определение углов между вектором скорости и прямыми до центров ПР
        /// и расстояний от проекции трассы до центров ПР
        for(int i = 0; i < area.count(); ++i)
        {
            /// Определение угла между вектором скорости и прямой до центра ПР
            track[j].target[i].errAngToV = track.at(j).errAngV - qAtan2(area.at(i).pos.x() - track.at(j).startPos.x(),
                                                                        area.at(i).pos.y() - track.at(j).startPos.y());

            /// Определение расстояния от проекции трассы до центра ПР
            track[j].target[i].errDist = track.at(j).target.at(i).startDist -
                                         calcDistance(&track.at(j).startPos, &track.at(j).pos) * qCos(track.at(j).target.at(i).errAngToV);

            /// Определение точек касания угла видимости
//            calcTanPoints(&track.at(j).pos, &area.at(i).pos, area.at(i).radius, &track[j].target[i].p1, &track[j].target[i].p2);
        }

        /// Определение ближайшего ПР
        track[j].nNearTarget = 0;
        for(int i = 1; i < area.count(); ++i)
        {
            if(track.at(j).target.at(track.at(j).nNearTarget).dist > track.at(j).target.at(i).dist)
                track[j].nNearTarget = i;
        }

        /// Определение координат экстраполированного конца траектории
        track[j].endPos = track.at(j).startPos +
                          QPointF((track.at(j).target.at(track.at(j).nNearTarget).startDist /
                                   qCos(track.at(j).target.at(track.at(j).nNearTarget).errAngToV)) * qSin(track.at(j).errAngV),
                                  (track.at(j).target.at(track.at(j).nNearTarget).startDist /
                                   qCos(track.at(j).target.at(track.at(j).nNearTarget).errAngToV)) * qCos(track.at(j).errAngV));
    }

    /// Расчет времени поражения ПР
    for(int i = 0; i < area.count(); ++i)
    {
        for(int j = 0; j < track.count(); ++j)
            track[j].target[i].errTime = track.at(j).target.at(i).errDist /
                                         (track.at(j).errModV * qCos(track.at(j).target.at(i).errAngToV));
    }
    /// ==================================================

    for(int i = 0; i < area.count(); ++i)
    {
        /// Определение наиболее опасной трассы
        area[i].nDangerTrack = 0;
        for(int j = 1; j < track.count(); ++j)
        {
            if(track.at(area[i].nDangerTrack).target.at(i).dist > track.at(j).target.at(i).dist)
                area[i].nDangerTrack = j;
        }

        /// Вычисление погрешности времени поражения
        area[i].diffTime = track.at(area.at(i).nDangerTrack).target.at(i).errTime -
                           track.at(area.at(i).nDangerTrack).target.at(i).time;

        /// Вычисление среднеквадратической погрешности времени поражения
        area[i].sumDiffTime += area[i].diffTime * area[i].diffTime;
        area[i].sigmaT = qSqrt(area[i].sumDiffTime / track.at(area.at(i).nDangerTrack).countP);
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

        /// Загрузка исходных данных из таблицы
        area[i].pos.setX(areaParameters->getPar(i, 0));
        area[i].pos.setY(areaParameters->getPar(i, 1));
        area[i].radius = areaParameters->getPar(i, 2);
        area[i].critTime = areaParameters->getPar(i, 3);

        /// Обнуление рекурентно использующихся параметров
        area[i].sumDiffTime = 0.0;
    }
}

void Painter::loadTrackPar()
{
    track.resize(trackParameters->getCount());

    for(int j = 0; j < track.count(); ++j)
    {
        track[j].num = j;

        /// Загрузка исходных данных из таблицы
        track[j].startPos.setX(trackParameters->getPar(j, 0));
        track[j].startPos.setY(trackParameters->getPar(j, 1));
        track[j].modV = trackParameters->getPar(j, 2);
        track[j].angV = qDegreesToRadians(trackParameters->getPar(j, 3));

        /// Присвоение текущим координатам начальных координат трассы
        track[j].pos = track.at(j).startPos;

        /// Обнуление рекурентно использующихся параметров
        track[j].errVx = 0.0;
        track[j].errVy = 0.0;
        track[j].countP = 0;

        /// Очистка номера ближайшего ПР
        track[j].nNearTarget = -1;

        /// Вычисление неизменяющихся параметров
        for(int i = 0; i < area.count(); ++i)
        {
            track[j].target.push_back(Track::Target());

            /// Определение расстояния от начальных координат трассы до центра ПР
            track[j].target[i].startDist = calcDistance(&track.at(j).startPos, &area.at(i).pos);

            /// Определение угла между вектором скорости и прямой от начальной точки до центра ПР
            track[j].target[i].angToV = track.at(j).angV - qAtan2(area.at(i).pos.x() - track.at(j).startPos.x(),
                                                                  area.at(i).pos.y() - track.at(j).startPos.y());

            /// Обнуление рекурентно использующихся параметров
            track[j].target[i].errAngToV = 0.0;
        }
    }
}

float Painter::calcDistance(const QPointF *_p1, const QPointF *_p2)
{
    return qSqrt((_p1->x() - _p2->x()) * (_p1->x() - _p2->x()) +
                 (_p1->y() - _p2->y()) * (_p1->y() - _p2->y()));
}

float Painter::gaussDistribution(float _mean, float _dev)
{
    static bool ready = false;
    static float second = 0.0;

    if(ready)
    {
        ready = false;
        return second * _dev + _mean;
    }

    float u, v, s;
    do
    {
        u = 2.0 * ((float) qrand() / RAND_MAX) - 1.0;
        v = 2.0 * ((float) qrand() / RAND_MAX) - 1.0;
        s = u * u + v * v;
    }
    while(s > 1.0 || s == 0.0);

    float r = qSqrt(-2.0 * qLn(s) / s);
    second = r * u;

    if(qAbs(second) < 1.5)
        ready = true;

    if(qAbs(r * v) < 1.5)
        return r * v * _dev + _mean;

    return gaussDistribution(_mean, _dev);
}

float Painter::uniformDistribution(float _mean, float _dev)
{
    return 2 * _dev * ((float) qrand() / RAND_MAX) - _dev + _mean;
}

void Painter::calcTanPoints(const QPointF *_track, const QPointF *_area, const float _radius, QPointF *_p1, QPointF *_p2)
{
    float distance = calcDistance(_area, _track) - _radius;
    if(distance < 0)
    {
        _p1 = NULL;
        _p2 = NULL;

        return;
    }

    QPointF midDistTargetBase((*_track - *_area) / 2.0);

    float a = midDistTargetBase.x() * midDistTargetBase.x() +
            midDistTargetBase.y() * midDistTargetBase.y();
    float E = 0.5 * (_radius * _radius - 0.25 * (distance + _radius) * (distance + _radius) + a);

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
