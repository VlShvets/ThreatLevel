#include "painter.h"

Painter::Painter(QWidget *_parent)
    : Grapher2D(_parent)
{
    setCSAbsMeasure(1000);
    setCSOrdMeasure(1000);
    setCSZoom(2);

    this->setWindowTitle(QObject::tr("Определение уровня угроз"));

    nBase = 3;
    nTrack = 3;

    sizeOfMemory = 100;
    totalTime = 60.0;

    tTime = new QTimer(this);
    QObject::connect(tTime, SIGNAL(timeout()), this, SLOT(timerOut()));

    initializationParOfBase();
    initializationParOfTrack();
}

Painter::~Painter()
{
    delete tTime;
    base.clear();
    track.clear();
}

void Painter::initializationParOfBase()
{
    base.push_back(Base());         base.push_back(Base());         base.push_back(Base());         base.push_back(Base());         base.push_back(Base());
    base[0].pos.setX(0.0);          base[1].pos.setX(-50000.0);     base[2].pos.setX(50000.0);      base[3].pos.setX(0.0);          base[4].pos.setX(0.0);
    base[0].pos.setY(50000.0);      base[1].pos.setY(0.0);          base[2].pos.setY(0.0);          base[3].pos.setY(-50000.0);     base[4].pos.setY(0.0);
    base[0].radius = 10000.0;       base[1].radius = 10000.0;       base[2].radius = 10000.0;       base[3].radius = 10000.0;       base[4].radius = 10000.0;
}

void Painter::initializationParOfTrack()
{
    for(int j = 0; j < 9; ++j)
    {
        track.push_back(Track());
        track[j].pos.setX(-100000.0 + 20000 * (int) (j / 3));
        track[j].pos.setY(50000.0 + 20000 * (j % 3));
        track[j].modV = 3000.0;
        track[j].angV = 90.0;
    }
}

void Painter::timerOut()
{
    /// Время
    if(time < totalTime)
        time += 0.1;
    else
        time = 0;

    repaint();
}

//void Painter::timerEvent(QTimerEvent *_tEvent)
//{
//    /// Определение угла видимости базы
//    track.angleBase = qAsin(base.radius / track.distBase);
////    qDebug() << "target.angleBase = " << qRadiansToDegrees(target.angleBase);

//    repaint();

//    if(qAbs(base.coord.x() - track.coord.x()) < base.radius ||
//       qAbs(base.coord.y() - track.coord.x()) < base.radius ||
//       ++time == TOTALTIME)
//        killTimer(_tEvent->timerId());
//}

float Painter::getTanPoints(const QPointF *_track, const QPointF *_base, const float _radius, QPointF *_p1, QPointF *_p2)
{
    float distance = qSqrt((_base->x() - _track->x()) * (_base->x() - _track->x()) +
                           (_base->y() - _track->y()) * (_base->y() - _track->y()));
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

    _p1[0] += *_base;
    _p2[1] += *_base;

    return distance;
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
    for(int i = 0; i < nBase; ++i)
        p.drawEllipse(base.at(i).pos, base.at(i).radius, base.at(i).radius);

    /// Трассы
    QVector <QPointF> pTrack;
    pen.setColor(Qt::darkGreen);
    pen.setWidth(6);
    p.setPen(pen);
    for(int j = 0; j < nTrack; ++j)
    {
        pTrack.push_back(track.at(j).pos + QPoint(track.at(j).modV * qCos(M_PI_2 - (2 * M_PI / 360) * track.at(j).angV) * time,
                                                  track.at(j).modV * qSin(M_PI_2 - (2 * M_PI / 360) * track.at(j).angV) * time));
        p.drawPoint(pTrack.at(j));
    }

    /// Определение расстояния до базы и угла видимости
    pen.setWidth(1);
    for(int j = 0; j < nTrack; ++j)
    {
        for(int i = 0; i < nBase; ++i)
        {
            track[j].target.push_back(Track::Target());
            track[j].target[i].dist = getTanPoints(&track.at(j).pos, &base.at(i).pos, base.at(i).radius,
                                                      &track[j].target[i].p1, &track[j].target[i].p2);

            /// Кратчайшее растояние
            pen.setColor(Qt::darkBlue);
            p.setPen(pen);
            p.drawLine(base.at(i).pos, pTrack.at(j));

//            /// Угол видимости
//            pen.setColor(Qt::darkGreen);
//            p.setPen(pen);
//            p.drawLine(track.at(j).pos, track.at(j).target.at(i).p1);
//            p.drawLine(track.at(j).pos, track.at(j).target.at(i).p2);
        }
    }

    pTrack.clear();

    p.end();
}
