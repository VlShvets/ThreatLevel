#ifndef GRAPHOFTRACKSCOUNT_H
#define GRAPHOFTRACKSCOUNT_H

#include "area.h"

#include <QMutex>

#include "Grapher2D.h"

namespace ThreatLevel
{

/// Класс виджета графика количественного состава налёта
class GraphOfTracksCount : public Grapher2D
{
public:
    explicit        GraphOfTracksCount(QWidget *_parent = 0);
    ~GraphOfTracksCount();

    /// Установление ЗКВ
    inline void     setAreas(const  QMap <int, Area> &_areas);

    /// Выбор ЗКВ для отображения
    inline void     showGraph(const int _numArea);

    /// Очистка графика количественного состава налёта
    void            resetGraph();

    /// Загрузка количественного состава налета
    void            loadTracksCount();

protected:
    /// Добавление количественного состава налета
    void            addTrackCount(const int _num, const int _count);

    /// Обновление отрисокви
    void            timerEvent(QTimerEvent *);

    /// Отрисовка графика количественного состава налета
    void            paintEvent(QPaintEvent *_pEvent);

private:
    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Указатель на словарь параметров ЗКВ
    const QMap <int, Area>      *areas;         /// Словарь ЗКВ     <номер, структура параметров>

    /// Переменные отрисовки
    int                         numArea;        /// Выбранный для отображения в данный момент времени ЗКВ
    QMutex                      mutex;          /// Мьютекс для синхронизации обращений к tracksCount
    QMap <int, QVector <int> >  tracksCount;    /// Список количественного состава налета во времени

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Параметры ограничения количества данных
    static const int    MAX_SUM_TRACKS  = 1000;     /// Максимальный размер списка количественного состава налета

    /// Параметры обновления графика
    static const int    GRAPH_INTERVAL  = 1000;     /// Интервал обновления графика (в мс)

    /// Параметры отрисовки
    static const int    WIDTH           = 3;        /// Ширина линии графика количественного состава налета
    static const int    SHIFT           = 5;        /// Сдвиг график относительно правого края
    static const int    STEP_ZOOM       = 3;        /// Шаг масштабирования
    static const int    DEF_ZOOM        = 10;       /// Масштаб отображения по умолчанию
    static constexpr float  ORD_MEASURE     = 1.0;      /// Масштаб оси ординат
    static constexpr float  ORD_EXPANSION   = 2.0;      /// Параметр растяжения оси ординат
};

/// Выбор ЗКВ для отображения
void GraphOfTracksCount::showGraph(const int _numArea)
{
    numArea = _numArea;
}

/// Установление ЗКВ
void GraphOfTracksCount::setAreas(const QMap <int, Area> &_areas)
{
    areas = &_areas;
}

}

#endif // GRAPHOFTRACKSCOUNT_H
