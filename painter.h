#ifndef PAINTER_H
#define PAINTER_H

#include "area.h"
#include "etalon.h"
#include "track.h"

#include <QTimer>

#include "Grapher2D.h"

namespace ThreatLevel
{

class AreaParameters;
class TrackParameters;
class Results;

/// Класс виджета отрисовки ЗКВ, эталонов и трасс
class Painter : public Grapher2D
{
public:
    explicit        Painter(QWidget *_parent = 0);
    ~Painter();

    /// Установление ЗКВ
    inline void     setAreas(const  QMap <int, Area> *_areas);

    /// Установление эталонов
    inline void     setEtalons(QMap <int, Etalon> *_etalons);

    /// Установление трасс
    inline void     setTracks(QMap <int, Track> *_tracks);

    /// Очищение отрисовки
    void            clearing();

protected:
    /// Обновление отрисокви
    void            timerEvent(QTimerEvent *);

    /// Отрисовка ЗКВ и трасс
    void            paintEvent(QPaintEvent *_pEvent);

private:
    /// --------------------------------------------------
    /// Статические функции
    /// --------------------------------------------------

    /// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ЗКВ
    static void     calcTanPoints(const QPointF &_area, const float _radius,
                                  const QPointF &_track, QPointF &_p1, QPointF &_p2);

    /// Вычисление расстояния между двумя точками
    static float    calcDistance(const QPointF &_p1, const QPointF &_p2);

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Указатели на словари параметров ЗКВ, эталонов и трасс
    const QMap <int, Area>  *areas;     /// Словарь ЗКВ         <номер, структура параметров>
    QMap <int, Etalon>      *etalons;   /// Словарь эталонов    <номер, структура параметров>
    QMap <int, Track>       *tracks;    /// Словарь трасс       <номер, структура параметров>

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    static const int    PAINTER_INTERVAL    = 100;          /// Интервал обновления отрисовки

    /// Параметры отрисовки
    static const int    DEF_ZOOM            = 10;           /// Масштаб отображения по умолчанию
    static constexpr float  ABS_MEASURE         = 10000.0;      /// Масштаб оси абсцисс
    static constexpr float  ORD_MEASURE         = 10000.0;      /// Масштаб оси ординат
    static constexpr float  LENGTH_COURSE       = 100000.0;     /// Длина курса эталонов

    /// Вычислительные параметры
    static constexpr float  ACCURACY_TAN_POINT  = 1e-06;        /// Точность вычисления точек соприкосновения касательных
                                                            /// от текущего положения трассы до границы ПР
};

/// Установление ЗКВ
void Painter::setAreas(const QMap <int, Area> *_areas)
{
    areas = _areas;
}

/// Установление эталонов
void Painter::setEtalons(QMap <int, Etalon> *_etalons)
{
    etalons = _etalons;
}

/// Установление трасс
void Painter::setTracks(QMap <int, Track> *_tracks)
{
    tracks = _tracks;
}

}

#endif // PAINTER_H
