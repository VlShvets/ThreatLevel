#include "painter.h"

#include <QDebug>

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
    loadEtalonPar();

    time = 0;
}

Painter::~Painter()
{
    area.clear();
    track.clear();
    etalon.clear();
}

void Painter::reStart()
{
    loadAreaPar();
    loadTrackPar();
    loadEtalonPar();

    time = 0;

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
        if(track.at(j).nFarTarget != -1)
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

void Painter::timerEvent(QTimerEvent *)
{
    ++time;

    /// Эталоны
    /// ==================================================
    for(int j = 0; j < etalon.count(); ++j)
    {
        /// Перемещение
        etalon[j].pos += QPointF(etalon.at(j).modV * qSin(etalon.at(j).angV),
                                 etalon.at(j).modV * qCos(etalon.at(j).angV)) * DELTAT;

        /// Определение расстояний от проекции эталона до центров ПР
        for(int i = 0; i < area.count(); ++i)
        {
            etalon[j].target[i].dist = etalon.at(j).target.at(i).startDist -
                                       calcDistance(&etalon.at(j).startPos, &etalon.at(j).pos) * qCos(etalon.at(j).target.at(i).angToV);

            /// Условие окончания
            if(etalon[j].target[i].dist < 0)
                return;
        }
    }

    /// Расчет эталонного времени
    for(int i = 0; i < area.count(); ++i)
    {
        for(int j = 0; j < etalon.count(); ++j)
            etalon[j].target[i].time = etalon.at(j).target.at(i).dist /
                                       (etalon.at(j).modV * qCos(etalon.at(j).target.at(i).angToV));
    }
    /// ==================================================

    /// Трассы
    /// ==================================================
    for(int j = 0; j < track.count(); ++j)
    {
        track[j].pos = etalon.at(j).pos;

        /// Внесение погрешностей
        track[j].modV = etalon.at(j).modV + normalDistribution(0, ERRMODV);
        track[j].angV = etalon.at(j).angV + qDegreesToRadians(normalDistribution(0, ERRANGV));

        /// Определение углов между вектором скорости и прямыми до центров ПР
        /// и расстояний от проекции трассы до центров ПР
        for(int i = 0; i < area.count(); ++i)
        {
            /// Определение угла между вектором скорости и прямой до центра ПР
            track[j].target[i].angToV = track.at(j).angV + qAtan2(area.at(i).pos.y() - track.at(j).startPos.y(),
                                                                  area.at(i).pos.x() - track.at(j).startPos.x()) - M_PI_2;

            /// Определение расстояния от проекции трассы до центра ПР
            track[j].target[i].dist = track.at(j).target.at(i).startDist -
                                      calcDistance(&track.at(j).startPos, &track.at(j).pos) * qCos(track.at(j).target.at(i).angToV);

            /// Определение точек касания угла видимости
//            calcTanPoints(&track.at(j).pos, &area.at(i).pos, area.at(i).radius, &track[j].target[i].p1, &track[j].target[i].p2);
        }

        /// Определение ближайшего и наиболее удаленного ПР
//        track[j].nNearTarget = 0;
        track[j].nFarTarget = 0;
        for(int i = 1; i < area.count(); ++i)
        {
            /// Определение ближайшего ПР
//            if(track.at(j).target.at(track.at(j).nNearTarget).dist > track.at(j).target.at(i).dist)
//                track[j].nNearTarget = i;

            /// Определение наиболее удаленного ПР
            if(track.at(j).target.at(track.at(j).nFarTarget).dist < track.at(j).target.at(i).dist)
                track[j].nFarTarget = i;
        }

        /// Определение координат экстраполированного конца траектории
        track[j].endPos = track.at(j).startPos +
                          QPointF((track.at(j).target.at(track.at(j).nFarTarget).startDist /
                                   qCos(track.at(j).target.at(track.at(j).nFarTarget).angToV)) * qSin(track.at(j).angV),
                                  (track.at(j).target.at(track.at(j).nFarTarget).startDist /
                                   qCos(track.at(j).target.at(track.at(j).nFarTarget).angToV)) * qCos(track.at(j).angV));
    }

    /// Расчет времени с погрешностями
    for(int i = 0; i < area.count(); ++i)
    {
        for(int j = 0; j < track.count(); ++j)
            track[j].target[i].time = track.at(j).target.at(i).dist /
                                      (track.at(j).modV * qCos(track.at(j).target.at(i).angToV));
    }
    /// ==================================================

    for(int i = 0; i < area.count(); ++i)
    {
        /// Определение наиболее опасной трассы
        area[i].nDangerousTrack = 0;
        for(int j = 1; j < track.count(); ++j)
        {
            if(track.at(area[i].nDangerousTrack).target.at(i).dist > track.at(j).target.at(i).dist)
                area[i].nDangerousTrack = j;
        }

        /// Вычисление погрешности времени поражения
        area[i].time = track.at(area.at(i).nDangerousTrack).target.at(i).time;
        area[i].errTime = track.at(area.at(i).nDangerousTrack).target.at(i).time -
                          etalon.at(area.at(i).nDangerousTrack).target.at(i).time;

        /// Вычисление среднеквадратической погрешности времени поражения
        area[i].sumErrTime += area[i].errTime * area[i].errTime;
        area[i].sigmaT = qSqrt(area[i].sumErrTime / time);
    }

    results->loadTable(&area, &track, &etalon);

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

        /// Обнуление суммы квадратов погрешностей времени поражения
        area[i].sumErrTime = 0.0;
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

        /// Присвоение начальных координат текущим
        track[j].pos = track.at(j).startPos;

        /// Вычисление неизменяющихся параметров
        for(int i = 0; i < area.count(); ++i)
        {
            track[j].target.push_back(Track::Target());

            /// Определение расстояния от начальной точки траектории до центра ПР
            track[j].target[i].startDist = calcDistance(&track.at(j).startPos, &area.at(i).pos);
        }

        /// Очистка номеров ближайшего и наиболее удаленного ПР
//        track[j].nNearTarget = -1;
        track[j].nFarTarget = -1;
    }
}

