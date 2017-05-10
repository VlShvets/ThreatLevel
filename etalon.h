#ifndef ETALON
#define ETALON

#include <QPointF>

namespace ThreatLevel
{

/// Структура параметров эталонов
struct Etalon
{
    /// Начальные параметры
    bool        initIsBG;               /// Признак БЦ
    QPointF     initStartPos;           /// Координаты начала движения
    float       initSpeed;              /// Начальная скорость
    QPointF     initFinalPos;           /// Координаты конечной точки
    float       initCourse;             /// Начальный курс в радианах
    int         initQuant;              /// Количественный состав (для КР: может принимать значения 1 - 15)

    /// Точные значения текущих параметров
    QPointF     exactPos;               /// Точные значения текущих координат

    /// Параметры измерения с погрешностью
    float       measCourse;             /// Текущий измеренный курс в радианах с погрешностью
    float       measSpeed;              /// Текущая измеренная скорость с погрешностью
};

}

#endif // ETALON
