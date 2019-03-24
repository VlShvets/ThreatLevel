#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include "imitation.h"
#include "tertiaryprocessingofdata.h"
#include "definitionofthreatlevel.h"
#include "painter.h"
#include "graphoftrackscount.h"
#include "results.h"

#include <QThread>

namespace ThreatLevel
{

/// Класс главного потока вычислений
class MainThread : public QThread
{
    Q_OBJECT

public:
    explicit        MainThread(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons,
                               Painter *_painter, GraphOfTracksCount *_graphOfTracksCount, Results *_results,
                               const int _waitingTime);
    ~MainThread();

    /// Поток вычислений
    void            run();

    /// Установление флага приостановления потока вычислений
    inline void     setPause(const bool _pause);

    /// Возвращение флага приостановления потока вычислений
    inline bool     isPaused();

public slots:
    /// Завершение потока вычислений
    inline void     complete();

    /// Установление времени ожидания между циклами вычислений
    inline void     setWaitingTime(const int _waitingTime);

private:
    /// --------------------------------------------------
    /// Указатели на объекты классов
    /// --------------------------------------------------

    Imitation                   *imitation;                 /// Класс имитации
    TertiaryProcessingOfData    *tertiaryProcessingOfData;  /// Класс третичной обработки данных
    DefinitionOfThreatLevel     *definitionOfThreatLevel;   /// Класс определения уровня угрозы
    Painter                     *painter;                   /// Класс виджета отрисовки ЗКВ и трасс
    GraphOfTracksCount          *graphOfTracksCount;        /// Класс виджета графика количественного состава налёта
    Results                     *results;                   /// Класс виджета отображения таблицы результатов

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Параметры процесса потока
    int                 waitingTime;    /// Время ожидания между циклами вычислений (в мс)
    bool                isCompleted;    /// Флаг завершения потока вычислений
    bool                isPause;        /// Флаг приостановления потока вычислений

    /// Словари параметров ЗКВ, эталонов и трасс
    QMap <int, Area>            *areas;     /// Словарь ЗКВ         <номер, структура параметров>
    const QMap <int, Etalon>    *etalons;   /// Словарь эталонов    <номер, структура параметров>
    QMap <int, Track>           *tracks;    /// Словарь трасс       <номер, структура параметров>

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    static constexpr float  PAUSE_T             = 1.0;          /// Интервал проверки флага приостановления потока вычислений
    static constexpr float  DELTA_T             = 10.0;         /// Интервал времени измерений для рассчётов (в с)
};

/// Установление флага приостановления потока вычислений
void MainThread::setPause(const bool _pause)
{
    isPause = _pause;
}

/// Возвращение флага приостановления потока вычислений
bool MainThread::isPaused()
{
    return isPause;
}

/// Завершение потока вычислений
void MainThread::complete()
{
    isCompleted = true;
}

/// Установление времени ожидания между циклами вычислений (в мс)
void MainThread::setWaitingTime(const int _waitingTime)
{
    waitingTime = _waitingTime;
}

}

#endif // MAINTHREAD_H
