#include "trackparameters.h"

namespace ThreatLevel
{

TrackParameters::TrackParameters(QWidget *parent) : QWidget(parent)
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

    lNumGroups = new QLCDNumber(2);
    lNumGroups->setSegmentStyle(QLCDNumber::Flat);
    lNumGroups->setMode(QLCDNumber::Dec);
    lNumGroups->display(DEFNUMGROUPS);
    lNumGroups->setFixedWidth(100);
    QObject::connect(sliderGroup, SIGNAL(valueChanged(int)), lNumGroups, SLOT(display(int)));
    QObject::connect(sliderGroup, SIGNAL(valueChanged(int)), this, SLOT(numGroupsChanged(int)));
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
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), this, SLOT(numTracksInGroupChanged(int)));
    gridLayout->addWidget(lTracksInGroup, 1, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("\tНачальные параметры трасс:")), 2, 0, 1, 3);

    tTrackPar = new QTableWidget(DEFNUMGROUPS * DEFTRACKSINGROUP, 5, this);
    tTrackPar->setHorizontalHeaderLabels(QStringList() << "Координата X" << "Координата Y" << "Курс (град.)" << "Скорость" << "Ускорение");
    gridLayout->addWidget(tTrackPar, 3, 0, 1, 3);

    this->setLayout(gridLayout);

    initPar(DEFNUMGROUPS, DEFTRACKSINGROUP);
}

TrackParameters::~TrackParameters()
{
    delete lNumGroups;
    delete lTracksInGroup;
    delete tTrackPar;
}

void TrackParameters::numGroupsChanged(int _num)
{
    tTrackPar->setRowCount(_num * lTracksInGroup->intValue());

    initPar(_num, lTracksInGroup->intValue());
}

void TrackParameters::numTracksInGroupChanged(int _num)
{
    tTrackPar->setRowCount(lNumGroups->intValue() * _num);

    initPar(lNumGroups->intValue(), _num);
}

void TrackParameters::initPar(int _nGroups, int _nTracksInGroup)
{
    for(int i = 0; i < _nTracksInGroup; ++i)
    {
        switch(_nGroups)
        {
        case 10:
            /// Группа трасс №10
            tTrackPar->setItem(_nGroups * i + 9, 0, new QTableWidgetItem(QString::number(-180000.0 - DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i + 9, 1, new QTableWidgetItem(QString::number(-440000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i + 9, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.5 * M_PI_4 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i + 9, 3, new QTableWidgetItem(QString::number(250.0)));
            tTrackPar->setItem(_nGroups * i + 9, 4, new QTableWidgetItem(QString::number(-0.05)));

        case 9:
            /// Группа трасс №9
            tTrackPar->setItem(_nGroups * i + 8, 0, new QTableWidgetItem(QString::number(180000.0 + DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i + 8, 1, new QTableWidgetItem(QString::number(-440000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i + 8, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.5 * M_PI_4 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i + 8, 3, new QTableWidgetItem(QString::number(180.0)));
            tTrackPar->setItem(_nGroups * i + 8, 4, new QTableWidgetItem(QString::number(0.05)));

        case 8:
            /// Группа трасс №8
            tTrackPar->setItem(_nGroups * i + 7, 0, new QTableWidgetItem(QString::number(350000.0 + DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i + 7, 1, new QTableWidgetItem(QString::number(-350000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i + 7, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.0 * M_PI_4 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i + 7, 3, new QTableWidgetItem(QString::number(215.0)));
            tTrackPar->setItem(_nGroups * i + 7, 4, new QTableWidgetItem(QString::number(0.0)));

        case 7:
            /// Группа трасс №7
            tTrackPar->setItem(_nGroups * i + 6, 0, new QTableWidgetItem(QString::number(-400000.0 - DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i + 6, 1, new QTableWidgetItem(QString::number(-400000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i + 6, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(M_PI_4 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i + 6, 3, new QTableWidgetItem(QString::number(250.0)));
            tTrackPar->setItem(_nGroups * i + 6, 4, new QTableWidgetItem(QString::number(0.0)));

        case 6:
            /// Группа трасс №6
            tTrackPar->setItem(_nGroups * i + 5, 0, new QTableWidgetItem(QString::number(0.0 + DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i + 5, 1, new QTableWidgetItem(QString::number(-400000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i + 5, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.0 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i + 5, 3, new QTableWidgetItem(QString::number(180.0)));
            tTrackPar->setItem(_nGroups * i + 5, 4, new QTableWidgetItem(QString::number(0.0)));

        case 5:
            /// Группа трасс №5
            tTrackPar->setItem(_nGroups * i + 4, 0, new QTableWidgetItem(QString::number(-160000.0 - DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i + 4, 1, new QTableWidgetItem(QString::number(-390000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i + 4, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.5 * M_PI_4 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i + 4, 3, new QTableWidgetItem(QString::number(250.0)));
            tTrackPar->setItem(_nGroups * i + 4, 4, new QTableWidgetItem(QString::number(-0.05)));

        case 4:
            /// Группа трасс №4
            tTrackPar->setItem(_nGroups * i + 3, 0, new QTableWidgetItem(QString::number(160000.0 + DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i + 3, 1, new QTableWidgetItem(QString::number(-390000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i + 3, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.5 * M_PI_4 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i + 3, 3, new QTableWidgetItem(QString::number(180.0)));
            tTrackPar->setItem(_nGroups * i + 3, 4, new QTableWidgetItem(QString::number(0.05)));

        case 3:
            /// Группа трасс №3
            tTrackPar->setItem(_nGroups * i + 2, 0, new QTableWidgetItem(QString::number(300000.0 + DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i + 2, 1, new QTableWidgetItem(QString::number(-300000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i + 2, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.0 * M_PI_4 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i + 2, 3, new QTableWidgetItem(QString::number(215.0)));
            tTrackPar->setItem(_nGroups * i + 2, 4, new QTableWidgetItem(QString::number(0.0)));

        case 2:
            /// Группа трасс №2
            tTrackPar->setItem(_nGroups * i + 1, 0, new QTableWidgetItem(QString::number(-350000.0 - DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i + 1, 1, new QTableWidgetItem(QString::number(-350000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i + 1, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(M_PI_4 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i + 1, 3, new QTableWidgetItem(QString::number(250.0)));
            tTrackPar->setItem(_nGroups * i + 1, 4, new QTableWidgetItem(QString::number(0.0)));

        case 1:
            /// Группа трасс №1
            tTrackPar->setItem(_nGroups * i, 0, new QTableWidgetItem(QString::number(0.0 + DISTBETWEENTRACKS * (i / 3))));
            tTrackPar->setItem(_nGroups * i, 1, new QTableWidgetItem(QString::number(-350000.0 - DISTBETWEENTRACKS * (i % 3))));
            tTrackPar->setItem(_nGroups * i, 2, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.0 + M_PI / 36.0))));
            tTrackPar->setItem(_nGroups * i, 3, new QTableWidgetItem(QString::number(180.0)));
            tTrackPar->setItem(_nGroups * i, 4, new QTableWidgetItem(QString::number(0.0)));

        }
    }
}

}
