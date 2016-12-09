#include "painter.h"

namespace ThreatLevel
{

Painter::Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent)
    : Grapher2D(_parent), areaParameters(_areaParameters), trackParameters(_trackParameters), results(_results)
{
    setCSAbsMeasure(10000);
    setCSOrdMeasure(10000);
    setCSZoom(10);

    initAreaPar();
    initTrackPar();
}

Painter::~Painter()
{
    for(int i = 0; i < area.count(); ++i)
        area[i].numTrack.clear();
    area.clear();
    track.clear();
}

void Painter::reStart()
{
    initAreaPar();
    initTrackPar();

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
    for(int i = 0; i < area.count(); ++i)
    {
        /// Отрисовка местоположения
        pen.setStyle(Qt::SolidLine);
        pen.setColor(Qt::darkBlue);
        pen.setWidth(3);
        p.setPen(pen);
        p.drawEllipse(area.at(i).initPos, area.at(i).initRadius,
                                          area.at(i).initRadius);

        /// Отрисовка критического времени при минимальной скорости
        pen.setStyle(Qt::DashLine);
        pen.setColor(Qt::darkRed);
        pen.setWidth(1);
        p.setPen(pen);
        p.drawEllipse(area.at(i).initPos, area.at(i).initCritTime * Track::MIN_SPEED,
                                          area.at(i).initCritTime * Track::MIN_SPEED);

        /// Отрисовка критического времени при максимальной скорости
        pen.setColor(Qt::darkGreen);
        p.setPen(pen);
        p.drawEllipse(area.at(i).initPos, area.at(i).initCritTime * Track::MAX_SPEED,
                                          area.at(i).initCritTime * Track::MAX_SPEED);

        /// Отрисовка района обнаружения
        pen.setColor(Qt::darkBlue);
        p.setPen(pen);
        p.drawEllipse(area.at(i).initPos, area.at(i).initRadarRange,
                                          area.at(i).initRadarRange);

        /// Отображение номера
        p.save();
        p.resetTransform();
        p.translate(width() / 2.0 + getCSAbsShift(), height() / 2.0 + getCSOrdShift());
        p.drawText(area.at(i).initPos.x() * getCSAbsScale() - 20,
                   area.at(i).initPos.y() * getCSOrdScale() - 5, QString::number(area.at(i).num + 1));
        p.restore();
    }

    /// Трассы
    pen.setColor(Qt::darkCyan);
    pen.setStyle(Qt::SolidLine);
    for(int j = 0; j < track.count(); ++j)
    {
        /// Отрисовка местоположения
        pen.setWidth(6);
        p.setPen(pen);
        p.drawPoint(track.at(j).pos);

        /// Отрисовка курса
        pen.setWidth(1);
        p.setPen(pen);
        if(!track.at(j).startPos.isNull() &&
           !track.at(j).finalPos.isNull())
        {
            p.drawLine(track.at(j).startPos, track.at(j).finalPos);
        }

        /// Отображение номера
        p.save();
        p.resetTransform();
        p.translate(width() / 2.0 + getCSAbsShift(), height() / 2.0 + getCSOrdShift());
        p.drawText(track.at(j).pos.x() * getCSAbsScale() - 20,
                   track.at(j).pos.y() * getCSOrdScale() - 5, QString::number(track.at(j).num + 1));
        p.restore();
    }

    /// Отрисовка кратчайших расстояний и касательных от текущего положения трасс до ПР
    for(int j = 0; j < track.count(); ++j)
    {
        if(track.at(j).numArea != -1)
        {
            /// Отрисовка кратчайших расстояний от текущего положения трассы до центра ПР
            pen.setColor(Qt::darkRed);
            p.setPen(pen);
            p.drawLine(track.at(j).pos, area.at(track.at(j).numArea).initPos);

            /// Отрисовка касательных от текущего положения трассы до границы ПР
            pen.setColor(Qt::darkBlue);
            p.setPen(pen);
            if(!track.at(j).tanPoint1.isNull())
                p.drawLine(track.at(j).pos, track.at(j).tanPoint1);
            if(!track.at(j).tanPoint2.isNull())
                p.drawLine(track.at(j).pos, track.at(j).tanPoint2);
        }
    }

    p.end();
}

