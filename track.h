#ifndef TRACK
#define TRACK

#include <QPointF>

namespace ThreatLevel
{

/// Структура параметров трасс
struct Track
{
    /// Начальные параметры
    bool        initIsBG;               /// Признак БЦ
    QPointF     initStartPos;           /// Координаты начала движения трассы
    float       initSpeed;              /// Начальная скорость
    QPointF     initFinalPos;           /// Координаты конечной точки
    float       initCourse;             /// Начальный курс в радианах
    int         initQuant;              /// Количественный состав (для КР: может принимать значения 1 - 15)

    /// Точные значения текущих параметров
    QPointF     exactPos;               /// Точные значения текущих координат трассы

    /// Параметры измерения с погрешностью
    float       measCourse;             /// Текущий измеренный курс в радианах с погрешностью
    float       measSpeed;              /// Текущая измеренная скорость с погрешностью

    /// Сглаженные проекции вектора скорости
    float       smoothVx;               /// Сглаженная проекция вектора скорости на ось абсцисс
    float       smoothVy;               /// Сглаженная проекция вектора скорости на ось ординат

    /// Параметры траектории с учетом текущих измерений
    /// (или линия направления вектора скорости в случае отсутствия ассоциированного ПР)
    QPointF     startPos;               /// Координаты начальной (или текущей) точки траектории
    QPointF     finalPos;               /// Координаты конечной точки экстраполированной траектории
                                        /// (или линии направления вектора скорости)

    /// Инициируемые при идентификации с позиционным районом параметры
    int         numArea;                /// Номер ПР (-1 - ни с каким ПР трасса не идентифицированна)
    float       startDist;              /// Расстояние от начальной точки траектории до центра ПР
    float       startTime;              /// Время преодоления расстояния от начальной точки траектории до центра ПР

    /// Точки соприкосновения касательных от текущих координат трассы до границы ассоциированного ПР
    QPointF     tanPoint1;              /// Точка соприкосновения касательных №1
    QPointF     tanPoint2;              /// Точка соприкосновения касательных №2

    /// Пересчитываемые связанные с позиционным районом параметры
    float       angCourseToPA;          /// Угол между курсом и отрезком от начальной точки траектории до центра ПР
    float       distToPA;               /// Расстояние от проекции текущей точки траектории до центра ПР
    float       timeToPA;               /// Время преодоления расстояния от проекции текущей точки траектории до центра ПР

    /// --------------------------------------------------
    /// Результирующие параметры по конкретной трассе
    /// --------------------------------------------------

    /// СКО времени преодоления расстояния до ПР
    int         countMeas;              /// Количество измерений
    float       diffTime;               /// Разность вычисленного и измеренного времени преодоления расстояния до ПР
    float       sumDiffTime;            /// Сумма квадратов разности времени преодоления расстояния до ПР
    float       rmsDiffTime;            /// Среднеквадратическое отклонение времени преодоления расстояния до ПР

    /// --------------------------------------------------
    /// Результирующие параметры по всем трассам
    /// --------------------------------------------------

    /// Трасса с минимальным временем поражения позиционного района
    static int  numTrackMinErrTime;     /// Номер трассы с минимальным временем поражения ПР

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Тратиловый эквивалент баллистической цели
    static const int    BG_WEIGHT_COEF  = 25;       /// Весовой коэффициент тратилового эквивалента БЦ

    /// Минимально и максимально возможные значения скорости
    static const float  MIN_SPEED       = 180.0;    /// Минимальное значение скорости
    static const float  MAX_SPEED       = 250.0;    /// Максимальное значение скорости

    /// Погрешности
    static const float  ERR_COURSE      = 10.0;     /// Погрешность измерения курса (град.)
    static const float  ERR_SPEED       = 30.0;     /// Погрешность измерения скорости (м/c)
};

}

#endif // TRACK

