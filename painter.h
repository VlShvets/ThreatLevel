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
    const float DELTAT = 1.0;      /// Константа времени

    /// Погрешности
    const float ERRMODV = 30.0;    /// Погрешность модуля скорости
    const float ERRANGV = 7.0;     /// Погрешность курса

    Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent = 0);
    ~Painter();

    inline const Area & areaAt(int _index) const;
    inline const Track & trackAt(int _index) const;
    inline const Track & etalonAt(int _index) const;

    inline Area & getArea(int _index);
    inline Track & getTrack(int _index);
    inline Track & getEtalon(int _index);

public slots:
    void reStart();

protected:
    void paintEvent(QPaintEvent *_pEvent);

private slots:
    void timerEvent(QTimerEvent *);

private:
    void loadAreaPar();         /// Начальная инициализация параметров ПР
    void loadTrackPar();        /// Начальная инициализация параметров трасс
    void loadEtalonPar();       /// Начальная инициализация параметров эталонов

    /// Вычисление расстояния между двумя точками
    static float calcDistance(const QPointF *_p1, const QPointF *_p2);

    /// Нормально распределенная случайная величина
    static float normalDistribution(float _mean, float _dev);

    /// Вычисление точек касания
    static void calcTanPoints(const QPointF *_track, const QPointF *_area,
                              const float _radius, QPointF *_p1, QPointF *_p2);

    AreaParameters *areaParameters;
    TrackParameters *trackParameters;
    Results *results;

    int time;

    QVector <Area> area;        /// Позиционные районы
    QVector <Track> track;      /// Трассы
    QVector <Track> etalon;     /// Эталоны
};

const Area & Painter::areaAt(int _index) const
{
    return area.at(_index);
}

const Track & Painter::trackAt(int _index) const
{
    return track.at(_index);
}

const Track & Painter::etalonAt(int _index) const
{
    return etalon.at(_index);
}

Area &Painter::getArea(int _index)
{
    return area[_index];
}

Track & Painter::getTrack(int _index)
{
    return track[_index];
}

Track & Painter::getEtalon(int _index)
{
    return etalon[_index];
}

}

#endif // PAINTER_H
