#ifndef RESULTS_H
#define RESULTS_H

#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>

namespace ThreatLevel
{

struct Area                 /// Позиционный район
{
    int num;                /// Номер

    QPointF pos;            /// Координаты
    float radius;           /// Радиус
    float critTime;         /// Критическое время

    int nDangerTrack;       /// Номер наиболее опасной трассы

    float diffTime;         /// Текущая погрешность времени поражения
    float sumDiffTime;      /// Сумма квадратов погрешностей времени поражения
    float sigmaT;           /// Среднеквадратическаяпогрешность времени поражения
};

struct Track                /// Трасса
{
    int num;                /// Номер

    QPointF startPos;       /// Начальные координаты
    QPointF pos;            /// Текущие координаты
    QPointF endPos;         /// Координаты экстраполированного конца траектории

    float modV;             /// Модуль вектора скорости
    float errModV;          /// Модуль вектора скорости с погрешностью

    float angV;             /// Курс (в радианах)
    float errAngV;          /// Курс с погрешностью (в радианах)

    float errVx;            /// Проекция вектора скорости с погрешностью на ось Х
    float errVy;            /// Проекция вектора скорости с погрешностью на ось Y

    int nNearTarget;        /// Номер ближайшего ПР

    struct Target
    {
        float startDist;    /// Расстояние от начальной точки трассы до центра ПР

        float dist;         /// Текущее расстояние от трассы до центра ПР
        float errDist;      /// Текущее расстояние от трассы до центра ПР с погрешностью

        float time;         /// Время достижения ПР
        float errTime;      /// Время достижения ПР с погрешностью

        float angToV;       /// Угол между вектором скорости и прямой до центра ПР
        float errAngToV;    /// Угол между вектором скорости и прямой до центра ПР с погрешностью
        float maxAngToV;    /// Максимально возможный угол между вектором скорости и прямой до центра ПР

        int nTakeP;         /// Количество взятых для рассчетов точек
        int nPassP;         /// Количество пропущенных точек

        /// Точки касания угла видимости
//        QPointF p1;
//        QPointF p2;
    };
    QVector <Target> target;    /// Цели
};

class Results : public QWidget
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = 0);
    ~Results();

    void loadTable(const QVector <Area> *_area, const QVector <Track> *_track);

private:
    QTableWidget *tResults;     /// Таблица результатов
};

}

#endif // RESULTS_H
