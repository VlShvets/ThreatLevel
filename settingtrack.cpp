#include "settingtrack.h"

namespace ThreatLevel
{

SettingTrack::SettingTrack(QWidget *parent) : QWidget(parent)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество групп: ")), 0, 0, 1, 1);

    QSlider *sliderGroup = new QSlider(Qt::Horizontal);
    sliderGroup->setRange(1, MAXNUMGROUPS);
    sliderGroup->setTickInterval(1);
    sliderGroup->setValue(DEFNUMGROUPS);
    sliderGroup->setTickPosition(QSlider::TicksAbove);
    sliderGroup->setFixedWidth(100);
    gridLayout->addWidget(sliderGroup, 0, 1, 1, 1);

    lNumGroups = new QLCDNumber(1);
    lNumGroups->setSegmentStyle(QLCDNumber::Flat);
    lNumGroups->setMode(QLCDNumber::Dec);
    lNumGroups->display(DEFNUMGROUPS);
    lNumGroups->setFixedWidth(100);
    QObject::connect(sliderGroup, SIGNAL(valueChanged(int)), lNumGroups, SLOT(display(int)));
    gridLayout->addWidget(lNumGroups, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество трасс в группе: ")), 1, 0, 1, 1);

    QSlider *sliderTrack = new QSlider(Qt::Horizontal);
    sliderTrack->setRange(1, MAXTRACKSINGROUP);
    sliderTrack->setTickInterval(1);
    sliderTrack->setValue(DEFTRACKSINGROUP);
    sliderTrack->setTickPosition(QSlider::TicksAbove);
    sliderTrack->setFixedWidth(100);
    gridLayout->addWidget(sliderTrack, 1, 1, 1, 1);

    lTracksInGroup = new QLCDNumber(2);
    lTracksInGroup->setSegmentStyle(QLCDNumber::Flat);
    lTracksInGroup->setMode(QLCDNumber::Dec);
    lTracksInGroup->display(DEFTRACKSINGROUP);
    lTracksInGroup->setFixedWidth(100);
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), lTracksInGroup, SLOT(display(int)));
    gridLayout->addWidget(lTracksInGroup, 1, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("\tПараметры целей:")), 2, 0, 1, 3);

    tTrackPar = new QTableWidget(DEFNUMGROUPS * DEFTRACKSINGROUP, 4, this);
    tTrackPar->setHorizontalHeaderLabels(QStringList() << "Координата X" << "Координата Y" << "Скорость" << "Курс (град.)");
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), this, SLOT(initPar(int)));
    gridLayout->addWidget(tTrackPar, 3, 0, 1, 3);

    this->setLayout(gridLayout);

    initPar(DEFNUMGROUPS * DEFTRACKSINGROUP);
}

SettingTrack::~SettingTrack()
{
    delete lNumGroups;
    delete lTracksInGroup;
    delete tTrackPar;
}

void SettingTrack::initPar(int _number)
{
    tTrackPar->setRowCount(_number);

    for(int i = 0; i < lTracksInGroup->intValue(); ++i)
    {
        /// Группа трасс №1
        tTrackPar->setItem(i, 0, new QTableWidgetItem(QString::number(0.0 + DISTBETWEENTRACKS * (int) (i / qSqrt(lTracksInGroup->intValue())))));
        tTrackPar->setItem(i, 1, new QTableWidgetItem(QString::number(-500000.0 - DISTBETWEENTRACKS * (i % (int) qSqrt(lTracksInGroup->intValue())))));
        tTrackPar->setItem(i, 2, new QTableWidgetItem(QString::number(3000.0)));
        tTrackPar->setItem(i, 3, new QTableWidgetItem(QString::number(0.0)));

        /// Группа трасс №2
        tTrackPar->setItem(i + 1, 0, new QTableWidgetItem(QString::number(-500000.0 - DISTBETWEENTRACKS * (int) (i / qSqrt(lTracksInGroup->intValue())))));
        tTrackPar->setItem(i + 1, 1, new QTableWidgetItem(QString::number(500000.0 + DISTBETWEENTRACKS * (i % (int) qSqrt(lTracksInGroup->intValue())))));
        tTrackPar->setItem(i + 1, 2, new QTableWidgetItem(QString::number(3000.0)));
        tTrackPar->setItem(i + 1, 3, new QTableWidgetItem(QString::number(3 * M_PI_4)));

        /// Группа трасс №3
        tTrackPar->setItem(i + 1, 0, new QTableWidgetItem(QString::number(500000.0 - DISTBETWEENTRACKS * (int) (i / qSqrt(lTracksInGroup->intValue())))));
        tTrackPar->setItem(i + 1, 1, new QTableWidgetItem(QString::number(500000.0 + DISTBETWEENTRACKS * (i % (int) qSqrt(lTracksInGroup->intValue())))));
        tTrackPar->setItem(i + 1, 2, new QTableWidgetItem(QString::number(3000.0)));
        tTrackPar->setItem(i + 1, 3, new QTableWidgetItem(QString::number(5 * M_PI_4)));
    }
}

}
