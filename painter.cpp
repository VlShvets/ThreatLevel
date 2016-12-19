#include "painter.h"

namespace ThreatLevel
{

/// Класс виджета отрисовки трасс и позиционных районов
Painter::Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters,
                 GraphSumTrack *_graphSumTrack, Results *_results, QWidget *_parent)
    : Grapher2D(_parent), areaParameters(_areaParameters), trackParameters(_trackParameters),
      graphSumTrack(_graphSumTrack), results(_results)
{
    setCSAbsMeasure(ABS_MEASURE);
    setCSOrdMeasure(ORD_MEASURE);
    setCSZoom(DEF_ZOOM);

    initAreaPar();
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

/// Перезапуск
void Painter::reStart()
{
    initAreaPar();
    initTrackPar();

    graphSumTrack->resetGraph();
    results->resetTable();

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
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Отрисовка местоположения
        pen.setStyle(Qt::SolidLine);
        pen.setColor(Qt::darkBlue);
        pen.setWidth(3);
        p.setPen(pen);
        p.drawEllipse(area.value().initPos, area.value().initRadius,
                      area.value().initRadius);

        /// Проверка имеющихся ассоциированных трасс
        if(!area.value().numTrack.isEmpty())
        {
            p.save();
            p.setBrush(QBrush(QColor(255, 0, 0, 50)));
            p.drawEllipse(area.value().initPos, area.value().initRadius,
                          area.value().initRadius);
            p.restore();
        }

        /// Отрисовка критического времени при минимальной скорости
        pen.setStyle(Qt::DashLine);
        pen.setColor(Qt::darkRed);
        pen.setWidth(1);
        p.setPen(pen);
        p.drawEllipse(area.value().initPos, area.value().initCritTime * Track::MIN_SPEED,
                      area.value().initCritTime * Track::MIN_SPEED);

        /// Отрисовка критического времени при максимальной скорости
        pen.setColor(Qt::darkGreen);
        p.setPen(pen);
        p.drawEllipse(area.value().initPos, area.value().initCritTime * Track::MAX_SPEED,
                      area.value().initCritTime * Track::MAX_SPEED);

        /// Отрисовка района обнаружения
        pen.setColor(Qt::darkBlue);
        p.setPen(pen);
        p.drawEllipse(area.value().initPos, area.value().initRadarRange,
                      area.value().initRadarRange);

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
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        if(!track.value().isReset)
        {
            if(track.value().isBG)
                pen.setColor(Qt::darkMagenta);
            else
                pen.setColor(Qt::darkCyan);

            /// Отрисовка местоположения
            pen.setWidth(6);
            p.setPen(pen);
            p.drawPoint(track.value().pos);

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
            p.drawText(track.value().pos.x() * getCSAbsScale() - 20,
                       track.value().pos.y() * getCSOrdScale() - 5, QString::number(track.key()));
            p.restore();
        }
    }

    /// Отрисовка кратчайших расстояний и касательных от текущего положения трасс до ПР
    for(track = tracks.begin(); track != tracks.end(); ++track)
    {
        if(!track.value().isReset)
        {
            if(track.value().numArea != -1)
            {
                /// Отрисовка кратчайшего расстояния от текущего положения трассы до центра ПР
                pen.setColor(Qt::darkRed);
                p.setPen(pen);
//                p.drawLine(track.value().pos, areas[track.value().numArea].initPos);

                /// Отрисовка касательных от текущего положения трассы до границы ПР
                pen.setColor(Qt::darkBlue);
                p.setPen(pen);
                if(!track.value().tanPoint1.isNull())
                    p.drawLine(track.value().pos, track.value().tanPoint1);
                if(!track.value().tanPoint2.isNull())
                    p.drawLine(track.value().pos, track.value().tanPoint2);
            }
        }
    }

    p.end();
}

