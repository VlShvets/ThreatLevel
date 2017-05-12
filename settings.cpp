#include "settings.h"

namespace ThreatLevel
{

/// Класс виджета настроек
Settings::Settings(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons,
                   Painter *_painter, Results *_results, QWidget *_parent)
    : QWidget(_parent), parametersOfAreas(_parametersOfAreas), parametersOfEtalons(_parametersOfEtalons),
      painter(_painter), results(_results), mainThread(NULL)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    hLayout->addWidget(new QSplitter());

    /// Кнопка "Начать с начала"
    QPushButton *pReStart = new QPushButton(tr("Начать с начала"));
    pReStart->setFixedWidth(200);
    QObject::connect(pReStart, SIGNAL(clicked()), this, SLOT(reStart()));
    hLayout->addWidget(pReStart);

    hLayout->addWidget(new QSplitter());

    hLayout->addWidget(new QLabel(tr("Интервал таймера (100 / X):")));

    /// Слайдер регулирования интервала таймера
    QSlider *sTimerInterval = new QSlider(Qt::Horizontal);
    sTimerInterval->setRange(TIMER_MIN_INTERVAL, TIMER_MAX_INTERVAL);
    sTimerInterval->setTickInterval(1);
    sTimerInterval->setValue(TIMER_DEF_INTERVAL);
    sTimerInterval->setTickPosition(QSlider::TicksAbove);
    QObject::connect(sTimerInterval, SIGNAL(valueChanged(int)), this, SLOT(changeTimerInterval(int)));
    hLayout->addWidget(sTimerInterval);

    /// Дисплей отображения интервала таймера
    lTimerInterval = new QLCDNumber(2);
    lTimerInterval->setSegmentStyle(QLCDNumber::Flat);
    lTimerInterval->setMode(QLCDNumber::Dec);
    lTimerInterval->display(TIMER_DEF_INTERVAL);
    QObject::connect(sTimerInterval, SIGNAL(valueChanged(int)), lTimerInterval, SLOT(display(int)));
    hLayout->addWidget(lTimerInterval);

    hLayout->addWidget(new QSplitter());

    /// Кнопка "Play/Pause"
    pStartStop = new QPushButton;
    pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    pStartStop->setFixedWidth(200);
    QObject::connect(pStartStop, SIGNAL(clicked()), this, SLOT(changeState()));
    hLayout->addWidget(pStartStop);

    hLayout->addWidget(new QSplitter());

    this->setLayout(hLayout);
}

Settings::~Settings()
{
    delete pStartStop;
    delete lTimerInterval;
}

/// Создание нового потока вычислений (удаление уже имеющегося потока)
void Settings::reStart()
{
    if(mainThread)
    {
        mainThread->complete();
        mainThread->quit();
        mainThread->wait();
        delete mainThread;
    }

    mainThread = new MainThread(parametersOfAreas, parametersOfEtalons, painter, results);
}

/// Запуск и остановка потока вычислений
void Settings::stateChanged()
{
    /// Гласс главного потока вычислений и отрисовки
//    ThreatLevel::MainThread mainThread(painter);
//    mainThread.start();

//    if(isStart)
//    {
//        if(painter->getIdTimer() != -1)
//            painter->killTimer(painter->getIdTimer());
//        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
//    }
//    else
//    {
//        painter->setIdTimer(painter->startTimer((int) 100 / lTimerInterval->intValue()));
//        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
//    }
//    isStart = !isStart;
}

/// Изменение интервала таймера потока вычислений
void Settings::intervalOfTimerChanged(int _interval)
{
//    if(isStart && painter->getIdTimer() != -1)
//    {
//        painter->killTimer(painter->getIdTimer());
//        painter->setIdTimer(painter->startTimer((int) 100 / _interval));
//    }
}

}
