#include "painter.h"

namespace ThreatLevel
{

/// Класс виджета отрисовки трасс и позиционных районов
Painter::Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent)
    : Grapher2D(_parent), areaParameters(_areaParameters), trackParameters(_trackParameters), results(_results)
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
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
        area.value().numTrack.clear();

    areas.clear();
    tracks.clear();

    initAreaPar();
    initTrackPar();

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

void Painter::timerEvent(QTimerEvent *)
{
    /// Движение трасс и внесение погрешностей измерения
    movementOfTracks();

    /// Сглаживание погрешностей измерения
    smootheningOfMeasurement();

    /// Идентификация трасс с позиционными районами
    identificationOfTracksWithAreas();

    /// Расчет связанных со временем переменных
    calculationOfTime();

    /// Определение номера трассы с минимальным временем преодоления расстояния до ПР путем сортировки
    Track::numTrackMinErrTime = numTrackOfMinTime();

    /// Сброс трасс
    resetTracks();

    /// Расчет количества крылатых ракет
    calculationOfCMCount();

    /// Расчет количества баллистических целей
    calculationOfBGCount();

    /// Расчет количественного состава налёта с учетом тротилового эквивалента БЦ
    calculationOfRaidCount();

    /// Отображение результатов вычислений
    results->loadTable(areas, tracks);

    repaint();
}

void Painter::initAreaPar()
{
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
        areas[num].initDetectRange  = areaParameters->getPar(i, 5);
    }

    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Обнуление максимального количества идентифицированных с ПР крылатых ракет
        area.value().CMMaxCount     = 0;

        /// Обнуление максимального количества идентифицированных с ПР баллистических целей
        area.value().BGMaxCount     = 0;

        /// Обнуление максимального количества идентифицированных с ПР трасс
        area.value().raidMaxCount   = 0;
    }

    /// Обнуление максимального суммарного количества крылатых ракет
    Area::CMMaxSumCount     = 0;

    /// Обнуление максимального суммарного количества баллистических целей
    Area::BGMaxSumCount     = 0;

    /// Обнуление максимального количественного состава налета по всем ПР
    Area::raidMaxSumCount   = 0;
}

void Painter::initTrackPar()
{
    /// Загрузка параметров КР
    for(int j = 0; j < trackParameters->getCMCount(); ++j)
    {
        /// Определение номера трассы
        int num = (int) trackParameters->getCMPar(j, 0);

        /// Загрузка начальных данных из таблицы
        tracks[num].initIsBG            = false;
        tracks[num].initStartPos.setX(trackParameters->getCMPar(j, 1));
        tracks[num].initStartPos.setY(trackParameters->getCMPar(j, 2));
        tracks[num].initSpeed           = trackParameters->getCMPar(j, 3);
        tracks[num].initFinalPos        = QPointF();
        tracks[num].initCourse          = qDegreesToRadians(trackParameters->getCMPar(j, 4));
        tracks[num].initQuant           = trackParameters->getCMPar(j, 5);
    }

    /// Загрузка параметров БЦ
    for(int j = 0; j < trackParameters->getBGCount(); ++j)
    {
        /// Определение номера трассы
        int num                         = (int) trackParameters->getBGPar(j, 0);

        /// Загрузка начальных данных из таблицы        
        tracks[num].initIsBG            = true;
        tracks[num].initStartPos.setX(trackParameters->getBGPar(j, 1));
        tracks[num].initStartPos.setY(trackParameters->getBGPar(j, 2));
        tracks[num].initSpeed           = trackParameters->getBGPar(j, 3);
        tracks[num].initFinalPos.setX(trackParameters->getBGPar(j, 4));
        tracks[num].initFinalPos.setY(trackParameters->getBGPar(j, 5));
        tracks[num].initCourse          = qAtan2(tracks[num].initFinalPos.x() - tracks[num].initStartPos.x(),
                                                 tracks[num].initFinalPos.y() - tracks[num].initStartPos.y());
        tracks[num].initQuant           = 1;
    }

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

/// Движение трасс и внесение погрешностей измерения
void Painter::movementOfTracks()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        /// Изменение точных значений текущих координат трассы
        track.value().exactPos      += QPointF(track.value().initSpeed  * qSin(track.value().initCourse),
                                               track.value().initSpeed  * qCos(track.value().initCourse)) * DELTA_T;

        /// Внесение погрешностей измерения
        track.value().measSpeed     = track.value().initSpeed   + gaussDistribution(0, Track::ERR_SPEED);
        track.value().measCourse    = track.value().initCourse  + qDegreesToRadians(uniformDistribution(0, Track::ERR_COURSE));
    }
}