void Painter::timerEvent(QTimerEvent *)
{
    /// Движение трассы
    trackMovement();

    /// Ассоциация трасс с позиционными районами
    associationTrackArea();

    /// Расчет времени поражения ПР
    calcErrTime();

    /// Расчет параметров связанных со временем
    calcTimeParameters();

    /// Расчет количества крылатых ракет
    calcCMcount();

    /// Расчет количества баллистических целей
    calcBGcount();

    /// Расчет количественного состава налёта
    calcTrackCount();

    /// Отображение графика количественного состава налета
    graphSumTrack->loadGraph(Area::trackSumCount);

    /// Отображение результатов вычислений
    results->loadTable(areas, tracks);

    repaint();
}

void Painter::initAreaPar()
{
    areas.clear();

    /// Загрузка параметров ПР
    for(int i = 0; i < areaParameters->getCount(); ++i)
    {
        /// Определение номера ПР
        int num                     = (int) areaParameters->getPar(i, 0);

        /// Загрузка начальных данных из таблицы
        areas[num].initPos.setX(areaParameters->getPar(i, 1));
        areas[num].initPos.setY(areaParameters->getPar(i, 2));
        areas[num].initRadius       = areaParameters->getPar(i, 3);
        areas[num].initCritTime     = areaParameters->getPar(i, 4);
        areas[num].initRadarRange   = areaParameters->getPar(i, 5);
    }

    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Обнуление списка номеров ассоциированных с ПР трасс
        area.value().numTrack.clear();

        /// Обнуление суммы квадратов разности времени поражения с погрешностью и точного времени поражения
        area.value().sumDiffTime    = 0.0;

        /// Обнуление среднеквадратической разности времени поражения с погрешностью и точного времени поражения
        area.value().rmsDiffTime    = 0.0;

        /// Обнуление максимального количества ассоциированных с ПР крылатых ракет
        area.value().CMMaxCount     = 0;

        /// Обнуление максимального количества ассоциированных с ПР баллистических целей
        area.value().BGMaxCount     = 0;

        /// Обнуление максимального количества ассоциированных с ПР трасс
        area.value().trackMaxCount  = 0;
    }

    /// Обнуление максимального суммарного количества крылатых ракет
    Area::CMMaxSumCount     = 0;

    /// Обнуление максимального суммарного количества баллистических целей
    Area::BGMaxSumCount     = 0;

    /// Обнуление максимального количественного состава налета по всем ПР
    Area::trackMaxSumCount  = 0;
}

void Painter::initTrackPar()
{
    tracks.clear();

    /// Загрузка параметров КР
    for(int j = 0; j < trackParameters->getCMCount(); ++j)
    {
        /// Определение номера трассы
        int num = (int) trackParameters->getCMPar(j, 0);
        tracks[num].isReset             = false;
        tracks[num].isBG                = false;

        /// Загрузка начальных данных из таблицы
        tracks[num].initStartPos.setX(trackParameters->getCMPar(j, 1));
        tracks[num].initStartPos.setY(trackParameters->getCMPar(j, 2));
        tracks[num].initFinalPos        = QPointF();
        tracks[num].initCourse          = qDegreesToRadians(trackParameters->getCMPar(j, 3));
        tracks[num].initSpeed           = trackParameters->getCMPar(j, 4);
        tracks[num].initAcceleration    = trackParameters->getCMPar(j, 5);
    }

    /// Загрузка параметров БЦ
    for(int j = 0; j < trackParameters->getBGCount(); ++j)
    {
        /// Определение номера трассы
        int num                         = (int) trackParameters->getBGPar(j, 0);
        tracks[num].isReset             = false;
        tracks[num].isBG                = true;

        /// Загрузка начальных данных из таблицы
        tracks[num].initStartPos.setX(trackParameters->getBGPar(j, 1));
        tracks[num].initStartPos.setY(trackParameters->getBGPar(j, 2));
        tracks[num].initFinalPos.setX(trackParameters->getBGPar(j, 3));
        tracks[num].initFinalPos.setY(trackParameters->getBGPar(j, 4));
        tracks[num].initCourse          = M_PI_2 - qAtan2(tracks[num].initFinalPos.y() - tracks[num].initStartPos.y(),
                                                          tracks[num].initFinalPos.x() - tracks[num].initStartPos.x());
        tracks[num].initSpeed           = trackParameters->getBGPar(j, 5);
        tracks[num].initAcceleration    = trackParameters->getBGPar(j, 6);
    }

    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        /// Присвоение текущим координатам начальных координат траектории
        track.value().pos           = track.value().initStartPos;

        /// Присвоение текущей скорости начальной скорости
        track.value().speed         = track.value().initSpeed;

        /// Очистка номера ассоциированного ПР
        track.value().numArea       = -1;

        /// Обнуление количества измерений
        track.value().amtMeasure    = 0;

        /// Обнуление координат начальной точки и экстраполированного конца траектории
        track.value().startPos      = QPointF();
        track.value().finalPos      = QPointF();

        /// Обнуление рекурентно использующихся при сглаживании погрешностей переменных
        track.value().errVx         = 0.0;
        track.value().errVy         = 0.0;
    }
}

