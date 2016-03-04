#include "settingtotal.h"

namespace ThreatLevel
{

SettingTotal::SettingTotal(Painter *_painter, QWidget *parent) :
    QWidget(parent), painter(_painter), isStart(false)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    hLayout->addWidget(new QSplitter());

    hLayout->addWidget(new QLabel(QObject::tr("Коэффициент скорости (x 0.1):")));

    QSlider *sSpeedFactor = new QSlider(Qt::Horizontal);
    sSpeedFactor->setRange(1, 20);
    sSpeedFactor->setTickInterval(1);
    sSpeedFactor->setValue(painter->getSpeedFactor());
    sSpeedFactor->setTickPosition(QSlider::TicksAbove);
    QObject::connect(sSpeedFactor, SIGNAL(valueChanged(int)), painter, SLOT(setSpeedFactor(int)));
    hLayout->addWidget(sSpeedFactor);

    QLCDNumber *lSpeedFactor = new QLCDNumber(2);
    lSpeedFactor->setSegmentStyle(QLCDNumber::Flat);
    lSpeedFactor->setMode(QLCDNumber::Dec);
    lSpeedFactor->display(painter->getSpeedFactor());
    QObject::connect(sSpeedFactor, SIGNAL(valueChanged(int)), lSpeedFactor, SLOT(display(int)));
    hLayout->addWidget(lSpeedFactor);

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

    pStartFromStart = new QPushButton(QObject::tr("Начать с начала"));
    pStartFromStart->setFixedWidth(200);
    QObject::connect(pStartFromStart, SIGNAL(clicked()), painter, SLOT(resetTime()));
    hLayout->addWidget(pStartFromStart);

    hLayout->addWidget(new QSplitter());

    this->setLayout(hLayout);
}

SettingTotal::~SettingTotal()
{
    delete pStartStop;
    delete pStartFromStart;
}

void SettingTotal::changeState()
{
    static int nTimer;
    if(isStart)
    {
        painter->killTimer(nTimer);
        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    else
    {
        nTimer = painter->startTimer(100);
        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    isStart = !isStart;
}

}
