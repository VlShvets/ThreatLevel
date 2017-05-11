#include "painter.h"

namespace ThreatLevel
{

/// Класс виджета отрисовки трасс и позиционных районов
Painter::Painter(QWidget *_parent)
    : Grapher2D(_parent)
{
    setCSAbsMeasure(ABS_MEASURE);
    setCSOrdMeasure(ORD_MEASURE);
    setCSZoom(DEF_ZOOM);

    initTrackPar();
}

Painter::~Painter()
{
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
        area.value().numTrack.clear();

    areas.clear();
    tracks.clear();
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
    QMap <int, Area>::const_iterator area = areas.begin();
    for(; area != areas.end(); ++area)
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

        /// Отрисовка критического времени при минимально и максимально возможной скорости
        pen.setStyle(Qt::DashLine);
        pen.setColor(Qt::darkRed);
        pen.setWidth(1);
        p.setPen(pen);
        p.drawEllipse(area.value().initPos, area.value().initCritTime * Track::MIN_SPEED,
                      area.value().initCritTime * Track::MIN_SPEED);
        p.drawEllipse(area.value().initPos, area.value().initCritTime * Track::MAX_SPEED,
                      area.value().initCritTime * Track::MAX_SPEED);

        /// Отрисовка района обнаружения
        pen.setColor(Qt::darkBlue);
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

    /// Трассы
    pen.setStyle(Qt::SolidLine);
    QMap <int, Track>::const_iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
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

    /// Отрисовка кратчайших расстояний и касательных от текущего положения трасс до ПР
    for(track = tracks.begin(); track != tracks.end(); ++track)
    {
        if(track.value().numArea != -1)
        {
            /// Отрисовка кратчайшего расстояния от текущего положения трассы до центра ПР
            pen.setColor(Qt::darkRed);
            p.setPen(pen);
//            if(areas.contains(track.value().numArea))
//                p.drawLine(track.value().pos, areas[track.value().numArea].initPos);

            /// Отрисовка касательных от текущего положения трассы до границы ПР
            pen.setColor(Qt::darkBlue);
            p.setPen(pen);
            if(!track.value().tanPoint1.isNull())
                p.drawLine(track.value().exactPos, track.value().tanPoint1);
            if(!track.value().tanPoint2.isNull())
                p.drawLine(track.value().exactPos, track.value().tanPoint2);
        }
    }

    p.end();
}

void Painter::initTrackPar()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        /// Присвоение точным текущим координатам координат начала движения трассы
        track.value().exactPos      = track.value().initStartPos;

        /// Обнуление координат начальной точки и экстраполированного конца траектории
        track.value().startPos      = QPointF();
        track.value().finalPos      = QPointF();

        /// Обнуление количества измерений
        track.value().countMeas     = 0;

        /// Обнуление рекурентно использующихся при сглаживании проекций вектора скорости
        track.value().smoothVx      = 0.0;
        track.value().smoothVy      = 0.0;

        /// Очистка номера идентифицированного ПР
        track.value().numArea       = -1;

        /// Обнуление суммы квадратов разности вычисленного и измеренного времени поражения ПР
        track.value().sumDiffTime   = 0.0;

        /// Обнуление среднеквадратической разности вычисленного и измеренного времени поражения ПР
        track.value().rmsDiffTime   = 0.0;
    }
}

/// --------------------------------------------------
/// Статические функции
/// --------------------------------------------------

/// Вычисление расстояния между двумя точками
float Painter::calcDistance(const QPointF &_p1, const QPointF &_p2)
{
    return qSqrt((_p1.x() - _p2.x()) * (_p1.x() - _p2.x()) +
                 (_p1.y() - _p2.y()) * (_p1.y() - _p2.y()));
}

/// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
void Painter::calcTanPoints(const QPointF &_area, const float _radius, const QPointF &_track, QPointF &_p1, QPointF &_p2)
{
    float distance  = calcDistance(_area, _track);

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
