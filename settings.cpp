#include "settings.h"

#include <QDebug>

namespace ThreatLevel
{

Settings::Settings(Painter *_painter, QWidget *parent) :
    QWidget(parent), painter(_painter), isStart(false)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    hLayout->addWidget(new QSplitter());

    hLayout->addWidget(new QLabel(QObject::tr("Интервал таймера (1000 / X):")));

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

    hLayout->addWidget(new QLabel(QObject::tr("Время моделирования:")));

    QSpinBox *sTotalTime = new QSpinBox(this);
    sTotalTime->setValue(painter->getTotalTime());
    sTotalTime->setRange(0, 600);
    sTotalTime->setSuffix(QObject::tr(" c"));
    QObject::connect(sTotalTime, SIGNAL(valueChanged(int)), painter, SLOT(setTotalTime(int)));
    hLayout->addWidget(sTotalTime);

    QPushButton *pStartFromStart = new QPushButton(QObject::tr("Начать с начала"));
    pStartFromStart->setFixedWidth(200);
    QObject::connect(pStartFromStart, SIGNAL(clicked()), painter, SLOT(resetTime()));
    hLayout->addWidget(pStartFromStart);

    hLayout->addWidget(new QSplitter());

    this->setLayout(hLayout);
}

Settings::~Settings()
{
    delete pStartStop;
    delete lTimerInterval;
}

void Settings::changeState()
{
    if(isStart)
    {
        painter->killTimer(idTimer);
        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    else
    {
        idTimer = painter->startTimer((int) 1000 / lTimerInterval->intValue());
        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    isStart = !isStart;
}

void Settings::changeTimerInterval(int _interval)
{
    if(isStart)
    {
        painter->killTimer(idTimer);
        idTimer = painter->startTimer((int) 1000 / _interval);
    }
}

}
