#include "settingarea.h"

namespace ThreatLevel
{

SettingArea::SettingArea(Painter *_painter, QWidget *parent) :
    QWidget(parent), painter(_painter)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество баз: ")), 0, 0, 1, 1);

    QSlider *sliderArea = new QSlider(Qt::Horizontal);
    sliderArea->setRange(1, MAXNUMAREAS);
    sliderArea->setTickInterval(1);
    sliderArea->setValue(painter->getAreaCount());
    sliderArea->setTickPosition(QSlider::TicksAbove);
    sliderArea->setFixedWidth(100);
    QObject::connect(sliderArea, SIGNAL(valueChanged(int)), painter, SLOT(setAreaCount(int)));
    QObject::connect(sliderArea, SIGNAL(valueChanged(int)), this, SLOT(loadTable(int)));

    gridLayout->addWidget(sliderArea, 0, 1, 1, 1);

    QLCDNumber *lNumberArea = new QLCDNumber(1);
    lNumberArea->setSegmentStyle(QLCDNumber::Flat);
    lNumberArea->setMode(QLCDNumber::Dec);
    lNumberArea->display(painter->getAreaCount());
    lNumberArea->setFixedWidth(100);
    QObject::connect(sliderArea, SIGNAL(valueChanged(int)), lNumberArea, SLOT(display(int)));
    gridLayout->addWidget(lNumberArea, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("\tПараметры баз:")), 1, 0, 1, 3);

    tAreaPar = new QTableWidget(painter->getAreaCount(), 3, this);
    tAreaPar->setHorizontalHeaderLabels(QStringList() << "Координата X" << "Координата Y" << "Радиус");
    QObject::connect(tAreaPar, SIGNAL(cellChanged(int,int)), this, SLOT(changeAreaPar(int,int)));
    gridLayout->addWidget(tAreaPar, 2, 0, 1, 3);

    loadTable(painter->getAreaCount());

    this->setLayout(gridLayout);
}

SettingArea::~SettingArea()
{
    delete tAreaPar;
}

void SettingArea::loadTable(int _number)
{
    tAreaPar->setRowCount(_number);

    for(int i = 0; i < _number; ++i)
    {
        tAreaPar->setItem(i, 0, new QTableWidgetItem(QString::number(painter->areaAt(i).pos.x())));
        tAreaPar->setItem(i, 1, new QTableWidgetItem(QString::number(painter->areaAt(i).pos.y())));
        tAreaPar->setItem(i, 2, new QTableWidgetItem(QString::number(painter->areaAt(i).radius)));
    }
}

void SettingArea::changeAreaPar(int _i, int _j)
{
    switch(_j)
    {
    case 0:
    {
        painter->getArea(_i).pos.setX(tAreaPar->item(_i, _j)->text().toFloat());
        break;
    }
    case 1:
    {
        painter->getArea(_i).pos.setY(tAreaPar->item(_i, _j)->text().toFloat());
        break;
    }
    case 2:
    {
        painter->getArea(_i).radius = tAreaPar->item(_i, _j)->text().toFloat();
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
