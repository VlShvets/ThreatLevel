#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include "mainwindow.h"
#include "imitation.h"
#include "tertiaryprocessingofdata.h"
//#include "definitionofthreatlevel.h"
#include "painter.h"

#include <QThread>

namespace ThreatLevel
{

/// Гласс главного потока вычислений и отрисовки
class MainThread : public QThread
{
public:
    explicit MainThread();

    /// Установка таймера
    inline void     setIdTimer(int _idTimer);

    /// Возврат номера таймера
    inline int      getIdTimer()    const;

private:
    /// Указатели на объекты классов
    Imitation                   *imitation;                 /// Класс имитации
    class TertiaryProcessingOfData    *tertiaryProcessingOfData;  /// Класс третичной обработки данных
//    DefinitionOfThreatLevel     *definitionOfThreatLevel;   /// Класс определения уровня угрозы
    Painter                     *painter;                   /// Класс виджета отрисовки трасс и позиционных районов

    /// Вычислительные параметры
    static const float  DELTA_T             = 10.0;         /// Интервал времени измерений
};

/// Установка таймера
//void MainThread::setIdTimer(int _idTimer)
//{
//    idTimer = _idTimer;
//}

/// Возврат номера таймера
//int MainThread::getIdTimer() const
//{
//    return idTimer;
//}

}

#endif // MAINTHREAD_H
