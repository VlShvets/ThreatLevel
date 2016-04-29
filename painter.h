#ifndef PAINTER_H
#define PAINTER_H

#include <QTimer>

#include "Grapher2D.h"

#include "areaparameters.h"
#include "trackparameters.h"
#include "results.h"

namespace ThreatLevel
{

const float DELTA = 0.1;    /// Константа времени

class AreaParameters;
class TrackParameters;
class Results;

struct Area
{
    QPointF pos;            /// Координаты
    float radius;           /// Радиус
};

struct Track
{
    QPointF pos;            /// Координаты
    float modV;             /// Модуль вектора скорости
    float angV;             /// Курс (в радианах)

    struct Target
    {
        float dist;         /// Расстояние от трассы до центра ПР
        float angToV;       /// Угол между вектором скорости и прямой до центра ПР

        /// Точки касания угла видимости
        QPointF p1;
        QPointF p2;
    };
    QVector <Target> target;    /// Цели

    Target *nearTarget;     /// Ближайший ПР
};

class Painter : public Grapher2D
{
    Q_OBJECT

public:
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

    /// Вычисление точек касания
    static float calcTanPoints(const QPointF *_track, const QPointF *_base,
                              const float _radius, QPointF *_p1, QPointF *_p2);

    /// Нормально распределенная случайная величина
    static float normalDistribution(float _mean, float _dev);

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
