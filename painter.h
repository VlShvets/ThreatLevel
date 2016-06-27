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

    /// Погрешности
    const float ERRMODV = 30.0;     /// Погрешность модуля скорости
    const float ERRANGV = 10.0;     /// Погрешность курса

    Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters, Results *_results, QWidget *_parent = 0);
    ~Painter();

    inline void setIdTimer(int _idTimer);
    inline int getIdTimer() const;

public slots:
    void reStart();

protected:
    void paintEvent(QPaintEvent *_pEvent);

private slots:
    void timerEvent(QTimerEvent *_tEvent);

private:
    void loadAreaPar();         /// Начальная инициализация параметров ПР
    void loadTrackPar();        /// Начальная инициализация параметров трасс
    void calcStaticPar();       /// Вычисление неизменяющихся параметров

    /// Быстрая сортировка целей по времени поражения с погрешностью определенного ПР
    void quickSortTargets(const int _numArea, const int _first, const int _last);

    /// Перестановка двух трасс в списке целей определенного ПР
    void swapTargets(const int _numArea, const int _numTarget1, const int _numTarget2);

    /// Вычисление расстояния между двумя точками
    static float calcDistance(const QPointF &_p1, const QPointF &_p2);

    /// Вычисление координат экстраполированного конца траектории
    static const QPointF calcEndPos(const QPointF &_startPos, const float _startDist, const float _errAngToV,
                                    const float _errModV,const float _critTime, const float _errAngV);

    /// Вычисление точек касания
    static void calcTanPoints(const QPointF &_area, const QPointF &_track,
                              const float _radius, QPointF &_p1, QPointF &_p2);

    /// Равномерное распределение
    static float uniformDistribution(const float _mean, const float _dev);

    /// Распределение Гаусса
    static float gaussDistribution(const float _mean, const float _dev);

    AreaParameters *areaParameters;
    TrackParameters *trackParameters;
    Results *results;

    int idTimer;                /// Номер таймера (-1 - нет таймера)

    QVector <Area> area;        /// Позиционные районы
    QVector <Track> track;      /// Трассы
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