void Painter::timerEvent(QTimerEvent *)
{
    /// Перемещение цели
    /// ==================================================
    for(int j = 0; j < track.count(); ++j)
    {
        /// Точные значения
        track[j].speed  += track.at(j).initAcceleration * DELTAT;
        track[j].pos    += QPointF(track.at(j).speed * qSin(track.at(j).initCourse),
                                   track.at(j).speed * qCos(track.at(j).initCourse)) * DELTAT;

        /// Внесение погрешностей
        track[j].errCourse  = track.at(j).initCourse + qDegreesToRadians(uniformDistribution(0, Track::ERR_COURSE));
        track[j].errSpeed   = track.at(j).speed + gaussDistribution(0, Track::ERR_SPEED);

        /// Сглаживание проекции вектора скорости на ось абсцисс
        if(track.at(j).errVx == 0.0)
            track[j].errVx = track.at(j).errSpeed * qSin(track.at(j).errCourse);
        else
            track[j].errVx = track.at(j).errVx                                      * WEIGHT +
                            (track.at(j).errSpeed * qSin(track.at(j).errCourse))    * (1.0 - WEIGHT);

        /// Сглаживание проекции вектора скорости на ось ординат
        if(track.at(j).errVy == 0.0)
            track[j].errVy = track.at(j).errSpeed * qCos(track.at(j).errCourse);
        else
            track[j].errVy = track.at(j).errVy                                      * WEIGHT +
                            (track.at(j).errSpeed * qCos(track.at(j).errCourse))    * (1.0 - WEIGHT);

        /// Возврат к курсу и скорости
        track[j].errCourse  = qAtan2(track.at(j).errVx, track.at(j).errVy);
        track[j].errSpeed   = qSqrt(track.at(j).errVx * track.at(j).errVx +
                                    track.at(j).errVy * track.at(j).errVy);
    }
    /// ==================================================

    /// Ассоциация трасс с позиционными районами
    /// ==================================================
    for(int j = 0; j < track.count(); ++j)
    {
        /// Определение для трассы номера ближайшего по курсу позиционного района
        float numArea = numOnCourseMinDistanceArea(track.at(j));

        if(track.at(j).numArea != numArea)
        {
            /// Трасса была ассоциированна с ПР
            if(track.at(j).numArea != -1)
            {
                /// Удаление номера трассы из списка номеров ассоциированных с ПР трасс
                if(area.at(track.at(j).numArea).numTrack.contains(j))
                    area[track.at(j).numArea].numTrack.removeOne(j);
            }
            /// Трасса не была ассоциированная с ПР
            else
            {
                /// Задание координат начальной точки траектории
                if(track[j].startPos.isNull())
                    track[j].startPos = track.at(j).pos;
            }

            /// Изменение номера ассоциированного ПР
            track[j].numArea = numArea;

            /// Трасса стала ассоциированна с новым ПР
            if(track.at(j).numArea != -1)
            {
                /// Добавление номера трассы в список номеров ассоциированных с ПР трасс
                if(!area.at(track.at(j).numArea).numTrack.contains(j))
                    area[track.at(j).numArea].numTrack.push_back(j);

                /// Определение расстояния от начальной точки траектории до центра ПР
                track[j].startDist = calcDistance(track.at(j).startPos, area.at(track.at(j).numArea).initPos);

                /// Определение угла между курсом и прямой от начальной точки траектории до центра ПР
                track[j].angCourseToPA = track.at(j).initCourse -
                                         qAtan2(area.at(track.at(j).numArea).initPos.x() - track.at(j).startPos.x(),
                                                area.at(track.at(j).numArea).initPos.y() - track.at(j).startPos.y());
            }
            /// Трасса перестала быть ассоциирована с ПР
            else
            {
                /// Очистка координат экстраполированного конца траектории
                track[j].finalPos = QPointF();
            }
        }
    }
    /// ==================================================

    /// Расчет времени поражения ПР
    /// ==================================================
    for(int j = 0; j < track.count(); ++j)
    {
        int numArea = track.at(j).numArea;
        if(numArea != -1)
        {
            /// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
//            calcTanPoints(area.at(numArea).initPos, area.at(numArea).initRadius,
//                          track.at(j).pos, track[j].tanPoint1, track[j].tanPoint2);

            /// Расчет точных значений переменных
            /// ------------------------------------------

            /// Вычисление расстояния между проекцией трассы на прямую от начальной точки траектории до центра ПР
            /// и центром ПР
            track[j].dist = track.at(j).startDist -
                            calcDistance(track.at(j).startPos, track.at(j).pos) * qCos(track.at(j).angCourseToPA);

            /// Расчет времени поражения ПР
            track[j].time = track.at(j).dist / (track.at(j).speed * qCos(track.at(j).angCourseToPA));
            /// ------------------------------------------

            /// Расчет значений переменных с погрешностями
            /// ------------------------------------------

            /// Определение угла между курсом и прямой от начальной точки траектории до центра ПР с погрешностью
            track[j].errAngCourseToPA = track.at(j).errCourse -
                                        qAtan2(area.at(numArea).initPos.x() - track.at(j).startPos.x(),
                                               area.at(numArea).initPos.y() - track.at(j).startPos.y());

            /// Вычисление расстояние между проекцией трассы на прямую от начальной точки траектории до центра ПР
            /// и центром ПР с погрешностью
            track[j].errDist = track.at(j).startDist -
                               calcDistance(track.at(j).startPos, track.at(j).pos) * qCos(track.at(j).errAngCourseToPA);

            /// Расчет времени поражения ПР с погрешностью
            track[j].errTime = track.at(j).errDist /
                               (track.at(j).errSpeed * qCos(track.at(j).errAngCourseToPA));
            /// ------------------------------------------

            /// Вычисление координат экстраполированного конца траектории
            track[j].finalPos = calcEndPos(track.at(j).startPos, track.at(j).startDist, track.at(j).errAngCourseToPA,
                                           track.at(j).errSpeed, area.at(numArea).initCritTime, track.at(j).errCourse);

            /// Инкрементация количества измерений
            ++track[j].amtMeasure;
        }
    }
    /// ==================================================

    /// Погрешность времени поражения ПР
    /// ==================================================
    for(int i = 0; i < area.count(); ++i)
    {
        if(!area.at(i).numTrack.isEmpty())
        {
            /// Сортировка целей по времени поражения ПР с погрешностью
            quickSortTracks(i, 0, area.at(i).numTrack.count() - 1);

            /// Вычисление разности времени поражения с погрешностью и точного времени поражения в текущий момент времени
            area[i].diffTime = track.at(area.at(i).numTrack.at(0)).errTime - track.at(area.at(i).numTrack.at(0)).time;

            /// Вычисление суммы квадратов разности времени поражения с погрешностью и точного времени поражения
            area[i].sumDiffTime += area.at(i).diffTime * area.at(i).diffTime;

            /// Вычисление среднеквадратической разности времени поражения с погрешностью и точного времени поражения
            area[i].rmsDiffTime = qSqrt(area.at(i).sumDiffTime / track.at(area.at(i).numTrack.at(0)).amtMeasure);
        }
    }

    /// Отображение результатов вычислений
    results->loadTable(area, track);
    /// ==================================================

    repaint();
}

