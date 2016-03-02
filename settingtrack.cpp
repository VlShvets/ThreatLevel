#include "settingtrack.h"

SettingTrack::SettingTrack(Painter *_painter, QWidget *parent) :
    QWidget(parent), painter(_painter)
{
    this->setWindowTitle(QObject::tr("Параметры трасс"));

    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество целей: ")), 0, 0, 1, 1);

    QSlider *sliderTrack = new QSlider(Qt::Horizontal);
    sliderTrack->setRange(1, 9);
    sliderTrack->setTickInterval(1);
    sliderTrack->setValue(painter->nTrack);
    sliderTrack->setTickPosition(QSlider::TicksAbove);
    sliderTrack->setFixedWidth(100);
    QObject::connect(sliderTrack, SIGNAL(valueChanged(int)), this, SLOT(changeNumberTrack(int)));
    gridLayout->addWidget(sliderTrack, 0, 1, 1, 1);

    lNumberTrack = new QLCDNumber(1);
    lNumberTrack->setSegmentStyle(QLCDNumber::Flat);
    lNumberTrack->setMode(QLCDNumber::Dec);
    lNumberTrack->display(painter->nTrack);
    lNumberTrack->setFixedWidth(100);
    gridLayout->addWidget(lNumberTrack, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("\tПараметры целей:")), 1, 0, 1, 3);

    tParTrack = new QTableWidget(painter->nTrack, 4, this);
    tParTrack->setHorizontalHeaderLabels(QStringList() << "Координата X" << "Координата Y" << "Скорость" << "Курс");
    QObject::connect(tParTrack, SIGNAL(cellChanged(int,int)), this, SLOT(changeParTrack(int,int)));
    gridLayout->addWidget(tParTrack, 2, 0, 1, 3);

    loadTable();

    this->setLayout(gridLayout);
}

SettingTrack::~SettingTrack()
{
    delete lNumberTrack;
    delete tParTrack;
}

void SettingTrack::loadTable()
{
    for(int i = 0; i < painter->nTrack; ++i)
    {
        tParTrack->setItem(i, 0, new QTableWidgetItem(QString::number(painter->track.at(i).pos.x())));
        tParTrack->setItem(i, 1, new QTableWidgetItem(QString::number(painter->track.at(i).pos.y())));
        tParTrack->setItem(i, 2, new QTableWidgetItem(QString::number(painter->track.at(i).modV)));
        tParTrack->setItem(i, 3, new QTableWidgetItem(QString::number(painter->track.at(i).angV)));
    }
}

void SettingTrack::changeNumberTrack(int _count)
{
    painter->nTrack = _count;

    tParTrack->setRowCount(_count);
    loadTable();

    lNumberTrack->display(_count);
}

void SettingTrack::changeParTrack(int _i, int _j)
{
    switch(_j)
    {
    case 0:
    {
        painter->track[_i].pos.setX(tParTrack->item(_i, _j)->text().toFloat());
        break;
    }
    case 1:
    {
        painter->track[_i].pos.setY(tParTrack->item(_i, _j)->text().toFloat());
        break;
    }
    case 2:
    {
        painter->track[_i].modV = tParTrack->item(_i, _j)->text().toFloat();
        break;
    }
    case 3:
    {
        painter->track[_i].angV = tParTrack->item(_i, _j)->text().toFloat();
        break;
    }
    default:
    {
        break;
    }
    }

    painter->repaint();
}