void Painter::loadEtalonPar()
{
    etalon.resize(trackParameters->getCount());

    for(int j = 0; j < etalon.count(); ++j)
    {
        etalon[j].num = j;

        /// Загрузка исходных данных из таблицы
        etalon[j].startPos.setX(trackParameters->getPar(j, 0));
        etalon[j].startPos.setY(trackParameters->getPar(j, 1));
        etalon[j].modV = trackParameters->getPar(j, 2);
        etalon[j].angV = qDegreesToRadians(trackParameters->getPar(j, 3));

        /// Присвоение текущим координатам начальных координат траектории
        etalon[j].pos = etalon.at(j).startPos;

        /// Вычисление неизменяющихся параметров
        for(int i = 0; i < area.count(); ++i)
        {
            etalon[j].target.push_back(Track::Target());

            /// Определение расстояния от начальной точки траектории до центра ПР
            etalon[j].target[i].startDist = calcDistance(&etalon.at(j).startPos, &area.at(i).pos);

            /// Определение угла между вектором скорости и прямой от начальной точки до центра ПР
            etalon[j].target[i].angToV = etalon.at(j).angV + qAtan2(area.at(i).pos.y() - etalon.at(j).startPos.y(),
                                                                    area.at(i).pos.x() - etalon.at(j).startPos.x()) - M_PI_2;
        }
    }
}

float Painter::calcDistance(const QPointF *_p1, const QPointF *_p2)
{
    return qSqrt((_p1->x() - _p2->x()) * (_p1->x() - _p2->x()) +
                 (_p1->y() - _p2->y()) * (_p1->y() - _p2->y()));
}

float Painter::normalDistribution(float _mean, float _dev)
{
    if(_dev == 0.0)
        return _mean;

    float y = (float) qrand() / (RAND_MAX * _dev * qSqrt(2 * M_PI));
    float ln = qLn(y * _dev * qSqrt(2 * M_PI));
    float answer = qSqrt((ln < 0 ? -2.0 : 2.0) * _dev * _dev * ln);
    if(answer > 1.5 * _dev)
        return normalDistribution(_mean, _dev);

    return (qrand() % 2 == 0 ? 1.0 : -1.0) * answer + _mean;
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
