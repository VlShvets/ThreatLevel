#ifndef DEFINITIONOFTHREATLEVEL_H
#define DEFINITIONOFTHREATLEVEL_H

namespace ThreatLevel
{

}

/// Класс определения уровня угрозы
class DefinitionOfThreatLevel
{
public:
    DefinitionOfThreatLevel();

    void run();

    /// Сглаживание погрешностей измерения
    void    smootheningOfMeasurement();

    /// Идентификация трасс с позиционными районами
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

private:
    /// Вычислительные параметры
    static const float  SMOOTH  = 0.9;  /// Весовой коэфициент сглаживания погрешностей ( < 1.0)
};

}

#endif // DEFINITIONOFTHREATLEVEL_H
