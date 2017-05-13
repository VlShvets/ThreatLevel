#include "mainthread.h"

#include <QDebug>

namespace ThreatLevel
{

/// Гласс главного потока вычислений и отрисовки
MainThread::MainThread(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons,
                       Painter *_painter, Results *_results, const int _waitingTime)
    : painter(_painter), results(_results), waitingTime(_waitingTime), isCompleted(false), isPause(false)
{
    imitation                   = new Imitation(_parametersOfAreas, _parametersOfEtalons);
    tertiaryProcessingOfData    = new TertiaryProcessingOfData;
    definitionOfThreatLevel     = new DefinitionOfThreatLevel;
}

MainThread::~MainThread()
{
    delete definitionOfThreatLevel;
    delete tertiaryProcessingOfData;
    delete imitation;

    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
        area.value().numTrack.clear();

    areas.clear();
    etalons.clear();
    tracks.clear();

    results->resetTable();
}

/// Процесс потока
void MainThread::run()
{
    float time = 0.0;   /// Время

    /// Флаг завершения потока вычислений
    while(!isCompleted)
    {

         /// Флаг приостановки потока вычислений
        if(isPause)
        {
            sleep(PAUSE_T);
            continue;
        }

        msleep(waitingTime);
        time += DELTA_T;
        qDebug() << "Time = " << time;

        /// --------------------------------------------------
        /// Имитация
        /// --------------------------------------------------

        /// Получение ЗКВ
        areas       = imitation->getAreas();

        /// Отправление ЗКВ на виджет отрисовки
        painter->setAreas(&areas);

        /// Получение эталонов
        etalons     = imitation->getEtalons(time);

        /// Отправление эталонов на виджет отрисовки
        painter->setEtalons(&etalons);

        /// --------------------------------------------------
        /// Третичная обработка данных
        /// --------------------------------------------------

        /// Получение трасс
        tracks      = tertiaryProcessingOfData->getTracks(&areas, &etalons);

        /// --------------------------------------------------
        /// Определение уровня угроз
        /// --------------------------------------------------

        /// Отправление ЗКВ в класс определения уровня угроз
        definitionOfThreatLevel->setAreas(&areas);

        /// Отправление трасс в класс определения уровня угроз
        definitionOfThreatLevel->setTracks(&tracks);

        /// Вычислительный процесс
        definitionOfThreatLevel->run();

        /// Отправление трасс на виджет отрисовки
        painter->setTracks(&tracks);

        /// Отправление ЗКВ и трасс на виджет отображения результатов
        results->loadTable(areas, tracks);
    }

    /// Очищение виджета отрисовки
    painter->clearing();

    /// Очищение виджета отображения результатов
    results->resetTable();
}



}
