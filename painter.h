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
        float dist;         /// Расстояние до центра позиционного района
        float angToV;       /// Угол между вектором скорости и прямой до центра позиционного района

        /// Точки касания угла видимости
        QPointF p1;
        QPointF p2;
    };
    QVector <Target> target;    /// Цели
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

    inline int getTotalTime();

public slots:
    inline void setTotalTime(int _totalTime);
    void resetTime();

protected:
    void paintEvent(QPaintEvent *_pEvent);

private slots:
    void timerEvent(QTimerEvent *);

private:
    void loadAreaPar();     /// Загрузка параметров позиционных районов
    void loadTrackPar();    /// Загрузка параметров трасс

    /// Вычисление точек касания
    static float calcTanPoints(const QPointF *_track, const QPointF *_base,
                              const float _radius, QPointF *_p1, QPointF *_p2);

    /// Нормально распределенная случайная величина
    static float normalDistribution(float _mean, float _dev);

    AreaParameters *areaParameters;
    TrackParameters *trackParameters;
    Results *results;

    float time;         /// Текущее время
    float totalTime;    /// Общее время моделирования (в секундах)

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

int Painter::getTotalTime()
{
    return totalTime;
}

void Painter::setTotalTime(int _totalTime)
{
    totalTime = _totalTime;
}

}

#endif // PAINTER_H
