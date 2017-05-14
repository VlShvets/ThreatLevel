#include "settings.h"

#include <QDebug>

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
    QPushButton *pReStart = new QPushButton;
    pReStart->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    pReStart->setFixedWidth(50);
    QObject::connect(pReStart, SIGNAL(clicked()), this, SLOT(reStart()));
    hLayout->addWidget(pReStart);

    /// Кнопка "Play/Pause"
    pPlayPause = new QPushButton;
    pPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    pPlayPause->setFixedWidth(50);
    QObject::connect(pPlayPause, SIGNAL(clicked()), this, SLOT(stateChanged()));
    hLayout->addWidget(pPlayPause);

    /// Кнопка "Stop"
    QPushButton *pStop = new QPushButton;
    pStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    pStop->setFixedWidth(50);
    QObject::connect(pStop, SIGNAL(clicked()), this, SLOT(stop()));
    hLayout->addWidget(pStop);

    hLayout->addWidget(new QSplitter());

    hLayout->addWidget(new QLabel(tr("Время ожидания между циклами вычислений (в мс):")));

    /// Слайдер регулирования времени ожидания между циклами вычислений
    sWaitingTime = new QSlider(Qt::Horizontal);
    sWaitingTime->setRange(WAITING_TIME_MIN, WAITING_TIME_MAX);
    sWaitingTime->setTickInterval(100);
    sWaitingTime->setPageStep(100);
    sWaitingTime->setValue(WAITING_TIME_DEF);
    sWaitingTime->setTickPosition(QSlider::TicksAbove);
    hLayout->addWidget(sWaitingTime);

    /// Дисплей отображения времени ожидания между циклами вычислений
    lWaitingTime = new QLCDNumber(4);
    lWaitingTime->setSegmentStyle(QLCDNumber::Flat);
    lWaitingTime->setMode(QLCDNumber::Dec);
    lWaitingTime->display(WAITING_TIME_DEF);
    QObject::connect(sWaitingTime, SIGNAL(valueChanged(int)), lWaitingTime, SLOT(display(int)));
    hLayout->addWidget(lWaitingTime);

    hLayout->addWidget(new QSplitter());

    QCheckBox *cAreas = new QCheckBox(tr("ЗКВ"));
    cAreas->setChecked(true);
    QObject::connect(cAreas, SIGNAL(clicked(bool)), painter, SLOT(setVisibleOfAreas(bool)));
    hLayout->addWidget(cAreas);

    QCheckBox *cEtalons = new QCheckBox(tr("Эталоны"));
    cEtalons->setChecked(true);
    QObject::connect(cEtalons, SIGNAL(clicked(bool)), painter, SLOT(setVisibleOfEtalons(bool)));
    hLayout->addWidget(cEtalons);

    QCheckBox *cTracks = new QCheckBox(tr("Трассы"));
    cTracks->setChecked(true);
    QObject::connect(cTracks, SIGNAL(clicked(bool)), painter, SLOT(setVisibleOfTracks(bool)));
    hLayout->addWidget(cTracks);

    QCheckBox *cMinDists = new QCheckBox(tr("Кратчайшие расстояния"));
    cMinDists->setChecked(false);
    QObject::connect(cMinDists, SIGNAL(clicked(bool)), painter, SLOT(setVisibleOfMinDists(bool)));
    hLayout->addWidget(cMinDists);

    hLayout->addWidget(new QSplitter());

    this->setLayout(hLayout);
}

Settings::~Settings()
{
    delete pPlayPause;
    delete lWaitingTime;
    delete sWaitingTime;
}

/// Перезапуск потока вычислений
void Settings::reStart()
{
    /// Возвращение флага приостановления потока вычислений
    bool isPaused;
    if(mainThread)
        isPaused = mainThread->isPaused();
    else
        isPaused = true;

    /// Завершение имеющегося потока вычислений
    completeOfThread();

    /// Создание нового потока вычислений
    createOfThread();

    /// Установление флага приостановления потока вычислений
    mainThread->setPause(isPaused);
}

/// Запуск и остановка потока вычислений
void Settings::stateChanged()
{
    /// Возвращение флага приостановления потока вычислений
    bool isPaused;
    if(mainThread)
        isPaused = mainThread->isPaused();
    else
        isPaused = true;
//    qDebug() << "Is Paused:" << isPaused;

    /// Создание нового потока вычислений
    createOfThread();

    if(isPaused)
        pPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    else
        pPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    /// Установление флага приостановления потока вычислений
    mainThread->setPause(!isPaused);
}

/// Остановка потока вычислений
void Settings::stop()
{
    /// Завершение имеющегося потока вычислений
    completeOfThread();

    pPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

/// Создание нового потока вычислений
void Settings::createOfThread()
{
    if(!mainThread)
    {
        qDebug() << "Create Of Thread";
        mainThread = new MainThread(parametersOfAreas, parametersOfEtalons, painter, results, lWaitingTime->intValue());
        QObject::connect(sWaitingTime, SIGNAL(valueChanged(int)), mainThread, SLOT(setWaitingTime(int)));
        mainThread->start();
    }
}

/// Завершение имеющегося потока вычислений
void Settings::completeOfThread()
{
    if(mainThread)
    {
        qDebug() << "Complete Of Thread";
        mainThread->complete();
        mainThread->wait();
        delete mainThread;
        mainThread = NULL;
    }
}

}
