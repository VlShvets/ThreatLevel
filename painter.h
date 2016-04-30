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
    const float DELTA = 0.1;    /// Константа времени

    Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent = 0);
    ~Painter();

    inline const Area & areaAt(int _index) const;
    inline const Track & trackAt(int _index) const;

    inline Area & getArea(int _index);
    inline Track & getTrack(int _index);

public slots:
    void reStart();

protected:
    void paintEvent(QPaintEvent *_pEvent);

private slots:
    void timerEvent(QTimerEvent *);

private:
    void loadAreaPar();         /// Загрузка параметров ПР
    void loadTrackPar();        /// Загрузка параметров трасс

    /// Вычисление расстояния от трассы до центра ПР
    static float calcDistance(const QPointF *_base, const QPointF *_track);

    /// Нормально распределенная случайная величина
    static float normalDistribution(float _mean, float _dev);

    /// Вычисление точек касания
    static void calcTanPoints(const QPointF *_track, const QPointF *_area,
                              const float _radius, QPointF *_p1, QPointF *_p2);

    AreaParameters *areaParameters;
    TrackParameters *trackParameters;
    Results *results;

    QVector <Area> area;    /// Позиционные районы
    QVector <Track> track;  /// Трассы
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

}

#endif // PAINTER_H
