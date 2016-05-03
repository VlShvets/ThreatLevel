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
    const float DELTAT = 0.1;    /// Константа времени

    /// Погрешности
    const float DEVIATION = 0.2;    /// Среднеквадратическое отклонение
    const float ERRPOS = 1000.0;    /// Погрешность координаты
    const float ERRMODV = 100.0;    /// Погрешность модуля скорости
    const float ERRANGV = 15.0;     /// Погрешность курса

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
    void loadEtalonPar();       /// Загрузка параметров эталонов

    /// Вычисление расстояния от трассы до центра ПР
    static float calcDistance(const QPointF *_base, const QPointF *_track, const float _radius);

    /// Нормально распределенная случайная величина
    static float normalDistribution(float _mean, float _dev);

    /// Вычисление точек касания
    static void calcTanPoints(const QPointF *_track, const QPointF *_area,
                              const float _radius, QPointF *_p1, QPointF *_p2);

    AreaParameters *areaParameters;
    TrackParameters *trackParameters;
    Results *results;

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