/// Движение трасс
void Painter::trackMovement()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        if(!track.value().isReset)
        {
            /// Точные значения
            track.value().speed += track.value().initAcceleration * DELTA_T;
            if(track.value().speed < 0.0)
                track.value().isReset = true;
            track.value().pos   += QPointF(track.value().speed * qSin(track.value().initCourse),
                                           track.value().speed * qCos(track.value().initCourse))    * DELTA_T;

            /// Внесение погрешностей
            track.value().errCourse = track.value().initCourse + qDegreesToRadians(uniformDistribution(0, Track::ERR_COURSE));
            track.value().errSpeed  = track.value().speed + gaussDistribution(0, Track::ERR_SPEED);

            /// Сглаживание проекции вектора скорости на ось абсцисс
            if(track.value().errVx == 0.0)
                track.value().errVx = track.value().errSpeed * qSin(track.value().errCourse);
            else
                track.value().errVx = track.value().errVx                                           * WEIGHT +
                                     (track.value().errSpeed * qSin(track.value().errCourse))       * (1.0 - WEIGHT);

            /// Сглаживание проекции вектора скорости на ось ординат
            if(track.value().errVy == 0.0)
                track.value().errVy = track.value().errSpeed * qCos(track.value().errCourse);
            else
                track.value().errVy = track.value().errVy                                           * WEIGHT +
                                     (track.value().errSpeed * qCos(track.value().errCourse))       * (1.0 - WEIGHT);

            /// Возврат к курсу и скорости
            track.value().errCourse  = qAtan2(track.value().errVx, track.value().errVy);
            track.value().errSpeed   = qSqrt(track.value().errVx * track.value().errVx +
                                             track.value().errVy * track.value().errVy);
        }
    }
}

/// Ассоциация трасс с позиционными районами
void Painter::associationTrackArea()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        if(!track.value().isReset)
        {
            /// Определение для трассы номера ближайшего по курсу позиционного района
            float numArea   = numOnCourseMinDistanceArea(track.value());

            if(track.value().numArea != numArea)
            {
                /// Трасса была ассоциированна с ПР
                if(track.value().numArea != -1)
                {
                    /// Удаление номера трассы из списка номеров ассоциированных с ПР трасс
                    if(areas[track.value().numArea].numTrack.contains(track.key()))
                        areas[track.value().numArea].numTrack.removeOne(track.key());
                }
                /// Трасса не была ассоциированная с ПР
                else
                {
                    /// Задание координат начальной точки траектории
                    if(track.value().startPos.isNull())
                        track.value().startPos  = track.value().pos;
                }

                /// Изменение номера ассоциированного ПР
                track.value().numArea   = numArea;

                /// Трасса стала ассоциированна с новым ПР
                if(track.value().numArea != -1)
                {
                    /// Добавление номера трассы в список номеров ассоциированных с ПР трасс
                    if(!areas[track.value().numArea].numTrack.contains(track.key()))
                        areas[track.value().numArea].numTrack.push_back(track.key());

                    /// Определение расстояния от начальной точки траектории до центра ПР
                    track.value().startDist = calcDistance(track.value().startPos, areas[track.value().numArea].initPos);

                    /// Определение угла между курсом и прямой от начальной точки траектории до центра ПР
                    track.value().angCourseToPA = track.value().initCourse -
                                                  qAtan2(areas[track.value().numArea].initPos.x() - track.value().startPos.x(),
                                                         areas[track.value().numArea].initPos.y() - track.value().startPos.y());
                }
                /// Трасса перестала быть ассоциирована с ПР
                else
                {
                    /// Очистка координат экстраполированного конца траектории
                    track.value().finalPos  = QPointF();
                }
            }
        }
    }
}

