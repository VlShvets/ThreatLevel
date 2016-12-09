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
    const float WEIGHT = 0.9;       /// Весовой коэфициент сглаживания ( < 1.0)

    Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent = 0);
    ~Painter();

    inline void setIdTimer(int _idTimer);
    inline int getIdTimer() const;

public slots:
    void reStart();

protected:
    void paintEvent(QPaintEvent *_pEvent);

private slots:
    void timerEvent(QTimerEvent *);

private:
    void initAreaPar();         /// Начальная инициализация параметров ПР
    void initTrackPar();        /// Начальная инициализация параметров трасс

    int numOnCourseMinDistanceArea(const Track &_track);    /// Определение для трассы номера ближайшего по курсу позиционного района

    /// Быстрая сортировка целей по времени поражения с погрешностью определенного ПР
    void quickSortTracks(const int _numArea, const int _first, const int _last);

    /// Вычисление расстояния между двумя точками
    static float calcDistance(const QPointF &_p1, const QPointF &_p2);

    /// Вычисление координат экстраполированного конца траектории
    static const QPointF calcEndPos(const QPointF &_startPos, const float _startDist, const float _errAngCourseToPA,
                                    const float _errSpeed, const float _initCritTime, const float _errCourse);

    /// Вычисление точек касания
    static void calcTanPoints(const QPointF &_area, const float _radius,
                              const QPointF &_track, QPointF &_p1, QPointF &_p2);

    /// Равномерное распределение
    static float uniformDistribution(const float _mean, const float _dev);

    /// Распределение Гаусса
    static float gaussDistribution(const float _mean, const float _dev);

    QVector <Area> area;        /// Позиционные районы
    QVector <Track> track;      /// Трассы

    int idTimer;                /// Номер таймера (-1 - нет таймера)

    AreaParameters *areaParameters;
    TrackParameters *trackParameters;
    Results *results;
};

void Painter::setIdTimer(int _idTimer)
{
    idTimer = _idTimer;
}

int Painter::getIdTimer() const
{
    return idTimer;
}

}

#endif // PAINTER_H
