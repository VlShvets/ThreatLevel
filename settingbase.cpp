#include "settingbase.h"

namespace ThreatLevel
{

SettingBase::SettingBase(Painter *_painter, QWidget *parent) :
    QWidget(parent), painter(_painter)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество баз: ")), 0, 0, 1, 1);

    QSlider *sliderBase = new QSlider(Qt::Horizontal);
    sliderBase->setRange(1, 5);
    sliderBase->setTickInterval(1);
    sliderBase->setValue(painter->getNumberOfBases());
    sliderBase->setTickPosition(QSlider::TicksAbove);
    sliderBase->setFixedWidth(100);
    QObject::connect(sliderBase, SIGNAL(valueChanged(int)), painter, SLOT(setNumberOfBases(int)));
    QObject::connect(sliderBase, SIGNAL(valueChanged(int)), this, SLOT(loadTable(int)));

    gridLayout->addWidget(sliderBase, 0, 1, 1, 1);

    QLCDNumber *lNumberBase = new QLCDNumber(1);
    lNumberBase->setSegmentStyle(QLCDNumber::Flat);
    lNumberBase->setMode(QLCDNumber::Dec);
    lNumberBase->display(painter->getNumberOfBases());
    lNumberBase->setFixedWidth(100);
    QObject::connect(sliderBase, SIGNAL(valueChanged(int)), lNumberBase, SLOT(display(int)));
    gridLayout->addWidget(lNumberBase, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("\tПараметры баз:")), 1, 0, 1, 3);

    tParBase = new QTableWidget(painter->getNumberOfBases(), 3, this);
    tParBase->setHorizontalHeaderLabels(QStringList() << "Координата X" << "Координата Y" << "Радиус");
    QObject::connect(tParBase, SIGNAL(cellChanged(int,int)), this, SLOT(changeParBase(int,int)));
    gridLayout->addWidget(tParBase, 2, 0, 1, 3);

    loadTable(painter->getNumberOfBases());

    this->setLayout(gridLayout);
}

SettingBase::~SettingBase()
{
    delete tParBase;
}

void SettingBase::loadTable(int _number)
{
    tParBase->setRowCount(_number);

    for(int i = 0; i < _number; ++i)
    {
        tParBase->setItem(i, 0, new QTableWidgetItem(QString::number(painter->baseAt(i).pos.x())));
        tParBase->setItem(i, 1, new QTableWidgetItem(QString::number(painter->baseAt(i).pos.y())));
        tParBase->setItem(i, 2, new QTableWidgetItem(QString::number(painter->baseAt(i).radius)));
    }
}

void SettingBase::changeParBase(int _i, int _j)
{
    switch(_j)
    {
    case 0:
    {
        painter->getBase(_i).pos.setX(tParBase->item(_i, _j)->text().toFloat());
        break;
    }
    case 1:
    {
        painter->getBase(_i).pos.setY(tParBase->item(_i, _j)->text().toFloat());
        break;
    }
    case 2:
    {
        painter->getBase(_i).radius = tParBase->item(_i, _j)->text().toFloat();
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