/// Расчет времени поражения ПР
void Painter::calcErrTime()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        if(!track.value().isReset)
        {
            int numArea = track.value().numArea;
            if(numArea != -1)
            {
                /// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
//                calcTanPoints(areas[numArea].initPos, areas[numArea].initRadius,
//                              track.value().pos, track.value().tanPoint1, track.value().tanPoint2);

                /// Расчет точных значений переменных
                /// ------------------------------------------

                /// Вычисление расстояния между проекцией трассы на прямую от начальной точки траектории до центра ПР
                /// и центром ПР
                track.value().dist  = track.value().startDist -
                                      calcDistance(track.value().startPos, track.value().pos) * qCos(track.value().angCourseToPA);

                /// Расчет времени поражения ПР
                track.value().time  = track.value().dist / (track.value().speed * qCos(track.value().angCourseToPA));
                /// ------------------------------------------

                /// Расчет значений переменных с погрешностями
                /// ------------------------------------------

                /// Определение угла между курсом и прямой от начальной точки траектории до центра ПР с погрешностью
                track.value().errAngCourseToPA  = track.value().errCourse -
                                                  qAtan2(areas[numArea].initPos.x() - track.value().startPos.x(),
                                                         areas[numArea].initPos.y() - track.value().startPos.y());

                /// Вычисление расстояние между проекцией трассы на прямую от начальной точки траектории до центра ПР
                /// и центром ПР с погрешностью
                track.value().errDist   = track.value().startDist -
                                          calcDistance(track.value().startPos, track.value().pos) * qCos(track.value().errAngCourseToPA);

                /// Расчет времени поражения ПР с погрешностью
                track.value().errTime   = track.value().errDist /
                                          (track.value().errSpeed * qCos(track.value().errAngCourseToPA));
                /// ------------------------------------------

                /// Вычисление координат экстраполированного конца траектории
                track.value().finalPos  = calcEndPos(track.value().startPos, track.value().startDist, track.value().errAngCourseToPA,
                                                     track.value().errSpeed, areas[numArea].initCritTime, track.value().errCourse);

                /// Инкрементация количества измерений
                ++track.value().amtMeasure;
            }
        }
    }
}

/// Расчет параметров связанных со временем
void Painter::calcTimeParameters()
{
    Track::numTrackMinErrTime   = -1;
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        if(!area.value().numTrack.isEmpty())
        {
            /// Сортировка целей по времени поражения ПР с погрешностью
            quickSortTracks(area.key(), 0, area.value().numTrack.count() - 1);

            /// Вычисление разности времени поражения с погрешностью и точного времени поражения в текущий момент времени
            area.value().diffTime       = tracks[area.value().numTrack.at(0)].errTime - tracks[area.value().numTrack.at(0)].time;

            /// Вычисление суммы квадратов разности времени поражения с погрешностью и точного времени поражения
            area.value().sumDiffTime    += area.value().diffTime * area.value().diffTime;

            /// Вычисление среднеквадратической разности времени поражения с погрешностью и точного времени поражения
            area.value().rmsDiffTime    = qSqrt(area.value().sumDiffTime / tracks[area.value().numTrack.at(0)].amtMeasure);

            /// Определение номера трассы с минимальным временем поражения ПР с погрешностью
            if(Track::numTrackMinErrTime == -1 ||
               tracks[Track::numTrackMinErrTime].errTime > tracks[area.value().numTrack.at(0)].errTime)
            {
                Track::numTrackMinErrTime   = area.value().numTrack.at(0);
            }
        }
    }
}