/// Сглаживание погрешностей измерения
void Painter::smootheningOfMeasurement()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        /// Проецирование вектора скорости на ось абсцисс со сглаживанием погрешностей измерения
        if(track.value().smoothVx == 0.0)
            track.value().smoothVx  = track.value().measSpeed * qSin(track.value().measCourse);
        else
            track.value().smoothVx  = track.value().smoothVx                                    * SMOOTH +
                                     (track.value().measSpeed * qSin(track.value().measCourse)) * (1.0 - SMOOTH);

        /// Проецирование вектора скорости на ось ординат со сглаживанием погрешностей измерения
        if(track.value().smoothVy == 0.0)
            track.value().smoothVy  = track.value().measSpeed * qCos(track.value().measCourse);
        else
            track.value().smoothVy  = track.value().smoothVy                                    * SMOOTH +
                                     (track.value().measSpeed * qCos(track.value().measCourse)) * (1.0 - SMOOTH);

        /// Возврат к значениям скорости и курса
        track.value().measSpeed     = qSqrt(track.value().smoothVx * track.value().smoothVx +
                                            track.value().smoothVy * track.value().smoothVy);
        track.value().measCourse    = qAtan2(track.value().smoothVx, track.value().smoothVy);
    }
}

/// Идентификация трасс с позиционными районами
void Painter::identificationOfTracksWithAreas()
{
    Area::detectTracksCount = 0;    /// Количествообнаруженных не идентифицированных ни с каким ПР трасс в текущий момент

    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        if(trackIsDetected(track.value()))
        {
            /// Определение номера позиционного района
            float numArea;
            if(track.value().initIsBG)
                numArea = numAreaHitFinalPosOfBG(track.value());
            else
                numArea = numAreaNearestOnCourseOfCM(track.value());

            /// Трасса идентифицированна с ПР
            if(numArea != -1 &&
               areas.contains(numArea))
            {
                /// Вычисление критической дистанции
                float critDistance;     /// Критическая дистанция
                if(track.value().measSpeed * areas[numArea].initCritTime > areas[numArea].initRadius)
                    critDistance = track.value().measSpeed * areas[numArea].initCritTime;
                else
                    critDistance = areas[numArea].initRadius;

                /// Вычисление координат экстраполированного конца траектории
                track.value().finalPos = track.value().startPos +
                        QPointF((track.value().startDist / qCos(track.value().angCourseToPA) - critDistance) * qSin(track.value().measCourse),
                                (track.value().startDist / qCos(track.value().angCourseToPA) - critDistance) * qCos(track.value().measCourse));

                /// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
    //            calcTanPoints(areas[numArea].initPos, areas[numArea].initRadius,
    //                          track.value().pos, track.value().tanPoint1, track.value().tanPoint2);
            }
            /// Трасса не идентифицированна с ПР
            else
            {
                /// Расчет количества обнаруженных не идентифицированных ни с каким ПР трасс в текущий момент
                ++Area::detectTracksCount;

                /// Подсчет координат конечной точки линии направления вектора скорости
                track.value().finalPos = track.value().exactPos;
            }

            if(track.value().numArea != numArea)
            {
                /// Трасса была идентифицированна с другим ПР
                if(track.value().numArea != -1 &&
                   areas.contains(track.value().numArea))
                {
                    /// Удаление номера трассы из списка номеров идентифицированных со старым ПР трасс
                    if(areas[track.value().numArea].numTrack.contains(track.key()))
                        areas[track.value().numArea].numTrack.removeOne(track.key());
                }

                /// Изменение номера идентифицированного ПР
                track.value().numArea   = numArea;

                /// Трасса стала идентифицированна с новым ПР
                if(track.value().numArea != -1 &&
                   areas.contains(track.value().numArea))
                {
                    /// Добавление номера трассы в список номеров идентифицированных с новым ПР трасс
                    if(!areas[track.value().numArea].numTrack.contains(track.key()))
                        areas[track.value().numArea].numTrack.push_back(track.key());

                    /// Определение расстояния от начальной точки траектории до центра ПР
                    track.value().startDist     = calcDistance(track.value().startPos, areas[track.value().numArea].initPos);

                    /// Определение угла между курсом и отрезком от начальной точки траектории до центра ПР
                    track.value().angCourseToPA = track.value().measCourse -
                                                  qAtan2(areas[track.value().numArea].initPos.x() - track.value().startPos.x(),
                                                         areas[track.value().numArea].initPos.y() - track.value().startPos.y());

                    /// Определение времения преодоления расстояния от начальной точки траектории до центра ПР
                    track.value().startTime     = track.value().startDist /
                                                 (track.value().measSpeed * qCos(track.value().angCourseToPA));
                }
            }
        }
        else
        {
            /// Присвоение координатам начальной точки траектории текущих координат
            track.value().startPos  = track.value().exactPos;

            /// Подсчет координат конечной точки линии направления вектора скорости
            track.value().finalPos  = track.value().startPos + LENGTH * QPointF(qSin(track.value().initCourse),
                                                                                qCos(track.value().initCourse));
        }
    }
}

