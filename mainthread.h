#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include "imitation.h"
//#include "tertiaryprocessingofdata.h"
//#include "definitionofthreatlevel.h"
#include "painter.h"

#include <QThread>

namespace ThreatLevel
{

/// Класс главного потока вычислений и отрисовки
class MainThread : public QThread
{
public:
    explicit        MainThread(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons,
                               Painter *_painter, Results *_results, QWidget *_parent = 0);
    ~MainThread();

    void run();

    /// Завершить работу потока
    inline void     complete();

private:

    /// --------------------------------------------------
    /// Указатели на объекты классов
    /// --------------------------------------------------

    Imitation                   *imitation;                 /// Класс имитации
//    TertiaryProcessingOfData    *tertiaryProcessingOfData;  /// Класс третичной обработки данных
    Painter                     *painter;                   /// Класс виджета отрисовки эталонов, трасс и ЗКВ
//    DefinitionOfThreatLevel     *definitionOfThreatLevel;   /// Класс определения уровня угрозы
    Results                     *results;                   /// Класс виджета отображения таблицы результатов

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    bool                isCompleted;    /// Флаг завершения процесса
    int                 waitingTime;    /// Время ожидания между циклами вычислений (в мс)
    QMap <int, Area>    areas;          /// Словарь ЗКВ         <номер, структура параметров>
    QMap <int, Etalon>  etalons;        /// Словарь эталонов    <номер, структура параметров>
    QMap <int, Track>   tracks;         /// Словарь трасс       <номер, структура параметров>

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    static const int    DEF_WAITING_TIME    = 100;          /// Время ожидания между циклами вычислений по умолчанию (100 мс)
    static const float  DELTA_T             = 10.0;         /// Интервал времени измерений для рассчётов (в с)
};

void MainThread::complete()
{
    isCompleted = true;
}

}

#endif // MAINTHREAD_H
