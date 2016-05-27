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

    int nDangerousTrack;    /// Номер наиболее опасной трассы
    float time;             /// Время поражения наиболее опасной трассой
    float errTime;          /// Текущая погрешность времени поражения
    float sumErrTime;       /// Сумма квадратов погрешностей времени поражения
    float sigmaT;           /// Среднеквадратическаяпогрешность времени поражения
};

struct Track                /// Трасса
{
    int num;                /// Номер

    QPointF startPos;       /// Начальные координаты
    float modV;             /// Модуль вектора скорости
    float angV;             /// Курс (в радианах)

    QPointF pos;            /// Текущие координаты
    QPointF endPos;         /// Координаты экстраполированного конца траектории

    struct Target
    {
        float startDist;    /// Расстояние от начальной точки трассы до центра ПР

        float dist;         /// Текущее расстояние от трассы до центра ПР
        float time;         /// Время достижения ПР
        float angToV;       /// Угол между вектором скорости и прямой до центра ПР

        /// Точки касания угла видимости
        QPointF p1;
        QPointF p2;
    };
    QVector <Target> target;    /// Цели

//    int nNearTarget;        /// Номер ближайшего ПР
    int nFarTarget;         /// Номер наиболее удаленного ПР
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
