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

    QPointF pos;            /// Координаты центра
    float radius;           /// Радиус
    float critTime;         /// Критическое время

    float sumDiffTime;      /// Сумма квадратов погрешностей времени поражения
    float sigmaT;           /// Среднеквадратическая погрешность времени поражения

    struct Target
    {
        int num;            /// Номер цели

        int countMeasure;   /// Количество измерений
        float startDist;    /// Расстояние от центра ПР до начальной точки трассы

        float dist;         /// Текущее расстояние от центра ПР до проекции цели
        float errDist;      /// Текущее расстояние от центра ПР до проекции цели с погрешностью

        float time;         /// Время поражения ПР
        float errTime;      /// Время поражения ПР с погрешностью

        float angToV;       /// Угол между прямой от центра ПР до цели и вектором скорости цели
        float errAngToV;    /// Угол между прямой от центра ПР до цели и вектором скорости цели с погрешностью

        /// Точки касания угла видимости
        QPointF p1;
        QPointF p2;
    };
    QVector <Target> target;    /// Цели
};

struct Track                /// Трасса
{
    int num;                /// Номер

    QPointF startPos;       /// Начальные координаты
    QPointF pos;            /// Текущие координаты    
    QPointF endPos;         /// Координаты экстраполированного конца траектории по ближайшему по времени с погрешностью ПР

    float modV;             /// Модуль вектора скорости
    float angV;             /// Курс (в радианах)
    float boost;            /// Ускорение (может принимать отрицательные значения)

    float errModV;          /// Модуль вектора скорости с погрешностью
    float errAngV;          /// Курс с погрешностью (в радианах)

    float errVx;            /// Проекция вектора скорости с погрешностью на ось Х
    float errVy;            /// Проекция вектора скорости с погрешностью на ось Y
};

class Results : public QWidget
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = 0);
    ~Results();

    void loadTable(const QVector <Area> &_area);

private:
    QTableWidget *tResults;     /// Таблица результатов
};

}

#endif // RESULTS_H