/// Расчет связанных со временем переменных
void Painter::calculationOfTime()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        int numArea = track.value().numArea;
        if(numArea != -1 &&
           areas.contains(numArea))
        {
            /// --------------------------------------------------
            /// Пересчитываемые связанные с позиционным районом параметры
            /// --------------------------------------------------

            /// Определение угла между курсом и отрезком от начальной точки траектории до центра ПР
            track.value().angCourseToPA = track.value().measCourse -
                                          qAtan2(areas[numArea].initPos.x() - track.value().startPos.x(),
                                                 areas[numArea].initPos.y() - track.value().startPos.y());

            /// Вычисление расстояния от проекции текущей точки траектории до центра ПР
            track.value().distToPA  = track.value().startDist -
                                      calcDistance(track.value().startPos, track.value().exactPos) * qCos(track.value().angCourseToPA);

            /// Вычисление времени преодоления расстояния от проекции текущей точки траектории до центра ПР
            track.value().timeToPA  = track.value().distToPA /
                                     (track.value().measSpeed * qCos(track.value().angCourseToPA));

            /// --------------------------------------------------
            /// СКО времени преодоления расстояния до ПР
            /// --------------------------------------------------

            /// Инкрементация количества измерений
            ++track.value().countMeas;

            /// Расчет разности вычисленного и измеренного времени преодоления расстояния до ПР
            track.value().diffTime      = track.value().startTime -
                                         (calcDistance(track.value().startPos, track.value().exactPos) / track.value().measSpeed) -
                                          track.value().timeToPA;

            /// Расчет суммы квадратов разности времени преодоления расстояния до ПР
            track.value().sumDiffTime   += track.value().diffTime * track.value().diffTime;

            /// Расчет среднеквадратического отклонения времени преодоления расстояния до ПР
            track.value().rmsDiffTime   = qSqrt(track.value().sumDiffTime / track.value().countMeas);
        }
    }
}