void Painter::initAreaPar()
{
    area.resize(areaParameters->getCount());

    for(int i = 0; i < area.count(); ++i)
    {
        /// Присвоение позиционному району номера
        area[i].num = i;

        /// Загрузка начальных данных из таблицы
        area[i].initPos.setX(areaParameters->getPar(i, 0));
        area[i].initPos.setY(areaParameters->getPar(i, 1));
        area[i].initRadius = areaParameters->getPar(i, 2);
        area[i].initCritTime = areaParameters->getPar(i, 3);
        area[i].initRadarRange = areaParameters->getPar(i, 4);

        /// Обнуление списка номеров ассоциированных с ПР трасс
        area[i].numTrack.clear();

        /// Обнуление рекурентно использующейся
        /// при подсчете среднеквадратической разности времени поражения с погрешностью и точного времени поражения
        /// переменной cуммы квадратов разности времени поражения с погрешностью и точного времени поражения
        area[i].sumDiffTime = 0.0;
    }
}

void Painter::initTrackPar()
{
    track.resize(trackParameters->getCount());

    for(int j = 0; j < track.count(); ++j)
    {
        /// Присовение трассе номера
        track[j].num = j;

        /// Загрузка начальных данных из таблицы
        track[j].initPos.setX(trackParameters->getPar(j, 0));
        track[j].initPos.setY(trackParameters->getPar(j, 1));
        track[j].initCourse = qDegreesToRadians(trackParameters->getPar(j, 2));
        track[j].initSpeed = trackParameters->getPar(j, 3);
        track[j].initAcceleration = trackParameters->getPar(j, 4);

        /// Присвоение текущим координатам начальных координат траектории
        track[j].pos = track.at(j).initPos;

        /// Присвоение текущей скорости начальной скорости
        track[j].speed = track.at(j).initSpeed;

        /// Очистка номера ассоциированного ПР
        track[j].numArea = -1;

        /// Обнуление количества измерений
        track[j].amtMeasure = 0;

        /// Обнуление координат начальной точки и экстраполированного конца траектории
        track[j].startPos = QPointF();
        track[j].finalPos = QPointF();

        /// Обнуление рекурентно использующихся при сглаживании погрешностей переменных
        track[j].errVx = 0.0;
        track[j].errVy = 0.0;
    }
}

