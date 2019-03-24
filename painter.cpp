#include "painter.h"

namespace ThreatLevel
{

/// Класс виджета отрисовки трасс и позиционных районов
Painter::Painter(QWidget *_parent)
    : Grapher2D(_parent), areas(NULL), etalons(NULL), tracks(NULL),
      isVisibleOfAreas(true), isVisibleOfEtalons(true), isVisibleOfTracks(true), isVisibleOfMinDists(false)
{
    setCSAbsMeasure(ABS_MEASURE);
    setCSOrdMeasure(ORD_MEASURE);
    setCSZoom(DEF_ZOOM);

    startTimer(PAINTER_INTERVAL);
}

Painter::~Painter()
{
    clearing();
}

/// Очищение отрисовки
void Painter::clearing()
{
    areas       = NULL;
    etalons     = NULL;
    tracks      = NULL;
}

/// Обновление отрисокви
void Painter::timerEvent(QTimerEvent *)
{
    repaint();
}

/// Отрисовка ЗКВ и трасс
void Painter::paintEvent(QPaintEvent * _pEvent)
{
    Grapher2D::paintEvent(_pEvent);

    QPainter p(this);
    p.translate(getCSAbsTranslate(), getCSOrdTranslate());
    p.scale(getCSAbsScale(), getCSOrdScale());

    QPen pen;
    pen.setCosmetic(true);

    /// Позиционные районы
    if(areas && isVisibleOfAreas)
    {
        QMap <int, Area>::const_iterator area = areas->constBegin();
        for(; area != areas->end(); ++area)
        {
            /// Отрисовка местоположения
            pen.setStyle(Qt::SolidLine);
            pen.setColor(Qt::darkBlue);
            pen.setWidth(3);
            p.setPen(pen);
            p.drawEllipse(area.value().initPos, area.value().initRadius,
                          area.value().initRadius);

            /// Подстветка при наличии ассоциированных трасс
            if(!area.value().numTrack.isEmpty())
            {
                p.save();
                p.setBrush(QBrush(QColor(255, 0, 0, 50)));
                p.drawEllipse(area.value().initPos, area.value().initRadius,
                              area.value().initRadius);
                p.restore();
            }

            /// Отрисовка критического времени при минимальном значении скорости
//            pen.setStyle(Qt::DashLine);
//            pen.setColor(Qt::darkRed);
//            pen.setWidth(1);
//            p.setPen(pen);
//            p.drawEllipse(area.value().initPos, area.value().initCritTime * Track::MIN_SPEED,
//                          area.value().initCritTime * Track::MIN_SPEED);

            /// Отрисовка критического времени при максимальном значении скорости
//            pen.setStyle(Qt::DashLine);
//            pen.setColor(Qt::darkGreen);
//            pen.setWidth(1);
//            p.setPen(pen);
//            p.drawEllipse(area.value().initPos, area.value().initCritTime * Track::MAX_SPEED,
//                          area.value().initCritTime * Track::MAX_SPEED);

            /// Отрисовка района обнаружения
            pen.setStyle(Qt::DashLine);
            pen.setColor(Qt::darkBlue);
            pen.setWidth(1);
            p.setPen(pen);
            p.drawEllipse(area.value().initPos, area.value().initDetectRange,
                          area.value().initDetectRange);

            /// Отображение номера
            p.save();
            p.resetTransform();
            p.translate(width() / 2.0 + getCSAbsShift(), height() / 2.0 + getCSOrdShift());
            p.drawText(area.value().initPos.x() * getCSAbsScale() - 20,
                       area.value().initPos.y() * getCSOrdScale() - 5, QString::number(area.key()));
            p.restore();
        }
    }

    /// Эталоны
    if(etalons && isVisibleOfEtalons)
    {
        pen.setStyle(Qt::SolidLine);
        QMap <int, Etalon>::const_iterator etalon = etalons->constBegin();
        for(; etalon != etalons->end(); ++etalon)
        {
            if(etalon.value().initIsBG)
                pen.setColor(Qt::darkGray);
            else
                pen.setColor(Qt::lightGray);

            /// Отрисовка местоположения
            pen.setWidth(6);
            p.setPen(pen);
            p.drawPoint(etalon.value().exactPos);

            /// Отрисовка курса
            pen.setWidth(1);
            p.setPen(pen);
            p.drawLine(etalon.value().exactPos, etalon.value().exactPos +
                       LENGTH_COURSE * QPointF(qSin(etalon.value().measCourse), qCos(etalon.value().measCourse)));

            /// Отображение номера
            p.save();
            p.resetTransform();
            p.translate(width() / 2.0 + getCSAbsShift(), height() / 2.0 + getCSOrdShift());
            QString quant = (etalon.value().initIsBG)?(""):(", " + QString::number(etalon.value().initQuant));
            p.drawText(etalon.value().exactPos.x() * getCSAbsScale() - 20,
                       etalon.value().exactPos.y() * getCSOrdScale() - 5, QString::number(etalon.key()) + quant);

            p.restore();
        }
    }

    /// Трассы
    if(tracks && isVisibleOfTracks)
    {
        pen.setStyle(Qt::SolidLine);
        QMap <int, Track>::const_iterator track = tracks->constBegin();
        for(; track != tracks->end(); ++track)
        {
            if(track.value().initIsBG)
                pen.setColor(Qt::darkMagenta);
            else
                pen.setColor(Qt::darkCyan);

            /// Отрисовка местоположения
            pen.setWidth(6);
            p.setPen(pen);
            p.drawPoint(track.value().exactPos);

            /// Отрисовка курса
            pen.setWidth(1);
            p.setPen(pen);
            if(!track.value().startPos.isNull() &&
               !track.value().finalPos.isNull())
            {
                p.drawLine(track.value().startPos, track.value().finalPos);
            }

            /// Отображение номера
            p.save();
            p.resetTransform();
            p.translate(width() / 2.0 + getCSAbsShift(), height() / 2.0 + getCSOrdShift());
            QString quant = (!track.value().initIsBG)?(", " + QString::number(track.value().initQuant)):("");
            p.drawText(track.value().exactPos.x() * getCSAbsScale() - 20,
                       track.value().exactPos.y() * getCSOrdScale() - 5, QString::number(track.key()) + quant);

            p.restore();
        }

        /// Отрисовка кратчайших расстояний и касательных от текущего положения трасс до ЗКВ
        if(isVisibleOfMinDists)
        {
            for(track = tracks->constBegin(); track != tracks->end(); ++track)
            {
                if(track.value().numArea != -1 &&
                   areas->contains(track.value().numArea))
                {
                    /// Отрисовка кратчайшего расстояния от текущего положения трассы до центра ЗКВ
                    pen.setColor(Qt::darkRed);
                    p.setPen(pen);
                    p.drawLine(track.value().exactPos, areas->value(track.value().numArea).initPos);

                    /// Отрисовка касательных от текущих координат трассы до границы идентифицированного ЗКВ
                    pen.setColor(Qt::darkBlue);
                    p.setPen(pen);
                    QPointF tanPoint1;
                    QPointF tanPoint2;
                    calcTanPoints(areas->value(track.value().numArea).initPos, areas->value(track.value().numArea).initRadius,
                            track.value().exactPos, tanPoint1, tanPoint2);
                    p.drawLine(track.value().exactPos, tanPoint1);
                    p.drawLine(track.value().exactPos, tanPoint2);
                }
            }
        }
    }

    p.end();
}

/// --------------------------------------------------
/// Статические функции
/// --------------------------------------------------

/// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
void Painter::calcTanPoints(const QPointF &_area, const float _radius, const QPointF &_track, QPointF &_p1, QPointF &_p2)
{
    float distance  = qSqrt((_area.x() - _track.x()) * (_area.x() - _track.x()) +
                            (_area.y() - _track.y()) * (_area.y() - _track.y()));

    QPointF midDistTargetBase((_track - _area) / 2.0);

    float a = midDistTargetBase.x() * midDistTargetBase.x() +
              midDistTargetBase.y() * midDistTargetBase.y();
    float E = 0.5 * (_radius * _radius - 0.25 * distance * distance + a);

    if(qAbs(midDistTargetBase.x()) < ACCURACY_TAN_POINT)
    {
        _p1.setX(qSqrt(_radius * _radius - E * E / (midDistTargetBase.y() * midDistTargetBase.y())));
        _p1.setY(E / midDistTargetBase.y());

        _p2.setX(- qSqrt(_radius * _radius - E * E / (midDistTargetBase.y() * midDistTargetBase.y())));
        _p2.setY(E / midDistTargetBase.y());
    }
    else
    {
        float b = - E * midDistTargetBase.y();
        float c = E * E - (_radius * _radius * midDistTargetBase.x() * midDistTargetBase.x());
        float D = b * b - a * c;

        _p1.setY((- b + qSqrt(D)) / a);
        _p2.setY((- b - qSqrt(D)) / a);

        _p1.setX((E - _p1.y() * midDistTargetBase.y()) / midDistTargetBase.x());
        _p2.setX((E - _p2.y() * midDistTargetBase.y()) / midDistTargetBase.x());
    }

    _p1 += _area;
    _p2 += _area;
}

}
