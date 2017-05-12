#include "mainthread.h"

namespace ThreatLevel
{

/// Гласс главного потока вычислений и отрисовки
MainThread::MainThread(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons,
                       Painter *_painter, Results *_results, QWidget *_parent)
    : QThread(_parent), painter(_painter), results(_results), isCompleted(false), waitingTime(DEF_WAITING_TIME)
{
    imitation = new Imitation(_parametersOfAreas, _parametersOfEtalons);
}

MainThread::~MainThread()
{
    QMap <int, Area>::iterator area = areas.begin();
    for(; area != areas.end(); ++area)
        area.value().numTrack.clear();

    areas.clear();
    etalons.clear();
    tracks.clear();

    results->resetTable();

//    delete definitionOfThreatLevel;
//    delete tertiaryProcessingOfData;
    delete imitation;
}

void MainThread::run()
{
    const float time = 0.0;

    while(!isCompleted)
    {
        wait(waitingTime);



        exec();
    }
}

}
