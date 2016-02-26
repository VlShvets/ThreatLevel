#ifndef PAINTER_H
#define PAINTER_H

#include <QDockWidget>

#include "grapher2d.h"

class Painter : public Grapher2D
{
    const static int TOTALTIME = 50;
    const static float BASECOORDX = -40.0;
    const static float BASECOORDY = -40.0;
    const static float BASERADIUS = 10.0;
    const static float TRACKCOORDX = 0.0;
    const static float TRACKCOORDY = 0.0;
    const static float TRACKMODULEV = 1.0;
    const static float TRACKVECTORV = -M_PI_2;

public:
    Painter(QWidget *_parent = 0);
    ~Painter();

    int nBase;      /// Количество баз
    int nTracks;    /// Количество трасс

    struct Base         /// База
    {
        QPointF coord;  /// Координаты центра
        float radius;   /// Радиус
    };

    struct Track                    /// Трасса
    {
        QPointF coord;              /// Координаты цели
        float moduleV;              /// Модуль скорости цели
        float vectorV;              /// Направление скорости цели
        float angleBase;            /// Угол видимости базы
        float angelVtoBase;         /// Угол между вектором скорости цели и прямой до центра базы
        float distBase;             /// Расстояние до центра базы
        QPointF tangentPoints[2];   /// Точки касания
    };

protected:
    void timerEvent(QTimerEvent * _tEvent);
    void paintEvent(QPaintEvent * _pEvent);

private:
    float getTangentPoints(QPointF *_track, QPointF *_base,
                           float _radius, QPointF *_tangentPoints);   /// Вычисление точек касания

    QVector <Base> bases;
    QVector <Track> tracks;
};

#endif // PAINTER_H