/// Сброс трасс
void Painter::resetTracks()
{
    bool isReset;   /// Флаг сброса трассы

    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end();)
    {
        isReset = false;

        int numArea = track.value().numArea;
        if(numArea != -1 &&
           areas.contains(numArea))
        {
            /// Сброс трассы при пересечении границ ПР
            if(track.value().distToPA < areas[numArea].initRadius)
                isReset = true;

            /// Сброс трассы при значении времени преодоления расстояния до ПР меньше значения критического времени ПР
            if(track.value().timeToPA < areas[numArea].initCritTime)
                isReset = true;
        }

        if(isReset)
        {
            /// Удаление номера трассы из списка номеров ассоциированных с ПР трасс
            if(areas.contains(numArea) &&
               areas[numArea].numTrack.contains(track.key()))
            {
                areas[numArea].numTrack.removeOne(track.key());
            }

            /// Удаление трассы
            track = tracks.erase(track);
         }
        else
             ++track;
    }
}

/// Определение номера трассы с минимальным временем преодоления расстояния до ПР путем сортировки
int Painter::numTrackOfMinTime()
{
    int numTrack    = -1;

    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        if(!area.value().numTrack.isEmpty())
        {
            /// Сортировка целей по времени поражения данного ПР
            quickSortTracks(area.value().numTrack, 0, area.value().numTrack.count() - 1);

            /// Определение номера трассы с минимальным временем поражения ПР
            if(numTrack == -1 ||
               tracks[numTrack].timeToPA > tracks[area.value().numTrack.at(0)].timeToPA)
            {
                numTrack   = area.value().numTrack.at(0);
            }
        }
    }

    return numTrack;
}

/// Расчет количества крылатых ракет
void Painter::calculationOfCMCount()
{
    Area::CMSumCount    = 0;
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Вычисление количества идентифицированных с конкретным ПР крылатых ракет
        area.value().CMCount = 0;
        for(int j = 0; j < area.value().numTrack.count(); ++j)
        {
            if(!tracks[area.value().numTrack.at(j)].initIsBG)
                area.value().CMCount += tracks[area.value().numTrack.at(j)].initQuant;
        }

        /// Вычисление максимального количества идентифицированных с конкретным ПР крылатых ракет
        if(area.value().CMMaxCount < area.value().CMCount)
            area.value().CMMaxCount = area.value().CMCount;

        /// Вычисление суммарного количества крылатых ракет по всем ПР
        Area::CMSumCount    += area.value().CMCount;
    }

    /// Вычисление максимального суммарного количества крылатых ракет с учетом не идентифицированных трасс
    if(Area::CMMaxSumCount < Area::CMSumCount + Area::detectTracksCount)
        Area::CMMaxSumCount = Area::CMSumCount + Area::detectTracksCount;
}

/// Расчет количества баллистических целей
void Painter::calculationOfBGCount()
{
    Area::BGSumCount = 0;
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Вычисление количества идентифицированных с конкретным ПР баллистических целей
        area.value().BGCount = 0;
        for(int j = 0; j < area.value().numTrack.count(); ++j)
        {
            if(tracks[area.value().numTrack.at(j)].initIsBG)
                area.value().BGCount += tracks[area.value().numTrack.at(j)].initQuant;
        }

        /// Вычисление максимального количества идентифицированных с конкретным ПР баллистических целей
        if(area.value().BGMaxCount < area.value().BGCount)
            area.value().BGMaxCount = area.value().BGCount;

        /// Вычисление суммарного количества баллистических целей по всем ПР
        Area::BGSumCount    += area.value().BGCount;
    }

    /// Вычисление максимального суммарного количества баллистических целей по всем ПР
    if(Area::BGMaxSumCount < Area::BGSumCount)
        Area::BGMaxSumCount = Area::BGSumCount;
}

