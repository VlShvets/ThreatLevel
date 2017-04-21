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
    /// Движение трассы
    tracksMovement();

    /// Ассоциация трасс с позиционными районами
    associationTrackArea();

    /// Расчет времени поражения ПР
    calcTime();

    /// Сортировка трасс и определение номера трассы с минимальным временем поражения ПР
    calcNumTrackMinErrTime();

    /// Сброс трасс
    resetTracks();

    /// Расчет количества крылатых ракет
    calcCMcount();

    /// Расчет количества баллистических целей
    calcBGcount();

    /// Расчет количественного состава налёта с учетом тратилового эквивалента БЦ
    calcRaidCount();

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
        /// Обнуление максимального количества ассоциированных с ПР крылатых ракет
        area.value().CMMaxCount     = 0;

        /// Обнуление максимального количества ассоциированных с ПР баллистических целей
        area.value().BGMaxCount     = 0;

        /// Обнуление максимального количества ассоциированных с ПР трасс
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
        /// Присвоение текущим координатам начальных координат траектории
        track.value().exactPos      = track.value().initStartPos;

        /// Обнуление координат начальной точки и экстраполированного конца траектории
        track.value().startPos      = QPointF();
        track.value().finalPos      = QPointF();

        /// Обнуление количества измерений
        track.value().countMeas     = 0;

        /// Обнуление рекурентно использующихся при сглаживании проекций вектора скорости с погрешностями
        track.value().smoothVx      = 0.0;
        track.value().smoothVy      = 0.0;

        /// Очистка номера ассоциированного ПР
        track.value().numArea       = -1;

        /// Обнуление суммы квадратов разности времени поражения с погрешностью и точного времени поражения
        track.value().sumDiffTime   = 0.0;

        /// Обнуление среднеквадратической разности времени поражения с погрешностью и точного времени поражения
        track.value().rmsDiffTime   = 0.0;
    }
}

/// Движение трасс и сглаживание измерений
void Painter::tracksMovement()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        /// Точные значения
        track.value().exactPos      += QPointF(track.value().initSpeed  * qSin(track.value().initCourse),
                                               track.value().initSpeed  * qCos(track.value().initCourse))   * DELTA_T;

        /// Внесение погрешностей
        track.value().measSpeed     = track.value().initSpeed  + gaussDistribution(0, Track::ERR_SPEED);
        track.value().measCourse    = track.value().initCourse  + qDegreesToRadians(uniformDistribution(0, Track::ERR_COURSE));

        /// Сглаживание проекции вектора скорости на ось абсцисс
        if(track.value().smoothVx == 0.0)
            track.value().smoothVx  = track.value().measSpeed * qSin(track.value().measCourse);
        else
            track.value().smoothVx  = track.value().smoothVx                                    * SMOOTH +
                                     (track.value().measSpeed * qSin(track.value().measCourse)) * (1.0 - SMOOTH);

        /// Сглаживание проекции вектора скорости на ось ординат
        if(track.value().smoothVy == 0.0)
            track.value().smoothVy  = track.value().measSpeed * qCos(track.value().measCourse);
        else
            track.value().smoothVy  = track.value().smoothVy                                    * SMOOTH +
                                     (track.value().measSpeed * qCos(track.value().measCourse)) * (1.0 - SMOOTH);

        /// Возврат к курсу и скорости
        track.value().measCourse    = qAtan2(track.value().smoothVx, track.value().smoothVy);
        track.value().measSpeed     = qSqrt(track.value().smoothVx * track.value().smoothVx +
                                            track.value().smoothVy * track.value().smoothVy);
    }
}

