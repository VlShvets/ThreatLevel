#ifndef PAINTER_H
#define PAINTER_H

#include <QTimer>

#include "grapher2d.h"

#include "results.h"

namespace ThreatLevel
{

struct Base
{
    QPointF pos;        /// Координаты
    float radius;       /// Радиус
};

struct Track
{
    QPointF pos;        /// Координаты
    float modV;         /// Модуль вектора скорости
    float angV;         /// Курс (в радианах)

    struct Target
    {
        float dist;     /// Расстояние до центра базы

        QPointF p1, p2; /// Точки касания угла видимости
    };
    QVector <Target> target;    /// Цели
};

class Painter : public Grapher2D
{
    Q_OBJECT

public:
    Painter(class Results *_results, QWidget *_parent = 0);
    ~Painter();

    inline int getNumberOfBases();
    inline int getNumberOfTracks();
    inline int getSpeedFactor();
    inline int getTotalTime();

    inline Base baseAt(int _index) const;
    inline Track trackAt(int _index) const;

    inline Base & getBase(int _index);
    inline Track & getTrack(int _index);

public slots:
    inline void setNumberOfBases(int _nBase);
    inline void setNumberOfTracks(int _Track);
    inline void setSpeedFactor(int _speedFactor);
    inline void setTotalTime(int _totalTime);
    void resetTime();

protected:
    void paintEvent(QPaintEvent *_pEvent);

private:
    void initializationParOfBase();     /// Начальная инициализация параметров баз
    void initializationParOfTrack();    /// Начальная инициализация параметров трасс

    /// Вычисление точек касания
    static float getTanPoints(const QPointF *_track, const QPointF *_base,
                              const float _radius, QPointF *_p1, QPointF *_p2);

    /// Нормально распределенная случайная величина
    static float normalDistribution(float _mean, float _dev);

    class Results *results;

    float speedFactor;  /// Коэффициент скорости
    float totalTime;    /// Общее время моделирования (в секундах)
    float time;         /// Текущее время

    int nBase;      /// Количество баз
    int nTrack;     /// Количество трасс

    QVector <Base> base;    /// Базы
    QVector <Track> track;  /// Трассы

private slots:
    void timerEvent(QTimerEvent *);
};

int Painter::getNumberOfBases()
{
    return nBase;
}

int Painter::getNumberOfTracks()
{
    return nTrack;
}

int Painter::getSpeedFactor()
{
    return (int) speedFactor * 10;
}

int Painter::getTotalTime()
{
    return totalTime;
}

Base Painter::baseAt(int _index) const
{
    return base.at(_index);
}

Track Painter::trackAt(int _index) const
{
    return track.at(_index);
}

Base & Painter::getBase(int _index)
{
    return base[_index];
}

Track & Painter::getTrack(int _index)
{
    return track[_index];
}

void Painter::setNumberOfBases(int _nBase)
{
    nBase =_nBase;
}

void Painter::setNumberOfTracks(int _Track)
{
    nTrack = _Track;
}

void Painter::setSpeedFactor(int _speedFactor)
{
    speedFactor = (float) _speedFactor / 10.0;
}

void Painter::setTotalTime(int _totalTime)
{
    totalTime = _totalTime;
}

}

#endif // PAINTER_H