int Painter::numOnCourseMinDistanceArea(const Track &_track)
{
    float tempDistance;         /// Расстояние от трассы до ПР
    int numArea = -1;           /// Номер ближайшего по курсу ПР
    float minDistanceOnCourse;  /// Расстояние от трассы до ближайшего по курсу ПР

    for(int i = 0; i < area.count(); ++i)
    {
        tempDistance = calcDistance(_track.pos, area.at(i).initPos);

        /// Определение попадания в зону обнаружения ПР
        if(_track.errSpeed * area.at(i).initCritTime < tempDistance &&
                                                       tempDistance < area.at(i).initRadarRange)
        {
            /// Определение попадания курса в границы ПР
            if(qAbs(_track.errCourse - qAtan2(area.at(i).initPos.x() - _track.pos.x(),
                                              area.at(i).initPos.y() - _track.pos.y())) <
               qAbs(qAsin(area.at(i).initRadius / tempDistance)))
            {
                /// Определение минимальной дистанции
                if(numArea == -1 ||
                   minDistanceOnCourse > tempDistance)
                {
                    numArea = i;
                    minDistanceOnCourse = tempDistance;
                }
            }
        }
    }

    return numArea;
}

void Painter::quickSortTracks(const int _numArea, const int _first, const int _last)
{
    int i = _first;
    int j = _last;
    float x = track.at(area.at(_numArea).numTrack.at((_first + _last) / 2.0)).errTime;

    do
    {
        while(track.at(area.at(_numArea).numTrack.at(i)).errTime < x) i++;
        while(track.at(area.at(_numArea).numTrack.at(j)).errTime > x) j--;

        if(i <= j)
        {
            if(track.at(area.at(_numArea).numTrack.at(i)).errTime > track.at(area.at(_numArea).numTrack.at(j)).errTime)
            {
                int tempNumTrack = area.at(_numArea).numTrack.at(i);
                area[_numArea].numTrack[i] = area.at(_numArea).numTrack.at(j);
                area[_numArea].numTrack[j] = tempNumTrack;
            }
            i++;
            j--;

        }
    }
    while(i <= j);

    if(i < _last)
        quickSortTracks(_numArea, i, _last);
    if(_first < j)
        quickSortTracks(_numArea, _first, j);
}

float Painter::calcDistance(const QPointF &_p1, const QPointF &_p2)
{
    return qSqrt((_p1.x() - _p2.x()) * (_p1.x() - _p2.x()) +
                 (_p1.y() - _p2.y()) * (_p1.y() - _p2.y()));
}

const QPointF Painter::calcEndPos(const QPointF &_startPos, const float _startDist, const float _errAngCourseToPA,
                                  const float _errSpeed, const float _initCritTime, const float _errCourse)
{
    return _startPos + QPointF((_startDist / qCos(_errAngCourseToPA) - _errSpeed * _initCritTime) * qSin(_errCourse),
                               (_startDist / qCos(_errAngCourseToPA) - _errSpeed * _initCritTime) * qCos(_errCourse));
}

void Painter::calcTanPoints(const QPointF &_area, const float _radius, const QPointF &_track, QPointF &_p1, QPointF &_p2)
{
    float distance = calcDistance(_area, _track);

    QPointF midDistTargetBase((_track - _area) / 2.0);

    float a = midDistTargetBase.x() * midDistTargetBase.x() +
              midDistTargetBase.y() * midDistTargetBase.y();
    float E = 0.5 * (_radius * _radius - 0.25 * distance * distance + a);

    if(qAbs(midDistTargetBase.x()) < 1.e-6)
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

float Painter::uniformDistribution(const float _mean, const float _dev)
{
    return 2.0 * _dev * ((float) qrand() / RAND_MAX) - _dev + _mean;
}

float Painter::gaussDistribution(const float _mean, const float _dev)
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

}
