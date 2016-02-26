#include "painter.h"

#include <QDebug>

Painter::Painter(QWidget *_parent)
    : Grapher2D(_parent)
{
    /// Априорная инициализация параметров базы
    base.coord = QPointF(BASECOORDX, BASECOORDY);
    base.radius = BASERADIUS;

    /// Априорная инициализация параметров цели
    track.coord = QPointF(TRACKCOORDX, TRACKCOORDY);
    getTangentPoints(&track.coord, &base.coord, BASERADIUS, track.tangentPoints);
    track.moduleV = TRACKMODULEV;
    track.vectorV = TRACKVECTORV;

    startTimer(1000);   /// 1.000 с
}

Painter::~Painter()
{
}

void Painter::timerEvent(QTimerEvent *_tEvent)
{
    static int time = 0;

    /// Движение цели
    track.coord.rx() += track.moduleV * qCos(M_PI_2 - track.vectorV);
    track.coord.ry() -= track.moduleV * qSin(M_PI_2 - track.vectorV);
//    qDebug() << "target.coord = " << target.coord;

    /// Определение расстояния до базы и точек касания
    track.distBase = getTangentPoints(&track.coord, &base.coord, base.radius, track.tangentPoints);
//    qDebug() << "target.distBase = " << target.distBase;

    /// Определение угла видимости базы
    track.angleBase = qAsin(base.radius / track.distBase);
//    qDebug() << "target.angleBase = " << qRadiansToDegrees(target.angleBase);

    repaint();

    if(qAbs(base.coord.x() - track.coord.x()) < base.radius ||
       qAbs(base.coord.y() - track.coord.x()) < base.radius ||
       ++time == TOTALTIME)
        killTimer(_tEvent->timerId());
}

void Painter::paintEvent(QPaintEvent * _pEvent)
{
    Grapher2D::paintEvent(_pEvent);

    QPainter p(this);
    p.translate(getCSAbsTranslate(), getCSOrdTranslate());
    p.scale(getCSAbsScale(), getCSOrdScale());

    QPen pen;
    pen.setCosmetic(true);

    /// База

    pen.setWidth(2);
    p.setPen(pen);
    p.drawEllipse(base.coord, base.radius, base.radius);

    /// Цель

    pen.setColor(Qt::red);
    pen.setWidth(10);
    p.setPen(pen);
    p.drawPoint(track.coord);

    /// Кратчайшее расстояние

    pen.setColor(Qt::blue);
    pen.setWidth(2);
    p.setPen(pen);
    p.drawLine(track.coord, base.coord);

    /// Угол видимости

    pen.setColor(Qt::darkBlue);
    pen.setWidth(3);
    p.setPen(pen);
    p.drawLine(track.coord, track.tangentPoints[0]);
    p.drawLine(track.coord, track.tangentPoints[1]);

    p.end();
}

float Painter::getTangentPoints(QPointF *_track, QPointF *_base, float _radius, QPointF *_tangentPoints)
{
    float distance = qSqrt((_base->x() - _track->x()) * (_base->x() - _track->x()) +
                           (_base->y() - _track->y()) * (_base->y() - _track->y()));
    QPointF midDistTargetBase((*_track - *_base) / 2.0);

    float a = midDistTargetBase.x() * midDistTargetBase.x() +
            midDistTargetBase.y() * midDistTargetBase.y();
    float E = 0.5 * (_radius * _radius - 0.25 * distance * distance + a);

    if(qAbs(midDistTargetBase.x()) < 1.e-6)
    {
        _tangentPoints[0] = QPointF(qSqrt(_radius * _radius -
                                          E * E / (midDistTargetBase.y() * midDistTargetBase.y())),
                                    E / midDistTargetBase.y());
        _tangentPoints[1] = QPointF(- qSqrt(_radius * _radius -
                                            E * E / (midDistTargetBase.y() * midDistTargetBase.y())),
                                    E / midDistTargetBase.y());
    }
    else
    {
        float b = - E * midDistTargetBase.y();
        float c = E * E - (_radius * _radius * midDistTargetBase.x() * midDistTargetBase.x());
        float D = b * b - a * c;

        _tangentPoints[0].setY((- b + qSqrt(D)) / a);
        _tangentPoints[1].setY((- b - qSqrt(D)) / a);

        _tangentPoints[0].setX((E - _tangentPoints[0].y() * midDistTargetBase.y()) / midDistTargetBase.x());
        _tangentPoints[1].setX((E - _tangentPoints[1].y() * midDistTargetBase.y()) / midDistTargetBase.x());
    }

    _tangentPoints[0] += *_base;
    _tangentPoints[1] += *_base;

    return distance;
}
