#include "settingtotal.h"

SettingTotal::SettingTotal(Painter *_painter, QWidget *parent) :
    QWidget(parent), painter(_painter), isStart(false)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    hLayout->addWidget(new QSplitter());

    hLayout->addWidget(new QLabel(QObject::tr("Длина следа:")));

    QSlider *sliderMemory = new QSlider(Qt::Horizontal);
    sliderMemory->setRange(0, 200);
    sliderMemory->setTickInterval(20);
    sliderMemory->setValue(painter->sizeOfMemory);
    sliderMemory->setTickPosition(QSlider::TicksAbove);
    QObject::connect(sliderMemory, SIGNAL(valueChanged(int)), this, SLOT(changeSizeMemory(int)));
    hLayout->addWidget(sliderMemory);

    pStartStop = new QPushButton;
    pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    pStartStop->setFixedWidth(200);
    QObject::connect(pStartStop, SIGNAL(clicked()), this, SLOT(visualizerStartStop()));
    hLayout->addWidget(pStartStop);

    hLayout->addWidget(new QSplitter());

    hLayout->addWidget(new QLabel(QObject::tr("Время моделирования:")));

    QSpinBox *sTotalTime = new QSpinBox(this);
    sTotalTime->setValue((int) painter->totalTime);
    sTotalTime->setRange(0, 600);
    sTotalTime->setSuffix(QObject::tr(" c"));
    QObject::connect(sTotalTime, SIGNAL(valueChanged(int)), this, SLOT(changeTotalTime(int)));
    hLayout->addWidget(sTotalTime);

    pStartFromStart = new QPushButton(QObject::tr("Начать с начала"));
    pStartFromStart->setFixedWidth(200);
    QObject::connect(pStartFromStart, SIGNAL(clicked()), this, SLOT(visualizerStartFromStart()));
    hLayout->addWidget(pStartFromStart);

    hLayout->addWidget(new QSplitter());

    this->setLayout(hLayout);
}

SettingTotal::~SettingTotal()
{
    delete pStartStop;
    delete pStartFromStart;
}

void SettingTotal::changeSizeMemory(int _count)
{
    painter->sizeOfMemory = _count + 1;
}

void SettingTotal::visualizerStartStop()
{
    if(isStart)
    {
        painter->tTime->stop();

        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    else
    {
        painter->tTime->start(100);

        pStartStop->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    isStart = !isStart;
}

void SettingTotal::changeTotalTime(int _count)
{
    painter->totalTime = (float) _count;
}

void SettingTotal::visualizerStartFromStart()
{
    painter->time = 0.0;

    painter->repaint();
}
