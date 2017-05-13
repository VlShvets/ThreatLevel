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
    explicit    DefinitionOfThreatLevel();
    ~DefinitionOfThreatLevel();

    /// Вычислительный процесс
    void        run();

    /// Установление ЗКВ
    void        setAreas(QMap <int, Area> *_areas);

    /// Установление трасс
    void        setTracks(QMap <int, Track> *_tracks);

private:
    /// Сглаживание погрешностей измерения
    void    smootheningOfMeasurement();

    /// Идентификация трасс с ЗКВ
    void    identificationOfTracksWithAreas();

    /// Расчёт времени движения
    void    calculationOfMotionTime();

    /// Определение номера трассы с минимальным временем преодоления расстояния до ПР путем сортировки
    int     numTrackOfMinTime();

    /// Расчет количества крылатых ракет
    void    calculationOfCMCount();

    /// Расчет количества баллистических целей
    void    calculationOfBGCount();

    /// Расчет количественного состава налёта с учетом тротилового эквивалента БЦ
    void    calculationOfRaidCount();

    /// Определение номера позиционного района по попаданию конечной точки траектории БЦ
    int     numAreaHitFinalPosOfBG(const Track &_track);

    /// Определение номера позиционного района ближайшего по курсу КР
    int     numAreaNearestOnCourseOfCM(const Track &_track);

    /// Быстрая сортировка трасс по времени поражения определенного ПР
    void    quickSortTracks(QVector <int> &_numTrack, const int _first, const int _last);

    /// Сброс трасс
    void    resetTracks();

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

void DefinitionOfThreatLevel::setAreas(QMap <int, Area> *_areas)
{
    areas = _areas;
}

void DefinitionOfThreatLevel::setTracks(QMap <int, Track> *_tracks)
{
    tracks = _tracks;
}

}

#endif // DEFINITIONOFTHREATLEVEL_H
