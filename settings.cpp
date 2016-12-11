#include "settings.h"

#include <QDebug>

namespace ThreatLevel
{

/// Класс виджета настроек
Settings::Settings(Painter *_painter, QWidget *parent) :
    QWidget(parent), painter(_painter), isStart(false)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    hLayout->addWidget(new QSplitter());

    QPushButton *pReStart = new QPushButton(QObject::tr("Начать с начала"));
    pReStart->setFixedWidth(200);
    QObject::connect(pReStart, SIGNAL(clicked()), painter, SLOT(reStart()));
    hLayout->addWidget(pReStart);

    hLayout->addWidget(new QSplitter());

    hLayout->addWidget(new QLabel(QObject::tr("Интервал таймера (100 / X):")));

    QSlider *sTimerInterval = new QSlider(Qt::Horizontal);
    sTimerInterval->setRange(1, MAXTIMERINTERVAL);
    sTimerInterval->setTickInterval(1);
    sTimerInterval->setValue(DEFTIMERINTERVAL);
    sTimerInterval->setTickPosition(QSlider::TicksAbove);
    QObject::connect(sTimerInterval, SIGNAL(valueChanged(int)), this, SLOT(changeTimerInterval(int)));
    hLayout->addWidget(sTimerInterval);

    lTimerInterval = new QLCDNumber(2);
    lTimerInterval->setSegmentStyle(QLCDNumber::Flat);
    lTimerInterval->setMode(QLCDNumber::Dec);
    lTimerInterval->display(DEFTIMERINTERVAL);
    QObject::connect(sTimerInterval, SIGNAL(valueChanged(int)), lTimerInterval, SLOT(display(int)));
    hLayout->addWidget(lTimerInterval);

    hLayout->addWidget(new QSplitter());

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

/// Запуск и остановка таймера
void Settings::changeState()
{
    if(isStart)
    {
        if(painter->getIdTimer() != -1)
            painter->killTimer(painter->getIdTimer());
        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    else
    {
        painter->setIdTimer(painter->startTimer((int) 100 / lTimerInterval->intValue()));
        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    isStart = !isStart;
}

/// Изменение интервала таймера
void Settings::changeTimerInterval(int _interval)
{
    if(isStart && painter->getIdTimer() != -1)
    {
        painter->killTimer(painter->getIdTimer());
        painter->setIdTimer(painter->startTimer((int) 100 / _interval));
    }
}

}
