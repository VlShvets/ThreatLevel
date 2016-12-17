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
    for(int i = 0; i < area.count(); ++i)
        area[i].numTrack.clear();
    area.clear();
    track.clear();
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
    for(int i = 0; i < area.count(); ++i)
    {
        /// Отрисовка местоположения
        pen.setStyle(Qt::SolidLine);
        pen.setColor(Qt::darkBlue);
        pen.setWidth(3);
        p.setPen(pen);
        p.drawEllipse(area.at(i).initPos, area.at(i).initRadius,
                      area.at(i).initRadius);

        /// Проверка имеющихся ассоциированных трасс
        if(!area.at(i).numTrack.isEmpty())
        {
            p.save();
            p.setBrush(QBrush(QColor(255, 0, 0, 50)));
            p.drawEllipse(area.at(i).initPos, area.at(i).initRadius,
                          area.at(i).initRadius);
            p.restore();
        }

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
    pen.setStyle(Qt::SolidLine);
    for(int j = 0; j < track.count(); ++j)
    {
        if(!track.at(j).isEmpty)
        {
            if(track.at(j).isBG)
                pen.setColor(Qt::darkMagenta);
            else
                pen.setColor(Qt::darkCyan);

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
                       track.at(j).pos.y() * getCSOrdScale() - 5, QString::number(track.at(j).initNum));
            p.restore();
        }
    }

    /// Отрисовка кратчайших расстояний и касательных от текущего положения трасс до ПР
    for(int j = 0; j < track.count(); ++j)
    {
        if(!track.at(j).isEmpty)
        {
            if(track.at(j).numArea != -1)
            {
                /// Отрисовка кратчайшего расстояния от текущего положения трассы до центра ПР
//                pen.setColor(Qt::darkRed);
//                p.setPen(pen);
//                p.drawLine(track.at(j).pos, area.at(track.at(j).numArea).initPos);

                /// Отрисовка касательных от текущего положения трассы до границы ПР
                pen.setColor(Qt::darkBlue);
                p.setPen(pen);
                if(!track.at(j).tanPoint1.isNull())
                    p.drawLine(track.at(j).pos, track.at(j).tanPoint1);
                if(!track.at(j).tanPoint2.isNull())
                    p.drawLine(track.at(j).pos, track.at(j).tanPoint2);
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
    graphSumTrack->loadGraph(Area::sumTrackCount);

    /// Отображение результатов вычислений
    results->loadTable(area, track);

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

        /// Обнуление суммы квадратов разности времени поражения с погрешностью и точного времени поражения
        area[i].sumDiffTime = 0.0;

        /// Обнуление среднеквадратической разности времени поражения с погрешностью и точного времени поражения
        area[i].rmsDiffTime = 0.0;

        /// Обнуление максимального количества ассоциированных с ПР крылатых ракет
        area[i].maxCMcount = 0;

        /// Обнуление максимального суммарного количества крылатых ракет
        Area::maxSumCMcount = 0;

        /// Обнуление максимального количества ассоциированных с ПР баллистических целей
        area[i].maxBGcount = 0;

        /// Обнуление максимального суммарного количества баллистических целей
        Area::maxSumBGcount = 0;

        /// Обнуление максимального количества ассоциированных с ПР трасс
        area[i].maxTrackCount = 0;

        /// Обнуление максимального количественного состава налета по всем ПР
        Area::maxSumTrackCount = 0;
    }
}

void Painter::initTrackPar()
{
    /// Определение максимального номера трасс
    int maxNumber = 0;
    for(int j = 0; j < trackParameters->getCMCount(); ++j)
    {
        int number = (int) trackParameters->getCMPar(j, 0);
        if(maxNumber < number)
            maxNumber = number;
    }
    for(int j = 0; j < trackParameters->getBGCount(); ++j)
    {
        int number = (int) trackParameters->getBGPar(j, 0);
        if(maxNumber < number)
            maxNumber = number;
    }

    /// Изменение размера массива трасс
    track.clear();
    for(int j = 0; j < maxNumber + 1; ++j)
    {
        track.push_back(Track());
        track.last().isEmpty = true;
    }

    /// Загрузка параметров КР
    for(int j = 0; j < trackParameters->getCMCount(); ++j)
    {
        int num = (int) trackParameters->getCMPar(j, 0);
        track[num].isEmpty = false;

        /// Загрузка начальных данных из таблицы
        track[num].initNum          = num;
        track[num].initStartPos.setX(trackParameters->getCMPar(j, 1));
        track[num].initStartPos.setY(trackParameters->getCMPar(j, 2));
        track[num].initFinalPos     = QPointF();
        track[num].initCourse       = qDegreesToRadians(trackParameters->getCMPar(j, 3));
        track[num].initSpeed        = trackParameters->getCMPar(j, 4);
        track[num].initAcceleration = trackParameters->getCMPar(j, 5);
    }

    /// Загрузка параметров БЦ
    for(int j = 0; j < trackParameters->getBGCount(); ++j)
    {
        int num = (int) trackParameters->getBGPar(j, 0);
        track[num].isEmpty = false;
        track[num].isBG = true;

        /// Загрузка начальных данных из таблицы
        track[num].initNum          = num;
        track[num].initStartPos.setX(trackParameters->getBGPar(j, 1));
        track[num].initStartPos.setY(trackParameters->getBGPar(j, 2));
        track[num].initFinalPos.setX(trackParameters->getBGPar(j, 3));
        track[num].initFinalPos.setY(trackParameters->getBGPar(j, 4));
        track[num].initCourse       = M_PI_2 - qAtan2(track.at(num).initFinalPos.y() - track.at(num).initStartPos.y(),
                                                      track.at(num).initFinalPos.x() - track.at(num).initStartPos.x());
        track[num].initSpeed        = trackParameters->getBGPar(j, 5);
        track[num].initAcceleration = trackParameters->getBGPar(j, 6);
    }

    for(int j = 0; j < track.count(); ++j)
    {
        if(!track.at(j).isEmpty)
        {
            /// Присвоение текущим координатам начальных координат траектории
            track[j].pos    = track.at(j).initStartPos;

            /// Присвоение текущей скорости начальной скорости
            track[j].speed  = track.at(j).initSpeed;

            /// Очистка номера ассоциированного ПР
            track[j].numArea    = -1;

            /// Обнуление количества измерений
            track[j].amtMeasure = 0;

            /// Обнуление координат начальной точки и экстраполированного конца траектории
            track[j].startPos   = QPointF();
            track[j].finalPos   = QPointF();

            /// Обнуление рекурентно использующихся при сглаживании погрешностей переменных
            track[j].errVx  = 0.0;
            track[j].errVy  = 0.0;
        }
    }
}

/// Движение трасс
void Painter::trackMovement()
{
    for(int j = 0; j < track.count(); ++j)
    {
        if(!track.at(j).isEmpty)
        {
            /// Точные значения
            track[j].speed  += track.at(j).initAcceleration * DELTA_T;
            if(track.at(j).speed < 0.0)
                track[j].speed = 0.0;
            track[j].pos    += QPointF(track.at(j).speed * qSin(track.at(j).initCourse),
                                       track.at(j).speed * qCos(track.at(j).initCourse)) * DELTA_T;

            /// Внесение погрешностей
            track[j].errCourse  = track.at(j).initCourse + qDegreesToRadians(uniformDistribution(0, Track::ERR_COURSE));
            track[j].errSpeed   = track.at(j).speed + gaussDistribution(0, Track::ERR_SPEED);

            /// Сглаживание проекции вектора скорости на ось абсцисс
            if(track.at(j).errVx == 0.0)
                track[j].errVx  = track.at(j).errSpeed * qSin(track.at(j).errCourse);
            else
                track[j].errVx  = track.at(j).errVx                                      * WEIGHT +
                                 (track.at(j).errSpeed * qSin(track.at(j).errCourse))    * (1.0 - WEIGHT);

            /// Сглаживание проекции вектора скорости на ось ординат
            if(track.at(j).errVy == 0.0)
                track[j].errVy  = track.at(j).errSpeed * qCos(track.at(j).errCourse);
            else
                track[j].errVy  = track.at(j).errVy                                      * WEIGHT +
                                 (track.at(j).errSpeed * qCos(track.at(j).errCourse))    * (1.0 - WEIGHT);

            /// Возврат к курсу и скорости
            track[j].errCourse  = qAtan2(track.at(j).errVx, track.at(j).errVy);
            track[j].errSpeed   = qSqrt(track.at(j).errVx * track.at(j).errVx +
                                        track.at(j).errVy * track.at(j).errVy);
        }
    }
}

/// Ассоциация трасс с позиционными районами
void Painter::associationTrackArea()
{
    for(int j = 0; j < track.count(); ++j)
    {
        if(!track.isEmpty())
        {
            /// Определение для трассы номера ближайшего по курсу позиционного района
            float numArea   = numOnCourseMinDistanceArea(track.at(j));

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
                        track[j].startPos   = track.at(j).pos;
                }

                /// Изменение номера ассоциированного ПР
                track[j].numArea    = numArea;

                /// Трасса стала ассоциированна с новым ПР
                if(track.at(j).numArea != -1)
                {
                    /// Добавление номера трассы в список номеров ассоциированных с ПР трасс
                    if(!area.at(track.at(j).numArea).numTrack.contains(j))
                        area[track.at(j).numArea].numTrack.push_back(j);

                    /// Определение расстояния от начальной точки траектории до центра ПР
                    track[j].startDist  = calcDistance(track.at(j).startPos, area.at(track.at(j).numArea).initPos);

                    /// Определение угла между курсом и прямой от начальной точки траектории до центра ПР
                    track[j].angCourseToPA  = track.at(j).initCourse -
                                              qAtan2(area.at(track.at(j).numArea).initPos.x() - track.at(j).startPos.x(),
                                                     area.at(track.at(j).numArea).initPos.y() - track.at(j).startPos.y());
                }
                /// Трасса перестала быть ассоциирована с ПР
                else
                {
                    /// Очистка координат экстраполированного конца траектории
                    track[j].finalPos   = QPointF();
                }
            }
        }
    }
}

