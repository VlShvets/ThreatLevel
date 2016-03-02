#include "settingbase.h"

SettingBase::SettingBase(Painter *_painter, QWidget *parent) :
    QWidget(parent), painter(_painter)
{
    this->setWindowTitle(QObject::tr("Параметры баз"));

    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество баз: ")), 0, 0, 1, 1);

    QSlider *sliderBase = new QSlider(Qt::Horizontal);
    sliderBase->setRange(1, 5);
    sliderBase->setTickInterval(1);
    sliderBase->setValue(painter->nBase);
    sliderBase->setTickPosition(QSlider::TicksAbove);
    sliderBase->setFixedWidth(100);
    QObject::connect(sliderBase, SIGNAL(valueChanged(int)), this, SLOT(changeNumberBase(int)));
    gridLayout->addWidget(sliderBase, 0, 1, 1, 1);

    lNumberBase = new QLCDNumber(1);
    lNumberBase->setSegmentStyle(QLCDNumber::Flat);
    lNumberBase->setMode(QLCDNumber::Dec);
    lNumberBase->display(painter->nBase);
    lNumberBase->setFixedWidth(100);
    gridLayout->addWidget(lNumberBase, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("\tПараметры баз:")), 1, 0, 1, 3);

    tParBase = new QTableWidget(painter->nBase, 3, this);
    tParBase->setHorizontalHeaderLabels(QStringList() << "Координата X" << "Координата Y" << "Радиус");
    QObject::connect(tParBase, SIGNAL(cellChanged(int,int)), this, SLOT(changeParBase(int,int)));
    gridLayout->addWidget(tParBase, 2, 0, 1, 3);

    loadTable();

    this->setLayout(gridLayout);
}

SettingBase::~SettingBase()
{
    delete lNumberBase;
    delete tParBase;
}

void SettingBase::loadTable()
{
    for(int i = 0; i < painter->nBase; ++i)
    {
        tParBase->setItem(i, 0, new QTableWidgetItem(QString::number(painter->base.at(i).pos.x())));
        tParBase->setItem(i, 1, new QTableWidgetItem(QString::number(painter->base.at(i).pos.y())));
        tParBase->setItem(i, 2, new QTableWidgetItem(QString::number(painter->base.at(i).radius)));
    }
}

void SettingBase::changeNumberBase(int _count)
{
    painter->nBase = _count;

    tParBase->setRowCount(_count);
    loadTable();

    lNumberBase->display(_count);
}

void SettingBase::changeParBase(int _i, int _j)
{
    switch(_j)
    {
    case 0:
    {
        painter->base[_i].pos.setX(tParBase->item(_i, _j)->text().toFloat());
        break;
    }
    case 1:
    {
        painter->base[_i].pos.setY(tParBase->item(_i, _j)->text().toFloat());
        break;
    }
    case 2:
    {
        painter->base[_i].radius = tParBase->item(_i, _j)->text().toFloat();
        break;
    }
    default:
    {
        break;
    }
    }

    painter->repaint();
}
