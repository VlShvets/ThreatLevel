#ifndef DEFINITIONOFTHREATLEVEL_H
#define DEFINITIONOFTHREATLEVEL_H

#include "area.h"
#include "track.h"

#include <QMap>
#include <QtMath>

namespace ThreatLevel
{

/// Класс определения уровня угрозы
class DefinitionOfThreatLevel
{
public:
    explicit        DefinitionOfThreatLevel();
    ~DefinitionOfThreatLevel();

    /// Вычислительный процесс
    void            run();

    /// Установление ЗКВ
    inline void     setAreas(QMap <int, Area> &_areas);

    /// Установление трасс
    inline void     setTracks(QMap <int, Track> &_tracks);

private:
    /// Сглаживание погрешностей измерения
    void            smootheningOfMeasurement();

    /// Идентификация трасс с ЗКВ
    void            identificationOfTracksWithAreas();

    /// Определение номера ЗКВ по попаданию конечной точки траектории БЦ в границы ЗКВ
    int             numAreaHitFinalPosOfBG(const Track &_track);

    /// Определение номера ближайшего ЗКВ по курсу движения КР
    int             numAreaNearestOnCourseOfCM(const Track &_track);

    /// Расчёт времени движения
    void            calculationOfMotionTime();

    /// Определение номера трассы с минимальным временем движения до идентифицированного ЗКВ путем сортировки
    int             numTrackOfMinTime();

    /// Быстрая сортировка трасс по времени движения до идентифицированного ЗКВ
    void            quickSortTracks(QVector <int> &_numTrack, const int _first, const int _last);

    /// Расчет количества крылатых ракет
    void            calculationOfCMCount();

    /// Расчет количества баллистических целей
    void            calculationOfBGCount();

    /// Расчет количественного состава налёта с учетом эквивалента БЦ
    void            calculationOfRaidCount();

    /// Сброс трасс
//    void            resetTracks();

    /// --------------------------------------------------
    /// Статические функции
    /// --------------------------------------------------

    /// Вычисление расстояния между двумя точками
    static float    calcDistance(const QPointF &_p1, const QPointF &_p2);

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Указатели на словари параметров ЗКВ и трасс
    QMap <int, Area>    *areas;     /// Словарь ЗКВ         <номер, структура параметров>
    QMap <int, Track>   *tracks;    /// Словарь трасс       <номер, структура параметров>

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Вычислительные параметры
    static constexpr float  SMOOTH  = 0.9;  /// Весовой коэфициент сглаживания погрешностей ( < 1.0)
};

/// Установление трасс
void DefinitionOfThreatLevel::setAreas(QMap <int, Area> &_areas)
{
    areas = &_areas;
}

/// Установление трасс
void DefinitionOfThreatLevel::setTracks(QMap <int, Track> &_tracks)
{
    tracks = &_tracks;
}

}

#endif // DEFINITIONOFTHREATLEVEL_H