/// Расчет времени поражения ПР
void Painter::calcErrTime()
{
    for(int j = 0; j < track.count(); ++j)
    {
        if(!track.isEmpty())
        {
            int numArea = track.at(j).numArea;
            if(numArea != -1)
            {
                /// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
//                calcTanPoints(area.at(numArea).initPos, area.at(numArea).initRadius,
//                              track.at(j).pos, track[j].tanPoint1, track[j].tanPoint2);

                /// Расчет точных значений переменных
                /// ------------------------------------------

                /// Вычисление расстояния между проекцией трассы на прямую от начальной точки траектории до центра ПР
                /// и центром ПР
                track[j].dist   = track.at(j).startDist -
                                  calcDistance(track.at(j).startPos, track.at(j).pos) * qCos(track.at(j).angCourseToPA);

                /// Расчет времени поражения ПР
                track[j].time   = track.at(j).dist / (track.at(j).speed * qCos(track.at(j).angCourseToPA));
                /// ------------------------------------------

                /// Расчет значений переменных с погрешностями
                /// ------------------------------------------

                /// Определение угла между курсом и прямой от начальной точки траектории до центра ПР с погрешностью
                track[j].errAngCourseToPA   = track.at(j).errCourse -
                                              qAtan2(area.at(numArea).initPos.x() - track.at(j).startPos.x(),
                                                     area.at(numArea).initPos.y() - track.at(j).startPos.y());

                /// Вычисление расстояние между проекцией трассы на прямую от начальной точки траектории до центра ПР
                /// и центром ПР с погрешностью
                track[j].errDist    = track.at(j).startDist -
                                      calcDistance(track.at(j).startPos, track.at(j).pos) * qCos(track.at(j).errAngCourseToPA);

                /// Расчет времени поражения ПР с погрешностью
                track[j].errTime    = track.at(j).errDist /
                                     (track.at(j).errSpeed * qCos(track.at(j).errAngCourseToPA));
                /// ------------------------------------------

                /// Вычисление координат экстраполированного конца траектории
                track[j].finalPos   = calcEndPos(track.at(j).startPos, track.at(j).startDist, track.at(j).errAngCourseToPA,
                                                 track.at(j).errSpeed, area.at(numArea).initCritTime, track.at(j).errCourse);

                /// Инкрементация количества измерений
                ++track[j].amtMeasure;
            }
        }
    }
}

