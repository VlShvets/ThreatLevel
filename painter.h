#ifndef PAINTER_H
#define PAINTER_H

#include <QTimer>

#include "Grapher2D.h"

#include "settingarea.h"
#include "settingtrack.h"
#include "results.h"

namespace ThreatLevel
{

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
    Painter(class SettingArea *_settingArea, class SettingTrack *_settingTrack, class Results *_results, QWidget *_parent = 0);
    ~Painter();

    inline int getSpeedFactor();
    inline int getTotalTime();

    inline const Area & areaAt(int _index) const;
    inline const Track & trackAt(int _index) const;

    inline Area & getArea(int _index);
    inline Track & getTrack(int _index);

public slots:
    inline void setSpeedFactor(int _speedFactor);
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

    class SettingArea *settingArea;
    SettingTrack *settingTrack;
    Results *results;

    float speedFactor;  /// Коэффициент скорости
    float totalTime;    /// Общее время моделирования (в секундах)
    float time;         /// Текущее время

    int nArea;      /// Количество позиционных районов
    int nTrack;     /// Количество трасс

    QVector <Area> area;    /// Позиционные районы
    QVector <Track> track;  /// Трассы
};

int Painter::getSpeedFactor()
{
    return (int) speedFactor * 10;
}

int Painter::getTotalTime()
{
    return totalTime;
}

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
