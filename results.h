#ifndef RESULTS_H
#define RESULTS_H

#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>

namespace ThreatLevel
{

struct Track                /// Трасса
{
    int num;                /// Номер
    QPointF pos;            /// Координаты
    float modV;             /// Модуль вектора скорости
    float angV;             /// Курс (в радианах)
    QPointF endPos;         /// Координаты экстраполированного конца траектории

    struct Target
    {
        float dist;         /// Расстояние от трассы до границы ПР
        float time;         /// Время достижения ПР
        float angToV;       /// Угол между вектором скорости и прямой до центра ПР

        /// Точки касания угла видимости
        QPointF p1;
        QPointF p2;
    };
    QVector <Target> target;    /// Цели

    int nNearTarget;        /// Номер ближайшего ПР
    int nFarTarget;         /// Номер наиболее удаленного ПР
};

struct Area                 /// Позиционный район
{
    int num;                /// Номер
    QPointF pos;            /// Координаты
    float radius;           /// Радиус

    int nDangerousTrack;    /// Номер наиболее опасная трасса
};

class Results : public QWidget
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = 0);
    ~Results();

    void loadTable(const QVector <Area> *_area, const QVector <Track> *_track, const QVector <Track> *_etalon);

private:
    QTableWidget *tResults;     /// Таблица результатов
};

}

#endif // RESULTS_H
