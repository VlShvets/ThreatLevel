#ifndef PAINTER_H
#define PAINTER_H

#include "parametersofareas.h"
#include "parametersofetalons.h"
#include "results.h"

#include <QTimer>

#include "Grapher2D.h"

namespace ThreatLevel
{

class AreaParameters;
class TrackParameters;
class Results;

/// Класс виджета отрисовки эталонов, трасс и ЗКВ
class Painter : public Grapher2D
{
    Q_OBJECT

public:
    explicit    Painter(QWidget *_parent = 0);
    ~Painter();

protected:
    void        paintEvent(QPaintEvent *_pEvent);

private:
    /// Начальная инициализация параметров трасс
    void        initTrackPar();

    /// --------------------------------------------------
    /// Статические функции
    /// --------------------------------------------------

    /// Вычисление расстояния между двумя точками
    static float    calcDistance(const QPointF &_p1, const QPointF &_p2);

    /// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
    static void     calcTanPoints(const QPointF &_area, const float _radius,
                                  const QPointF &_track, QPointF &_p1, QPointF &_p2);

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Позиционные районы и трассы
    QMap <int, Area>    areas;      /// Словарь ПР      <номер, структура параметров>
    QMap <int, Track>   tracks;     /// Словарь трасс   <номер, структура параметров>

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Параметры отрисовки
    static const int    DEF_ZOOM            = 10;           /// Масштаб отображения по умолчанию
    static const float  ABS_MEASURE         = 10000.0;      /// Масштаб оси абсцисс
    static const float  ORD_MEASURE         = 10000.0;      /// Масштаб оси ординат
    static const float  LENGTH              = 100000.0;     /// Длина курса трасс до ассоциации с ПР

    /// Вычислительные параметры
    static const float  ACCURACY_TAN_POINT  = 1e-06;        /// Точность вычисления точек соприкосновения касательных
                                                            /// от текущего положения трассы до границы ПР
};

}

#endif // PAINTER_H
