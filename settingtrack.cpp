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
    sliderTrack->setValue(painter->getNumberOfTracks());
    sliderTrack->setTickPosition(QSlider::TicksAbove);
    sliderTrack->setFixedWidth(100);
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), painter, SLOT(setNumberOfTracks(int)));
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), this, SLOT(loadTable(int)));
    gridLayout->addWidget(sliderTrack, 0, 1, 1, 1);

    QLCDNumber *lNumberTrack = new QLCDNumber(2);
    lNumberTrack->setSegmentStyle(QLCDNumber::Flat);
    lNumberTrack->setMode(QLCDNumber::Dec);
    lNumberTrack->display(painter->getNumberOfTracks());
    lNumberTrack->setFixedWidth(100);
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), lNumberTrack, SLOT(display(int)));
    gridLayout->addWidget(lNumberTrack, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("\tПараметры целей:")), 1, 0, 1, 3);

    tParTrack = new QTableWidget(painter->getNumberOfTracks(), 4, this);
    tParTrack->setHorizontalHeaderLabels(QStringList() << "Координата X" << "Координата Y" << "Скорость" << "Курс (град.)");
    QObject::connect(tParTrack, SIGNAL(cellChanged(int,int)), this, SLOT(changeParTrack(int,int)));
    gridLayout->addWidget(tParTrack, 2, 0, 1, 3);

    loadTable(painter->getNumberOfTracks());

    this->setLayout(gridLayout);
}

SettingTrack::~SettingTrack()
{
    delete tParTrack;
}

void SettingTrack::loadTable(int _number)
{
    tParTrack->setRowCount(_number);

    for(int i = 0; i < _number; ++i)
    {
        tParTrack->setItem(i, 0, new QTableWidgetItem(QString::number(painter->trackAt(i).pos.x())));
        tParTrack->setItem(i, 1, new QTableWidgetItem(QString::number(painter->trackAt(i).pos.y())));
        tParTrack->setItem(i, 2, new QTableWidgetItem(QString::number(painter->trackAt(i).modV)));
        tParTrack->setItem(i, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(painter->trackAt(i).angV))));
    }
}

void SettingTrack::changeParTrack(int _i, int _j)
{
    switch(_j)
    {
    case 0:
    {
        painter->getTrack(_i).pos.setX(tParTrack->item(_i, _j)->text().toFloat());
        break;
    }
    case 1:
    {
        painter->getTrack(_i).pos.setY(tParTrack->item(_i, _j)->text().toFloat());
        break;
    }
    case 2:
    {
        painter->getTrack(_i).modV = tParTrack->item(_i, _j)->text().toFloat();
        break;
    }
    case 3:
    {
        painter->getTrack(_i).angV = qDegreesToRadians(tParTrack->item(_i, _j)->text().toFloat());
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