/// Расчет количества крылатых ракет
void Painter::calcCMcount()
{
    Area::CMSumCount    = 0;
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Вычисление количества ассоциированных с ПР крылатых ракет
        area.value().CMCount = 0;
        for(int j = 0; j < area.value().numTrack.count(); ++j)
        {
            if(!tracks[area.value().numTrack.at(j)].isBG)
                ++area.value().CMCount;
        }

        /// Вычисление максимального количества ассоциированных с ПР крылатых ракет
        if(area.value().CMMaxCount < area.value().CMCount)
            area.value().CMMaxCount = area.value().CMCount;

        /// Вычисление суммарного количества крылатых ракет
        Area::CMSumCount    += area.value().CMCount;
    }

    /// Вычисление максимального суммарного количества крылатых ракет
    if(Area::CMMaxSumCount < Area::CMSumCount)
        Area::CMMaxSumCount = Area::CMSumCount;
}

/// Расчет количества баллистических целей
void Painter::calcBGcount()
{
    Area::BGSumCount = 0;
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Вычисление количества ассоциированных с ПР баллистических целей
        area.value().BGCount = 0;
        for(int j = 0; j < area.value().numTrack.count(); ++j)
        {
            if(tracks[area.value().numTrack.at(j)].isBG)
                ++area.value().BGCount;
        }

        /// Вычисление максимального количества ассоциированных с ПР баллистических целей
        if(area.value().BGMaxCount < area.value().BGCount)
            area.value().BGMaxCount = area.value().BGCount;

        /// Вычисление суммарного количества баллистических целей
        Area::BGSumCount    += area.value().BGCount;
    }

    /// Вычисление максимального суммарного количества баллистических целей
    if(Area::BGMaxSumCount < Area::BGSumCount)
        Area::BGMaxSumCount = Area::BGSumCount;
}

/// Расчет количественного состава налёта
void Painter::calcTrackCount()
{
    Area::trackSumCount = 0;
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Вычисление количественного состава налета по ПР
        area.value().trackCount = area.value().CMCount + Track::BG_WEIGHT_COEF * area.value().BGCount;

        /// Вычисление максимального количественного состава налета по ПР
        if(area.value().trackMaxCount < area.value().trackCount)
            area.value().trackMaxCount  = area.value().trackCount;

        /// Вычисление количественного состава налета по всем ПР
        Area::trackSumCount += area.value().trackCount;
    }

    /// Вычисление максимального количественного состава налета по всем ПР
    if(Area::trackMaxSumCount < Area::trackSumCount)
        Area::trackMaxSumCount  = Area::trackSumCount;
}