/// Расчет параметров связанных со временем
void Painter::calcTimeParameters()
{
    Track::numTrackMinErrTime   = -1;
    for(int i = 0; i < area.count(); ++i)
    {
        if(!area.at(i).numTrack.isEmpty())
        {
            /// Сортировка целей по времени поражения ПР с погрешностью
            quickSortTracks(i, 0, area.at(i).numTrack.count() - 1);

            /// Вычисление разности времени поражения с погрешностью и точного времени поражения в текущий момент времени
            area[i].diffTime    = track.at(area.at(i).numTrack.at(0)).errTime - track.at(area.at(i).numTrack.at(0)).time;

            /// Вычисление суммы квадратов разности времени поражения с погрешностью и точного времени поражения
            area[i].sumDiffTime += area.at(i).diffTime * area.at(i).diffTime;

            /// Вычисление среднеквадратической разности времени поражения с погрешностью и точного времени поражения
            area[i].rmsDiffTime = qSqrt(area.at(i).sumDiffTime / track.at(area.at(i).numTrack.at(0)).amtMeasure);

            /// Определение номера трассы с минимальным временем поражения ПР с погрешностью
            if(Track::numTrackMinErrTime == -1 ||
               track.at(Track::numTrackMinErrTime).errTime > track.at(area.at(i).numTrack.at(0)).errTime)
            {
                Track::numTrackMinErrTime   = area.at(i).numTrack.at(0);
            }
        }
    }
}

