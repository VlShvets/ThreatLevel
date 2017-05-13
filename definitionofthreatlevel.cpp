#include "definitionofthreatlevel.h"

namespace ThreatLevel
{

/// Класс определения уровня угрозы
DefinitionOfThreatLevel::DefinitionOfThreatLevel()
{
}

DefinitionOfThreatLevel::~DefinitionOfThreatLevel()
{
}

/// Вычислительный процесс
void DefinitionOfThreatLevel::run()
{
    /// Сглаживание погрешностей измерения
    smootheningOfMeasurement();

    /// Идентификация трасс с позиционными районами
    identificationOfTracksWithAreas();

    /// Расчёт времени движения
    calculationOfMotionTime();

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
}

/// Сглаживание погрешностей измерения
void DefinitionOfThreatLevel::smootheningOfMeasurement()
{
    QMap <int, Track>::iterator track = tracks->begin();
    for(; track != tracks->end(); ++track)
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

/// Идентификация трасс с ЗКВ
void DefinitionOfThreatLevel::identificationOfTracksWithAreas()
{
    Area::detectTracksCount = 0;    /// Количество обнаруженных не идентифицированных ни с каким ЗКВ трасс в текущий момент

    QMap <int, Track>::iterator track = tracks->begin();
    for(; track != tracks->end(); ++track)
    {
        /// Определение номера ЗКВ
        float numArea;
        if(track.value().initIsBG)
            numArea = numAreaHitFinalPosOfBG(track.value());
        else
            numArea = numAreaNearestOnCourseOfCM(track.value());

        /// Трасса идентифицированна с ЗКВ
        if(numArea != -1 &&
                areas->contains(numArea))
        {
            /// Вычисление критической дистанции
            float critDistance;     /// Критическая дистанция
            if(track.value().measSpeed * areas->value(numArea).initCritTime > areas->value(numArea).initRadius)
                critDistance = track.value().measSpeed * areas->value(numArea).initCritTime;
            else
                critDistance = areas->value(numArea).initRadius;

            /// Вычисление координат экстраполированного конца траектории
            track.value().finalPos = track.value().startPos +
                    QPointF((track.value().startDist / qCos(track.value().angCourseToPA) - critDistance) * qSin(track.value().measCourse),
                            (track.value().startDist / qCos(track.value().angCourseToPA) - critDistance) * qCos(track.value().measCourse));
        }
        /// Трасса не идентифицированна с ЗКВ
        else
        {
            /// Расчет количества обнаруженных не идентифицированных ни с каким ЗКВ трасс в текущий момент
            ++Area::detectTracksCount;
        }

        if(track.value().numArea != numArea)
        {
            /// Трасса была идентифицированна с другим ЗКВ
            if(track.value().numArea != -1 &&
                    areas->contains(track.value().numArea))
            {
                /// Удаление номера трассы из списка номеров идентифицированных со старым ЗКВ трасс
                if(areas->value(track.value().numArea).numTrack.contains(track.key()))
                    areas->value(track.value().numArea).numTrack.removeOne(track.key());
            }

            /// Изменение номера идентифицированного ЗКВ
            track.value().numArea   = numArea;

            /// Трасса стала идентифицированна с новым ЗКВ
            if(track.value().numArea != -1 &&
                    areas->contains(track.value().numArea))
            {
                /// Добавление номера трассы в список номеров идентифицированных с новым ЗКВ трасс
                if(!areas->value(track.value().numArea).numTrack.contains(track.key()))
                    areas->value(track.value().numArea).numTrack.push_back(track.key());

                /// Определение расстояния от начальной точки траектории до центра ЗКВ
                track.value().startDist     = calcDistance(track.value().startPos, areas->value(track.value().numArea).initPos);

                /// Определение угла между курсом и отрезком от начальной точки траектории до центра ПР
                track.value().angCourseToPA = track.value().measCourse -
                        qAtan2(areas->value(track.value().numArea).initPos.x() - track.value().startPos.x(),
                        areas->value(track.value().numArea).initPos.y() - track.value().startPos.y());

                /// Определение времения преодоления расстояния от начальной точки траектории до центра ПР
                track.value().startTime     = track.value().startDist /
                        (track.value().measSpeed * qCos(track.value().angCourseToPA));
            }
        }
    }
}

/// Расчёт времени движения
void DefinitionOfThreatLevel::calculationOfMotionTime()
{
    QMap <int, Track>::iterator track = tracks->begin();
    for(; track != tracks->end(); ++track)
    {
        int numArea = track.value().numArea;
        if(numArea != -1 &&
           areas->contains(numArea))
        {
            /// --------------------------------------------------
            /// Пересчитываемые связанные с позиционным районом параметры
            /// --------------------------------------------------

            /// Определение угла между курсом и отрезком от начальной точки траектории до центра ПР
            track.value().angCourseToPA = track.value().measCourse -
                                          qAtan2(areas->value(numArea).initPos.x() - track.value().startPos.x(),
                                                 areas->value(numArea).initPos.y() - track.value().startPos.y());

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

/// Определение номера трассы с минимальным временем преодоления расстояния до ПР путем сортировки
int DefinitionOfThreatLevel::numTrackOfMinTime()
{
    int numTrack    = -1;

    QMap <int, Area>::iterator area = areas->begin();
    for(; area != areas->end(); ++area)
    {
        if(!area.value().numTrack.isEmpty())
        {
            /// Сортировка целей по времени поражения данного ПР
            quickSortTracks(area.value().numTrack, 0, area.value().numTrack.count() - 1);

            /// Определение номера трассы с минимальным временем поражения ПР
            if(numTrack == -1 ||
               tracks->value(numTrack).timeToPA > tracks->value(area.value().numTrack.at(0)).timeToPA)
            {
                numTrack   = area.value().numTrack.at(0);
            }
        }
    }

    return numTrack;
}

/// Расчет количества крылатых ракет
void DefinitionOfThreatLevel::calculationOfCMCount()
{
    Area::CMSumCount    = 0;
    QMap <int, Area>::iterator area = areas->begin();
    for(; area != areas->end(); ++area)
    {
        /// Вычисление количества идентифицированных с конкретным ПР крылатых ракет
        area.value().CMCount = 0;
        for(int j = 0; j < area.value().numTrack.count(); ++j)
        {
            if(!tracks->value(area.value().numTrack.at(j)).initIsBG)
                area.value().CMCount += tracks->value(area.value().numTrack.at(j)).initQuant;
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
void DefinitionOfThreatLevel::calculationOfBGCount()
{
    Area::BGSumCount = 0;
    QMap <int, Area>::iterator area = areas->begin();
    for(; area != areas->end(); ++area)
    {
        /// Вычисление количества идентифицированных с конкретным ПР баллистических целей
        area.value().BGCount = 0;
        for(int j = 0; j < area.value().numTrack.count(); ++j)
        {
            if(tracks->value(area.value().numTrack.at(j)).initIsBG)
                area.value().BGCount += tracks->value(area.value().numTrack.at(j)).initQuant;
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
void DefinitionOfThreatLevel::calculationOfRaidCount()
{
    Area::idenTracksMaxCount = 0;
    Area::raidSumCount = 0;
    QMap <int, Area>::iterator area = areas->begin();
    for(; area != areas->end(); ++area)
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

/// Определение номера позиционного района по попаданию конечной точки траектории БЦ
int DefinitionOfThreatLevel::numAreaHitFinalPosOfBG(const Track &_track)
{
    QMap <int, Area>::iterator area = areas->begin();
    for(; area != areas->end(); ++area)
    {
        if(calcDistance(area.value().initPos, _track.initFinalPos) < area.value().initRadius)
            return area.key();
    }

    return -1;
}

/// Определение номера позиционного района ближайшего по курсу КР
int DefinitionOfThreatLevel::numAreaNearestOnCourseOfCM(const Track &_track)
{
    int     numArea = -1;           /// Номер ближайшего по курсу ПР

    float   distance;               /// Расстояние от трассы до ПР
    float   minDistanceOnCourse;    /// Расстояние от трассы до ближайшего ПР по курсу

    QMap <int, Area>::iterator area = areas->begin();
    for(area = areas->begin(); area != areas->end(); ++area)
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
void DefinitionOfThreatLevel::quickSortTracks(QVector <int> &_numTrack, const int _first, const int _last)
{
    int i   = _first;
    int j   = _last;
    float x = tracks->value(_numTrack.at((_first + _last) / 2.0)).timeToPA;

    do
    {
        while(tracks->value(_numTrack.at(i)).timeToPA < x) i++;
        while(tracks->value(_numTrack.at(j)).timeToPA > x) j--;

        if(i <= j)
        {
            if(tracks->value(_numTrack.at(i)).timeToPA > tracks->value(_numTrack.at(j)).timeToPA)
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

/// Сброс трасс
void DefinitionOfThreatLevel::resetTracks()
{
    bool isReset;   /// Флаг сброса трассы

    QMap <int, Track>::iterator track = tracks->begin();
    for(; track != tracks->end();)
    {
        isReset = false;

        int numArea = track.value().numArea;
        if(numArea != -1 &&
           areas->contains(numArea))
        {
            /// Сброс трассы при пересечении границ ПР
            if(track.value().distToPA < areas->value(numArea).initRadius)
                isReset = true;

            /// Сброс трассы при значении времени преодоления расстояния до ПР меньше значения критического времени ПР
            if(track.value().timeToPA < areas->value(numArea).initCritTime)
                isReset = true;
        }

        if(isReset)
        {
            /// Удаление номера трассы из списка номеров ассоциированных с ПР трасс
            if(areas->contains(numArea) &&
               areas->value(numArea).numTrack.contains(track.key()))
            {
                areas->value(numArea).numTrack.removeOne(track.key());
            }

            /// Удаление трассы
            track = tracks->erase(track);
         }
        else
             ++track;
    }
}

/// --------------------------------------------------
/// Статические функции
/// --------------------------------------------------

/// Вычисление расстояния между двумя точками
float DefinitionOfThreatLevel::calcDistance(const QPointF &_p1, const QPointF &_p2)
{
    return qSqrt((_p1.x() - _p2.x()) * (_p1.x() - _p2.x()) +
                 (_p1.y() - _p2.y()) * (_p1.y() - _p2.y()));
}

}
