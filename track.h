#ifndef TRACK
#define TRACK

#include <QPointF>

namespace ThreatLevel
{

/// Структура параметров трасс
struct Track
{
    bool                isBG;                           /// Признак баллистической цели

    /// Начальные параметры
    QPointF             initStartPos;                   /// Координаты точки старта
    QPointF             initFinalPos;                   /// Координаты конечной точки
    float               initCourse;                     /// Начальный курс в радианах
    float               initSpeed;                      /// Начальная скорость (может принимать отрицательные значения)
    float               initAcceleration;               /// Начальное ускорение (может принимать отрицательные значения)
    int                 initQuant;                      /// Количественный состав (1 - 15)

    /// Текущие параметры
    QPointF             pos;                            /// Текущие координаты
    float               speed;                          /// Текущая скорость (может принимать отрицательные значения)

    /// Инициируемые при обнаружении трассы параметры
    QPointF             startPos;                       /// Координаты начальной точки траектории
    QPointF             finalPos;                       /// Координаты экстраполированного конца траектории
    int                 amtMeasure;                     /// Количество измерений

    /// Параметры с погрешностью
    float               errCourse;                      /// Текущий курс в радианах с погрешностью
    float               errSpeed;                       /// Текущая скорость с погрешностью

    /// Параметры сглаживания погрешностей
    float               errVx;                          /// Проекция вектора скорости с погрешностью на ось абсцисс
    float               errVy;                          /// Проекция вектора скорости с погрешностью на ось ординат

    /// Связанные с ассоциированным позиционным районом параметры
    int                 numArea;                        /// Номер ПР (-1 - ни с каким ПР трасса не ассоциированна)
    float               startDist;                      /// Расстояние от начальной точки траектории до центра ПР
    float               angCourseToPA;                  /// Угол между курсом и прямой от начальной точки траектории до центра ПР
    float               errAngCourseToPA;               /// Угол между курсом и прямой от начальной точки траектории до центра ПР с погрешностью

    /// Точки соприкосновения касательных от текущего положения трассы до границы ПР
    QPointF             tanPoint1;
    QPointF             tanPoint2;

    /// Текущее расстояние между проекцией трассы на прямую от начальной точки траектории до центра ПР и центром ПР
    float               dist;                           /// Текущее расстояние
    float               errDist;                        /// Текущее расстояние с погрешностью

    /// Результирующие параметры
    float               time;                           /// Время поражения ПР
    float               errTime;                        /// Время поражения ПР с погрешностью

    /// Общие для всех трасс параметры
    static int          numTrackMinErrTime;             /// Номер трассы с минимальным временем поражения ПР с погрешностью

    /// Общие для всех трасс константы
    static const int    BG_WEIGHT_COEF      = 25;       /// Весовой коэффициент тратилового эквивалента баллистической цели

    /// Минимально и максимально возможные значения скорости
    static const float  MIN_SPEED           = 180.0;    /// Минимальное значение скорости
    static const float  MAX_SPEED           = 250.0;    /// Максимальное значение скорости

    /// Погрешности
    static const float  ERR_COURSE          = 10.0;     /// Погрешность курса
    static const float  ERR_SPEED           = 30.0;     /// Погрешность скорости
};

}

#endif // TRACK