/// Ассоциация трасс с позиционными районами
void Painter::associationTrackArea()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        /// Определение для трассы номера ближайшего по курсу позиционного района
        float numArea   = numOnCourseMinDistanceArea(track.value());

        if(track.value().numArea != numArea)
        {
            /// Трасса была ассоциированна с другим ПР
            if(track.value().numArea != -1 &&
               areas.contains(track.value().numArea))
            {
                /// Удаление номера трассы из списка номеров ассоциированных с ПР трасс
                if(areas[track.value().numArea].numTrack.contains(track.key()))
                    areas[track.value().numArea].numTrack.removeOne(track.key());
            }
            /// Трасса не была ассоциированная ни с каким ПР
            else
            {
                /// Задание координат начальной точки траектории
                if(track.value().startPos.isNull())
                    track.value().startPos  = track.value().exactPos;
            }

            /// Изменение номера ассоциированного ПР
            track.value().numArea   = numArea;

            /// Трасса стала ассоциированна с новым ПР
            if(track.value().numArea != -1 &&
               areas.contains(track.value().numArea))
            {
                /// Добавление номера трассы в список номеров ассоциированных с ПР трасс
                if(!areas[track.value().numArea].numTrack.contains(track.key()))
                    areas[track.value().numArea].numTrack.push_back(track.key());

                /// Определение расстояния от начальной точки траектории до центра ПР
                track.value().startDist     = calcDistance(track.value().startPos, areas[track.value().numArea].initPos);

                /// Определение угла между курсом и прямой от начальной точки траектории до центра ПР
                track.value().angCourseToPA = track.value().measCourse -
                                              qAtan2(areas[track.value().numArea].initPos.x() - track.value().startPos.x(),
                                                     areas[track.value().numArea].initPos.y() - track.value().startPos.y());

                /// Определение времени поражения ПР от начальной точки траектории
                track.value().startTime     = track.value().startDist /
                                             (track.value().measSpeed * qCos(track.value().angCourseToPA));
            }
            /// Трасса перестала быть ассоциирована с каким-либо ПР
            else
            {
                /// Очистка координат экстраполированного конца траектории
                track.value().finalPos  = QPointF();
            }
        }
        else if(numArea == -1)
        {
            /// Присвоение начальной точки траектории текущих координат
            track.value().startPos      = track.value().exactPos;

            /// Подсчёт точки для отображения курса до ассоциации с ПР
            track.value().finalPos      = track.value().startPos + LENGTH * QPointF(qSin(track.value().initCourse),
                                                                                    qCos(track.value().initCourse));
        }
    }
}

