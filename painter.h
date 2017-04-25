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

/// Класс виджета отрисовки трасс и позиционных районов
class Painter : public Grapher2D
{
    Q_OBJECT

public:
    explicit        Painter(AreaParameters *_areaParameters, TrackParameters *_trackParameters,
                            Results *_results, QWidget *_parent = 0);
    ~Painter();

    /// Установка таймера
    inline void     setIdTimer(int _idTimer);

    /// Возврат номера таймера
    inline int      getIdTimer()    const;

public slots:
    /// Перезапуск
    void            reStart();

protected:
    void            paintEvent(QPaintEvent *_pEvent);

private slots:
    void            timerEvent(QTimerEvent *);

private:
    /// Начальная инициализация параметров ПР
    void            initAreaPar();

    /// Начальная инициализация параметров трасс
    void            initTrackPar();

    /// Движение трасс и внесение погрешностей измерения
    void            movementOfTracks();

    /// Сглаживание погрешностей измерения
    void            smootheningOfMeasurement();

    /// Идентификация трасс с позиционными районами
    void            identificationOfTracksWithAreas();

    /// Расчет связанных со временем переменных
    void            calculationOfTime();

    /// Сброс трасс
    void            resetTracks();

    /// Определение номера трассы с минимальным временем преодоления расстояния до ПР путем сортировки
    int             numTrackOfMinTime();

    /// Расчет количества крылатых ракет
    void            calculationOfCMCount();

    /// Расчет количества баллистических целей
    void            calculationOfBGCount();

    /// Расчет количественного состава налёта с учетом тротилового эквивалента БЦ
    void            calculationOfRaidCount();

    /// Определение попадания в зону обнаружения
    bool            trackIsDetected(const Track &_track);

    /// Определение номера позиционного района по попаданию конечной точки траектории БЦ
    int             numAreaHitFinalPosOfBG(const Track &_track);

    /// Определение номера позиционного района ближайшего по курсу КР
    int             numAreaNearestOnCourseOfCM(const Track &_track);

    /// Быстрая сортировка трасс по времени поражения определенного ПР
    void            quickSortTracks(QVector <int> &_numTrack, const int _first, const int _last);

    /// --------------------------------------------------
    /// Статические функции
    /// --------------------------------------------------

    /// Вычисление расстояния между двумя точками
    static float            calcDistance(const QPointF &_p1, const QPointF &_p2);

    /// Вычисление точек соприкосновения касательных от текущего положения трассы до границы ПР
    static void             calcTanPoints(const QPointF &_area, const float _radius,
                                          const QPointF &_track, QPointF &_p1, QPointF &_p2);

    /// Равномерное распределение
    static float            uniformDistribution(const float _mean, const float _dev);

    /// Распределение Гаусса
    static float            gaussDistribution(const float _mean, const float _dev);

    /// Указатели на объекты классов
    AreaParameters      *areaParameters;    /// Класс редактирования параметров ПР
    TrackParameters     *trackParameters;   /// Класс редактирования параметров трасс
    Results             *results;           /// Класс отображения результатов

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Таймер
    int                 idTimer;    /// Номер таймера (-1 - нет таймера)

    /// Позиционные районы и трассы
    QMap <int, Area>    areas;      /// Словарь ПР <номер, структура параметров>
    QMap <int, Track>   tracks;     /// Словарь трасс <номер, структура параметров>

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Параметры отрисовки
    static const int    DEF_ZOOM            = 10;           /// Масштаб отображения по умолчанию
    static const float  ABS_MEASURE         = 10000.0;      /// Масштаб оси абсцисс
    static const float  ORD_MEASURE         = 10000.0;      /// Масштаб оси ординат
    static const float  LENGTH              = 100000.0;     /// Длина курса трасс до ассоциации с ПР

    /// Вычислительные параметры
    static const float  DELTA_T             = 10.0;         /// Интервал времени измерений
    static const float  SMOOTH              = 0.9;          /// Весовой коэфициент сглаживания погрешностей ( < 1.0)
    static const float  ACCURACY_TAN_POINT  = 1e-06;        /// Точность вычисления точек соприкосновения касательных
                                                            /// от текущего положения трассы до границы ПР
};

/// Установка таймера
void Painter::setIdTimer(int _idTimer)
{
    idTimer = _idTimer;
}

/// Возврат номера таймера
int Painter::getIdTimer() const
{
    return idTimer;
}

}

#endif // PAINTER_H
