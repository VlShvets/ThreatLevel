#ifndef PAINTER_H
#define PAINTER_H

#include "area.h"
#include "etalon.h"
#include "track.h"

#include "Grapher2D.h"

namespace ThreatLevel
{

class AreaParameters;
class TrackParameters;
class Results;

/// Класс виджета отрисовки ЗКВ, эталонов и трасс
class Painter : public Grapher2D
{
    Q_OBJECT

public:
    explicit        Painter(QWidget *_parent = 0);
    ~Painter();

    /// Установление ЗКВ
    inline void     setAreas(const  QMap <int, Area> &_areas);

    /// Установление эталонов
    inline void     setEtalons(const QMap <int, Etalon> &_etalons);

    /// Установление трасс
    inline void     setTracks(const QMap <int, Track> &_tracks);

    /// Очищение отрисовки
    void            clearing();

public slots:
    /// Установление флага отображения ЗКВ
    inline void     setVisibleOfAreas(bool _isVisibleOfAreas);

    /// Установление флага отображения эталонов
    inline void     setVisibleOfEtalons(bool _isVisibleOfEtalons);

    /// Установление флага отображения трасс
    inline void     setVisibleOfTracks(bool _isVisibleOfTracks);

    /// Установление флага отображения кратчайших расстояний
    inline void     setVisibleOfMinDists(bool _isVisibleOfMinDists);

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

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Указатели на словари параметров ЗКВ, эталонов и трасс
    const QMap <int, Area>      *areas;             /// Словарь ЗКВ         <номер, структура параметров>
    const QMap <int, Etalon>    *etalons;           /// Словарь эталонов    <номер, структура параметров>
    const QMap <int, Track>     *tracks;            /// Словарь трасс       <номер, структура параметров>

    /// Флаги отображения ЗКВ, эталонов, трасс и кратчайших расстояний
    bool    isVisibleOfAreas;       /// Флаг отображения ЗКВ
    bool    isVisibleOfEtalons;     /// Флаг отображения эталонов
    bool    isVisibleOfTracks;      /// Флаг отображения трасс
    bool    isVisibleOfMinDists;    /// Флаг отображения кратчайших расстояний

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Параметры обновления отрисовки
    static const int    PAINTER_INTERVAL    = 100;          /// Интервал обновления отрисовки (в мс)

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
void Painter::setAreas(const QMap <int, Area> &_areas)
{
    areas = &_areas;
}

/// Установление эталонов
void Painter::setEtalons(const QMap <int, Etalon> &_etalons)
{
    etalons = &_etalons;
}

/// Установление трасс
void Painter::setTracks(const QMap <int, Track> &_tracks)
{
    tracks = &_tracks;
}

/// Установление флага отображения ЗКВ
void Painter::setVisibleOfAreas(bool _isVisibleOfAreas)
{
    isVisibleOfAreas = _isVisibleOfAreas;
}

/// Установление флага отображения эталонов
void Painter::setVisibleOfEtalons(bool _isVisibleOfEtalons)
{
    isVisibleOfEtalons = _isVisibleOfEtalons;
}

/// Установление флага отображения трасс
void Painter::setVisibleOfTracks(bool _isVisibleOfTracks)
{
    isVisibleOfTracks = _isVisibleOfTracks;
}

/// Установление флага отображения кратчайших расстояний
void Painter::setVisibleOfMinDists(bool _isVisibleOfMinDists)
{
    isVisibleOfMinDists = _isVisibleOfMinDists;
}

}

#endif // PAINTER_H