/// Расчет количественного состава налёта с учетом тротилового эквивалента БЦ
void Painter::calculationOfRaidCount()
{
    Area::idenTracksMaxCount = 0;
    Area::raidSumCount = 0;
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Вычисление количественного состава налета по конкретному ПР с учетом тротилового эквивалента БЦ
        area.value().raidCount = area.value().CMCount + Track::BG_WEIGHT_COEF * area.value().BGCount;

        /// Вычисление максимального количественного состава налета по конкретному ПР с учетом тротилового эквивалента БЦ
        if(area.value().raidMaxCount < area.value().raidCount)
            area.value().raidMaxCount  = area.value().raidCount;

        /// Вычисление количественного состава налета по всем ПР с учетом тротилового эквивалента БЦ
        Area::raidSumCount  += area.value().raidCount;

        /// Определение максимального количества идентифицированных с каким-либо ПР трасс в текущий момент
        if(Area::idenTracksMaxCount < area.value().numTrack.count())
            Area::idenTracksMaxCount = area.value().numTrack.count();
    }

    /// Вычисление максимального количественного состава налета с учетом не идентифицированных трасс
    if(Area::raidMaxSumCount < Area::raidSumCount + Area::detectTracksCount)
        Area::raidMaxSumCount  = Area::raidSumCount + Area::detectTracksCount;
}


/// Определение попадания в зону обнаружения
bool Painter::trackIsDetected(const Track &_track)
{
    QMap <int, Area>::const_iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        if(calcDistance(_track.exactPos, area.value().initPos) < area.value().initDetectRange)
            return true;
    }

    return false;
}

/// Определение номера позиционного района по попаданию конечной точки траектории БЦ
int Painter::numAreaHitFinalPosOfBG(const Track &_track)
{
    QMap <int, Area>::iterator area = areas.begin();
    for(area = areas.begin(); area != areas.end(); ++area)
    {
        if(calcDistance(area.value().initPos, _track.initFinalPos) < area.value().initRadius)
            return area.key();
    }

    return -1;
}

/// Определение номера позиционного района ближайшего по курсу КР
int Painter::numAreaNearestOnCourseOfCM(const Track &_track)
{
    int     numArea = -1;           /// Номер ближайшего по курсу ПР

    float   distance;               /// Расстояние от трассы до ПР
    float   minDistanceOnCourse;    /// Расстояние от трассы до ближайшего ПР по курсу

    QMap <int, Area>::iterator area = areas.begin();
    for(area = areas.begin(); area != areas.end(); ++area)
    {
        /// Определение расстояния от трассы до ПР
        distance = calcDistance(_track.exactPos, area.value().initPos);

        /// Определение попадания курса в границы ПР
        if(qAbs(_track.measCourse - qAtan2(area.value().initPos.x() - _track.exactPos.x(),
                                           area.value().initPos.y() - _track.exactPos.y())) <
           qAbs(qAsin(area.value().initRadius / distance)))
        {
            /// Определение минимального расстояния
            if(numArea == -1 ||
               minDistanceOnCourse > distance)
            {
                numArea             = area.key();
                minDistanceOnCourse = distance;
            }
        }
    }

    return numArea;
}

/// Быстрая сортировка целей по времени поражения определенного ПР
void Painter::quickSortTracks(QVector <int> &_numTrack, const int _first, const int _last)
{
    int i   = _first;
    int j   = _last;
    float x = tracks[_numTrack.at((_first + _last) / 2.0)].timeToPA;

    do
    {
        while(tracks[_numTrack.at(i)].timeToPA < x) i++;
        while(tracks[_numTrack.at(j)].timeToPA > x) j--;

        if(i <= j)
        {
            if(tracks[_numTrack.at(i)].timeToPA > tracks[_numTrack.at(j)].timeToPA)
            {
                int tempNumTrack = _numTrack.at(i);
                _numTrack[i] = _numTrack.at(j);
                _numTrack[j] = tempNumTrack;
            }
            i++;
            j--;

        }
    }
    while(i <= j);

    if(i < _last)
        quickSortTracks(_numTrack, i, _last);
    if(_first < j)
        quickSortTracks(_numTrack, _first, j);
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