/// Определение для трассы номера ближайшего по курсу позиционного района
int Painter::numOnCourseMinDistanceArea(const Track &_track)
{
    int numArea = -1;           /// Номер ближайшего по курсу ПР

    /// Определение попадания в зону обнаружения
    bool isDetected = false;    /// Флаг попадания в зону обнаружения
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        if(calcDistance(_track.pos, area.value().initPos) < area.value().initRadarRange)
        {
            isDetected = true;
            break;
        }
    }

    /// Трасса в зоне обнаружения
    if(isDetected)
    {
        /// Баллистическая цель
        if(_track.isBG)
        {
            for(area = areas.begin(); area != areas.end(); ++area)
            {
                if(calcDistance(area.value().initPos, _track.initFinalPos) < area.value().initRadius)
                {
                    numArea = area.key();
                    break;
                }
            }
        }
        /// Крылатая ракета
        else
        {
            float distanceOnCourse;     /// Расстояние от трассы до ПР по курсу
            float minDistanceOnCourse;  /// Расстояние от трассы до ближайшего ПР по курсу

            for(area = areas.begin(); area != areas.end(); ++area)
            {
                distanceOnCourse = calcDistance(_track.pos, area.value().initPos);

                /// Определение попадания курса в границы ПР
                if(qAbs(_track.errCourse - qAtan2(area.value().initPos.x() - _track.pos.x(),
                                                  area.value().initPos.y() - _track.pos.y())) <
                   qAbs(qAsin(area.value().initRadius / distanceOnCourse)))
                {
                    /// Определение минимальной дистанции
                    if(numArea == -1 ||
                       minDistanceOnCourse > distanceOnCourse)
                    {
                        numArea             = area.key();
                        minDistanceOnCourse = distanceOnCourse;
                    }
                }
            }
        }
    }

    return numArea;
}

/// Быстрая сортировка целей по времени поражения с погрешностью определенного ПР
void Painter::quickSortTracks(const int _numArea, const int _first, const int _last)
{
    int i   = _first;
    int j   = _last;
    float x = tracks[areas[_numArea].numTrack.at((_first + _last) / 2.0)].errTime;

    do
    {
        while(tracks[areas[_numArea].numTrack.at(i)].errTime < x) i++;
        while(tracks[areas[_numArea].numTrack.at(j)].errTime > x) j--;

        if(i <= j)
        {
            if(tracks[areas[_numArea].numTrack.at(i)].errTime > tracks[areas[_numArea].numTrack.at(j)].errTime)
            {
                int tempNumTrack = areas[_numArea].numTrack.at(i);
                areas[_numArea].numTrack[i] = areas[_numArea].numTrack.at(j);
                areas[_numArea].numTrack[j] = tempNumTrack;
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

/// Вычисление расстояния между двумя точками
float Painter::calcDistance(const QPointF &_p1, const QPointF &_p2)
{
    return qSqrt((_p1.x() - _p2.x()) * (_p1.x() - _p2.x()) +
                 (_p1.y() - _p2.y()) * (_p1.y() - _p2.y()));
}

/// Вычисление координат экстраполированного конца траектории
const QPointF Painter::calcEndPos(const QPointF &_startPos, const float _startDist, const float _errAngCourseToPA,
                                  const float _errSpeed, const float _initCritTime, const float _errCourse)
{
    return _startPos + QPointF((_startDist / qCos(_errAngCourseToPA) - _errSpeed * _initCritTime) * qSin(_errCourse),
                               (_startDist / qCos(_errAngCourseToPA) - _errSpeed * _initCritTime) * qCos(_errCourse));
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

/// Равномерное распределение
float Painter::uniformDistribution(const float _mean, const float _dev)
{
    return 2.0 * _dev * ((float) qrand() / RAND_MAX) - _dev + _mean;
}

/// Распределение Гаусса
float Painter::gaussDistribution(const float _mean, const float _dev)
{
    static bool ready   = false;
    static float second = 0.0;

    if(ready)
    {
        ready   = false;
        return second * _dev + _mean;
    }

    float u, v, s;
    do
    {
        u   = 2.0 * ((float) qrand() / RAND_MAX) - 1.0;
        v   = 2.0 * ((float) qrand() / RAND_MAX) - 1.0;
        s   = u * u + v * v;
    }
    while(s > 1.0 || s == 0.0);

    float r = qSqrt(-2.0 * qLn(s) / s);
    second  = r * u;

    if(qAbs(second) < 1.5)
        ready   = true;

    if(qAbs(r * v) < 1.5)
        return r * v * _dev + _mean;

    return gaussDistribution(_mean, _dev);
}

}