/// Расчет времени поражения ПР
void Painter::calcTime()
{
    QMap <int, Track>::iterator track = tracks.begin();
    for(; track != tracks.end(); ++track)
    {
        int numArea = track.value().numArea;
        if(numArea != -1 &&
           areas.contains(numArea))
        {
            /// Инкрементация количества измерений
            ++track.value().countMeas;

            /// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
//            calcTanPoints(areas[numArea].initPos, areas[numArea].initRadius,
//                          track.value().pos, track.value().tanPoint1, track.value().tanPoint2);

            /// --------------------------------------------------
            /// Пересеёт связанных с ПР переменных
            /// --------------------------------------------------

            /// Определение угла между курсом и прямой от начальной точки траектории до центра ПР с погрешностью
            track.value().angCourseToPA = track.value().measCourse -
                                          qAtan2(areas[numArea].initPos.x() - track.value().startPos.x(),
                                                 areas[numArea].initPos.y() - track.value().startPos.y());

            /// Вычисление расстояние между проекцией трассы на прямую от начальной точки траектории до центра ПР
            /// и центром ПР с погрешностью
            track.value().distToPA  = track.value().startDist -
                                      calcDistance(track.value().startPos, track.value().exactPos) * qCos(track.value().angCourseToPA);

            /// Расчет времени поражения ПР с погрешностью
            track.value().timeToPA  = track.value().distToPA /
                                     (track.value().measSpeed * qCos(track.value().angCourseToPA));

            /// --------------------------------------------------
            /// Расчет среднеквадратической разности времени поражения с погрешностью и точного времени поражения
            /// --------------------------------------------------

            /// Вычисление разности времени поражения с погрешностью и точного времени поражения в текущий момент времени
            track.value().diffTime      = track.value().startTime -
                                         (calcDistance(track.value().startPos, track.value().exactPos) / track.value().measSpeed) -
                                          track.value().timeToPA;

            /// Вычисление суммы квадратов разности времени поражения с погрешностью и точного времени поражения
            track.value().sumDiffTime   += track.value().diffTime * track.value().diffTime;

            /// Вычисление среднеквадратической разности времени поражения с погрешностью и точного времени поражения
            track.value().rmsDiffTime   = qSqrt(track.value().sumDiffTime / track.value().countMeas);

            /// Вычисление координат экстраполированного конца траектории
            track.value().finalPos      = calcEndPos(track.value(), areas[numArea]);
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

            /// Сброс трассы при значении времени поражения ПР меньше значения критического времени ПР
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

/// Сортировка трасс и определение номера трассы с минимальным временем поражения ПР
void Painter::calcNumTrackMinErrTime()
{
    Track::numTrackMinErrTime   = -1;
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        if(!area.value().numTrack.isEmpty())
        {
            /// Сортировка целей по времени поражения данного ПР с погрешностью
            quickSortTracks(area.value().numTrack, 0, area.value().numTrack.count() - 1);

            /// Определение номера трассы с минимальным временем поражения ПР с погрешностью
            if(Track::numTrackMinErrTime == -1 ||
               tracks[Track::numTrackMinErrTime].timeToPA > tracks[area.value().numTrack.at(0)].timeToPA)
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
            if(!tracks[area.value().numTrack.at(j)].initIsBG)
                area.value().CMCount += tracks[area.value().numTrack.at(j)].initQuant;
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
            if(tracks[area.value().numTrack.at(j)].initIsBG)
                area.value().BGCount += tracks[area.value().numTrack.at(j)].initQuant;
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

/// Расчет количественного состава налёта с учетом тратилового эквивалента БЦ
void Painter::calcRaidCount()
{
    Area::maxTracksCount = 0;
    Area::raidSumCount = 0;
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        /// Вычисление количественного состава налета по ПР
        area.value().raidCount = area.value().CMCount + Track::BG_WEIGHT_COEF * area.value().BGCount;

        /// Вычисление максимального количественного состава налета по ПР
        if(area.value().raidMaxCount < area.value().raidCount)
            area.value().raidMaxCount  = area.value().raidCount;

        /// Вычисление количественного состава налета по всем ПР
        Area::raidSumCount += area.value().raidCount;

        /// Определение максимального количества ассоциированных с каким-либо ПР трасс в текущий момент
        if(Area::maxTracksCount < area.value().numTrack.count())
            Area::maxTracksCount = area.value().numTrack.count();
    }

    /// Вычисление максимального количественного состава налета по всем ПР
    if(Area::raidMaxSumCount < Area::raidSumCount)
        Area::raidMaxSumCount  = Area::raidSumCount;
}

/// Определение для трассы номера ближайшего по курсу позиционного района
int Painter::numOnCourseMinDistanceArea(const Track &_track)
{
    int numArea = -1;           /// Номер ближайшего по курсу ПР

    /// Определение попадания в зону обнаружения
    bool isDetected = false;    /// Флаг попадания в зону обнаружения
    QMap <int, Area>::const_iterator area = areas.begin();
    for(; area != areas.end(); ++area)
    {
        if(calcDistance(_track.exactPos, area.value().initPos) < area.value().initDetectRange)
        {
            isDetected = true;
            break;
        }
    }

    /// Трасса в зоне обнаружения
    if(isDetected)
    {
        /// Баллистическая цель
        if(_track.initIsBG)
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
            float distance;     /// Расстояние от трассы до ПР
            float minDistanceOnCourse;  /// Расстояние от трассы до ближайшего ПР по курсу

            for(area = areas.begin(); area != areas.end(); ++area)
            {
                distance = calcDistance(_track.exactPos, area.value().initPos);

                /// Определение попадания курса в границы ПР
                if(qAbs(_track.measCourse - qAtan2(area.value().initPos.x() - _track.exactPos.x(),
                                                   area.value().initPos.y() - _track.exactPos.y())) <
                   qAbs(qAsin(area.value().initRadius / distance)))
                {
                    /// Определение минимальной дистанции
                    if(numArea == -1 ||
                       minDistanceOnCourse > distance)
                    {
                        numArea             = area.key();
                        minDistanceOnCourse = distance;
                    }
                }
            }
        }
    }

    return numArea;
}

/// Быстрая сортировка целей по времени поражения с погрешностью определенного ПР
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

/// Вычисление координат экстраполированного конца траектории
const QPointF Painter::calcEndPos(const Track &_track, const Area &_area)
{
    float distance;
    if(_track.measSpeed * _area.initCritTime > _area.initRadius)
        distance = _track.measSpeed * _area.initCritTime;
    else
        distance = _area.initRadius;

    return _track.startPos + QPointF((_track.startDist / qCos(_track.angCourseToPA) - distance) * qSin(_track.measCourse),
                                     (_track.startDist / qCos(_track.angCourseToPA) - distance) * qCos(_track.measCourse));
}

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
