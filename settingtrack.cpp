#include "settingtrack.h"

namespace ThreatLevel
{

SettingTrack::SettingTrack(Painter *_painter, QWidget *parent) :
    QWidget(parent), painter(_painter)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество целей: ")), 0, 0, 1, 1);

    QSlider *sliderTrack = new QSlider(Qt::Horizontal);
    sliderTrack->setRange(1, MAXNUMTRACKS);
    sliderTrack->setTickInterval(1);
    sliderTrack->setValue(painter->getTrackCount());
    sliderTrack->setTickPosition(QSlider::TicksAbove);
    sliderTrack->setFixedWidth(100);
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), painter, SLOT(setTrackCount(int)));
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), this, SLOT(loadTable(int)));
    gridLayout->addWidget(sliderTrack, 0, 1, 1, 1);

    QLCDNumber *lNumberTrack = new QLCDNumber(2);
    lNumberTrack->setSegmentStyle(QLCDNumber::Flat);
    lNumberTrack->setMode(QLCDNumber::Dec);
    lNumberTrack->display(painter->getTrackCount());
    lNumberTrack->setFixedWidth(100);
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), lNumberTrack, SLOT(display(int)));
    gridLayout->addWidget(lNumberTrack, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("\tПараметры целей:")), 1, 0, 1, 3);

    tTrackPar = new QTableWidget(painter->getTrackCount(), 4, this);
    tTrackPar->setHorizontalHeaderLabels(QStringList() << "Координата X" << "Координата Y" << "Скорость" << "Курс (град.)");
    QObject::connect(tTrackPar, SIGNAL(cellChanged(int,int)), this, SLOT(changeTrackPar(int,int)));
    gridLayout->addWidget(tTrackPar, 2, 0, 1, 3);

    loadTable(painter->getTrackCount());

    this->setLayout(gridLayout);
}

SettingTrack::~SettingTrack()
{
    delete tTrackPar;
}

void SettingTrack::loadTable(int _number)
{
    tTrackPar->setRowCount(_number);

    for(int i = 0; i < _number; ++i)
    {
        tTrackPar->setItem(i, 0, new QTableWidgetItem(QString::number(painter->trackAt(i).pos.x())));
        tTrackPar->setItem(i, 1, new QTableWidgetItem(QString::number(painter->trackAt(i).pos.y())));
        tTrackPar->setItem(i, 2, new QTableWidgetItem(QString::number(painter->trackAt(i).modV)));
        tTrackPar->setItem(i, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(painter->trackAt(i).angV))));
    }
}

void SettingTrack::changeTrackPar(int _i, int _j)
{
    switch(_j)
    {
    case 0:
    {
        painter->getTrack(_i).pos.setX(tTrackPar->item(_i, _j)->text().toFloat());
        break;
    }
    case 1:
    {
        painter->getTrack(_i).pos.setY(tTrackPar->item(_i, _j)->text().toFloat());
        break;
    }
    case 2:
    {
        painter->getTrack(_i).modV = tTrackPar->item(_i, _j)->text().toFloat();
        break;
    }
    case 3:
    {
        painter->getTrack(_i).angV = qDegreesToRadians(tTrackPar->item(_i, _j)->text().toFloat());
        break;
    }
    default:
    {
        break;
    }
    }

    painter->repaint();
}

}
