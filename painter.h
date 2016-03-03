#ifndef PAINTER_H
#define PAINTER_H

#include <QTimer>
#include <QQueue>

#include "grapher2d.h"

#include "results.h"

#define LENGTHARROW 30  /// Длина отображаемой стрелки вектора скорости в пикселях

class Painter : public Grapher2D
{
    Q_OBJECT

public:
    Painter(class Results *_results, QWidget *_parent = 0);
    ~Painter();

    int nBase;      /// Количество баз
    int nTrack;     /// Количество трасс

    int sizeOfMemory;   /// Длина следа

    /// Переменные, связанные со временем
    QTimer *tTime;      /// Таймер времени
    float totalTime;    /// Общее время моделирования (в секундах)
    float time;         /// Текущее время

    struct Base
    {
        QPointF pos;        /// Координаты
        float radius;       /// Радиус
    };
    QVector <Base> base;    /// Базы

    struct Track
    {
        QPointF pos;        /// Координаты
        float modV;         /// Модуль вектора скорости
        float angV;         /// Угол вектора скорости от оси ординат

        struct Target
        {
            float time;     /// Время достижения цели
            float dist;     /// Расстояние до центра базы
            QPointF p1, p2; /// Точки касания
        };
        QVector <Target> target;    /// Цели
    };
    QVector <Track> track;  /// Трассы

protected:
    void paintEvent(QPaintEvent * _pEvent);

private slots:
    void timerOut();

private:
    void initializationParOfBase();     /// Начальная инициализация параметров баз
    void initializationParOfTrack();    /// Начальная инициализация параметров трасс

    /// Вычисление точек касания
    float getTanPoints(const QPointF *_track, const QPointF *_base,
                       const float _radius, QPointF *_p1, QPointF *_p2);

    class Results *results;
};

#endif // PAINTER_H
