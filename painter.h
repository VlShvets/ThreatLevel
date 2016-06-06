#ifndef PAINTER_H
#define PAINTER_H

#include <QTimer>

#include "Grapher2D.h"

#include "areaparameters.h"
#include "trackparameters.h"
#include "results.h"

namespace ThreatLevel
{

class AreaParameters;
class TrackParameters;
class Results;

class Painter : public Grapher2D
{
    Q_OBJECT

public:
    const float DELTAT = 10.0;      /// Константа времени

    /// Погрешности
    const float ERRMODV = 30.0;     /// Погрешность модуля скорости
    const float ERRANGV = 10.0;     /// Погрешность курса
    const float WEIGHT = 0.9;       /// Весовой коэфициент ( < 1.0)

    Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent = 0);
    ~Painter();

    inline const Area & areaAt(int _index) const;
    inline const Track & trackAt(int _index) const;

    inline Area & getArea(int _index);
    inline Track & getTrack(int _index);

    inline void setIdTimer(int _idTimer);
    inline int getIdTimer();

public slots:
    void reStart();

protected:
    void paintEvent(QPaintEvent *_pEvent);

private slots:
    void timerEvent(QTimerEvent *_tEvent);

private:
    void loadAreaPar();         /// Начальная инициализация параметров ПР
    void loadTrackPar();        /// Начальная инициализация параметров трасс

    /// Вычисление расстояния между двумя точками
    static float calcDistance(const QPointF *_p1, const QPointF *_p2);

    /// Распределение Гаусса
    static float gaussDistribution(float _mean, float _dev);

    /// Равномерное распределение
    static float uniformDistribution(float _mean, float _dev);

    /// Вычисление точек касания
    static void calcTanPoints(const QPointF *_track, const QPointF *_area,
                              const float _radius, QPointF *_p1, QPointF *_p2);

    AreaParameters *areaParameters;
    TrackParameters *trackParameters;
    Results *results;

    int idTimer;                /// Номер таймера (-1 - нет таймера)

    QVector <Area> area;        /// Позиционные районы
    QVector <Track> track;      /// Трассы
};

const Area & Painter::areaAt(int _index) const
{
    return area.at(_index);
}

const Track & Painter::trackAt(int _index) const
{
    return track.at(_index);
}

Area &Painter::getArea(int _index)
{
    return area[_index];
}

Track & Painter::getTrack(int _index)
{
    return track[_index];
}

void Painter::setIdTimer(int _idTimer)
{
    idTimer = _idTimer;
}

int Painter::getIdTimer()
{
    return idTimer;
}

}

#endif // PAINTER_H
