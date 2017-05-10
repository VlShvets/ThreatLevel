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

/// Класс виджета отрисовки трасс и позиционных районов
class Painter : public Grapher2D
{
    Q_OBJECT

public:
    explicit        Painter(ParametersOfAreas *_parametersOfAreas, Results *_results, QWidget *_parent = 0);
    ~Painter();

public slots:
    /// Перезапуск
    void            reStart();

protected:
    void            paintEvent(QPaintEvent *_pEvent);

private slots:
    void            timerEvent(QTimerEvent *);

private:
    /// Начальная инициализация параметров ПР
    void            initAreaPar();

    /// Начальная инициализация параметров трасс
    void            initTrackPar();

    /// --------------------------------------------------
    /// Статические функции
    /// --------------------------------------------------

    /// Вычисление расстояния между двумя точками
    static float            calcDistance(const QPointF &_p1, const QPointF &_p2);

    /// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
    static void             calcTanPoints(const QPointF &_area, const float _radius,
                                          const QPointF &_track, QPointF &_p1, QPointF &_p2);

    /// Указатели на объекты классов
    class ParametersOfAreas   *parametersOfAreas;     /// Класс редактирования параметров областей
    Results             *results;               /// Класс отображения результатов

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Таймер
    int                 idTimer;    /// Номер таймера (-1 - нет таймера)

    /// Позиционные районы и трассы
    QMap <int, Area>    areas;      /// Словарь ПР <номер, структура параметров>
    QMap <int, Track>   tracks;     /// Словарь трасс <номер, структура параметров>

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Параметры отрисовки
    static const int    DEF_ZOOM            = 10;           /// Масштаб отображения по умолчанию
    static constexpr float  ABS_MEASURE         = 10000.0;      /// Масштаб оси абсцисс
    static constexpr float  ORD_MEASURE         = 10000.0;      /// Масштаб оси ординат
    static constexpr float  LENGTH              = 100000.0;     /// Длина курса трасс до ассоциации с ПР

    /// Вычислительные параметры
    static constexpr float  ACCURACY_TAN_POINT  = 1e-06;        /// Точность вычисления точек соприкосновения касательных
                                                            /// от текущего положения трассы до границы ПР
};

}

#endif // PAINTER_H
