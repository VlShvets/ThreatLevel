#include "mainthread.h"

#include <QDebug>

namespace ThreatLevel
{

/// Гласс главного потока вычислений и отрисовки
MainThread::MainThread(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons,
                       Painter *_painter, GraphOfTracksCount *_graphOfTracksCount, Results *_results, const int _waitingTime)
    : painter(_painter), graphOfTracksCount(_graphOfTracksCount), results(_results), waitingTime(_waitingTime),
      isCompleted(false), isPause(false)
{
    imitation                   = new Imitation(_parametersOfAreas, _parametersOfEtalons);
    tertiaryProcessingOfData    = new TertiaryProcessingOfData;
    definitionOfThreatLevel     = new DefinitionOfThreatLevel;

    /// --------------------------------------------------
    /// Имитация
    /// --------------------------------------------------

    /// Получение ЗКВ
    areas = imitation->getAreas();

    /// Получение эталонов
    etalons = imitation->getEtalons();

    /// --------------------------------------------------
    /// Третичная обработка данных
    /// --------------------------------------------------

    /// Отправление ЗКВ в класс третичной обработки данных
    tertiaryProcessingOfData->setAreas(*areas);

    /// Отправление эталонов в класс третичной обработки данных
    tertiaryProcessingOfData->setEtalons(*etalons);

    /// Получение трасс
    tracks = tertiaryProcessingOfData->getTracks();

    /// --------------------------------------------------
    /// Определение уровня угроз
    /// --------------------------------------------------

    /// Отправление ЗКВ в класс определения уровня угроз
    definitionOfThreatLevel->setAreas(*areas);

    /// Отправление трасс в класс определения уровня угроз
    definitionOfThreatLevel->setTracks(*tracks);

    /// --------------------------------------------------
    /// Отрисовка
    /// --------------------------------------------------

    /// Отправление ЗКВ на виджет отрисовки
    painter->setAreas(*areas);

    /// Отправление эталонов на виджет отрисокви
    painter->setEtalons(*etalons);

    /// Отправление трасс на виджет отрисовки
    painter->setTracks(*tracks);

    /// --------------------------------------------------
    /// Таблица результатов
    /// --------------------------------------------------

    /// Отправление ЗКВ на виджет таблицы результатов
    results->setAreas(*areas);

    /// Отправление трасс на виджет таблицы результатов
    results->setTracks(*tracks);
}

MainThread::~MainThread()
{
    delete definitionOfThreatLevel;
    delete tertiaryProcessingOfData;
    delete imitation;

    /// Очищение виджета отрисовки
    painter->clearing();

    /// Очищение виджета отображения результатов
    results->resetTable();

    /// Очищение графика количественного состава налета
    graphOfTracksCount->resetGraph();
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
//        qDebug() << "Time = " << time;

        /// Имитация
        imitation->run(time);

        /// Третичная обработка данных
        tertiaryProcessingOfData->run();

        /// Определение уровня угроз
        definitionOfThreatLevel->run();

        /// График количественного состава налета
        graphOfTracksCount->loadTrackCount(*areas);
    }
}

}