/// Расчет количества крылатых ракет
void Painter::calcCMcount()
{
    Area::sumCMcount = 0;
    for(int i = 0; i < area.count(); ++i)
    {
        /// Вычисление количества ассоциированных с ПР крылатых ракет
        area[i].CMcount = 0;
        for(int j = 0; j < area.at(i).numTrack.count(); ++j)
        {
            if(!track.at(area.at(i).numTrack.at(j)).isBG)
                ++area[i].CMcount;
        }

        /// Вычисление максимального количества ассоциированных с ПР крылатых ракет
        if(area.at(i).maxCMcount < area.at(i).CMcount)
            area[i].maxCMcount  = area.at(i).CMcount;

        /// Вычисление суммарного количества крылатых ракет
        Area::sumCMcount    += area.at(i).CMcount;
    }

    /// Вычисление максимального суммарного количества крылатых ракет
    if(Area::maxSumCMcount < Area::sumCMcount)
        Area::maxSumCMcount = Area::sumCMcount;
}

/// Расчет количества баллистических целей
void Painter::calcBGcount()
{
    Area::sumBGcount = 0;
    for(int i = 0; i < area.count(); ++i)
    {
        /// Вычисление количества ассоциированных с ПР баллистических целей
        area[i].BGcount = 0;
        for(int j = 0; j < area.at(i).numTrack.count(); ++j)
        {
            if(track.at(area.at(i).numTrack.at(j)).isBG)
                ++area[i].BGcount;
        }

        /// Вычисление максимального количества ассоциированных с ПР баллистических целей
        if(area.at(i).maxBGcount < area.at(i).BGcount)
            area[i].maxBGcount  = area.at(i).BGcount;

        /// Вычисление суммарного количества баллистических целей
        Area::sumBGcount    += area.at(i).BGcount;
    }

    /// Вычисление максимального суммарного количества баллистических целей
    if(Area::maxSumBGcount < Area::sumBGcount)
        Area::maxSumBGcount = Area::sumBGcount;
}

/// Расчет количественного состава налёта
void Painter::calcTrackCount()
{
    Area::sumTrackCount = 0;
    for(int i = 0; i < area.count(); ++i)
    {
        /// Вычисление количественного состава налета по ПР
        area[i].trackCount = area.at(i).CMcount + Track::WEIGHT_COEF_BG * area.at(i).BGcount;

        /// Вычисление максимального количественного состава налета по ПР
        if(area.at(i).maxTrackCount < area.at(i).trackCount)
            area[i].maxTrackCount   = area.at(i).trackCount;

        /// Вычисление количественного состава налета по всем ПР
        Area::sumTrackCount += area.at(i).trackCount;
    }

    /// Вычисление максимального количественного состава налета по всем ПР
    if(Area::maxSumTrackCount < Area::sumTrackCount)
        Area::maxSumTrackCount  = Area::sumTrackCount;
}

/// Определение для трассы номера ближайшего по курсу позиционного района
int Painter::numOnCourseMinDistanceArea(const Track &_track)
{
    int numArea = -1;           /// Номер ближайшего по курсу ПР

    float tempDistance;         /// Расстояние от трассы до ПР
    bool isDetected = false;    /// Флаг обнаружения цели

    /// Определение попадания в зону обнаружения
    for(int i = 0; i < area.count(); ++i)
    {
        tempDistance = calcDistance(_track.pos, area.at(i).initPos);

        if(_track.errSpeed * area.at(i).initCritTime < tempDistance &&
                               area.at(i).initRadius < tempDistance &&
                                                       tempDistance < area.at(i).initRadarRange)
        {
            isDetected = true;
        }
    }

    if(isDetected)
    {
        /// Баллистическая цель
        if(_track.isBG)
        {
            for(int i = 0; i < area.count(); ++i)
            {
                if(calcDistance(area.at(i).initPos, _track.initFinalPos) < area.at(i).initRadius)
                    numArea = i;
            }
        }
        /// Крылатая ракета
        else
        {
            float minDistanceOnCourse;  /// Расстояние от трассы до ближайшего по курсу ПР

            for(int i = 0; i < area.count(); ++i)
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
                        numArea             = i;
                        minDistanceOnCourse = tempDistance;
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
