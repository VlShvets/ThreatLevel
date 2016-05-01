#include "areaparameters.h"

namespace ThreatLevel
{

AreaParameters::AreaParameters(QWidget *parent) : QWidget(parent)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество ПР: ")), 0, 0, 1, 1);

    QSlider *sliderArea = new QSlider(Qt::Horizontal);
    sliderArea->setRange(1, MAXNUMAREAS);
    sliderArea->setTickInterval(1);
    sliderArea->setValue(DEFNUMAREAS);
    sliderArea->setTickPosition(QSlider::TicksAbove);
    sliderArea->setFixedWidth(100);
    gridLayout->addWidget(sliderArea, 0, 1, 1, 1);

    QLCDNumber *lNumberArea = new QLCDNumber(1);
    lNumberArea->setSegmentStyle(QLCDNumber::Flat);
    lNumberArea->setMode(QLCDNumber::Dec);
    lNumberArea->display(DEFNUMAREAS);
    lNumberArea->setFixedWidth(100);
    QObject::connect(sliderArea, SIGNAL(valueChanged(int)), lNumberArea, SLOT(display(int)));
    gridLayout->addWidget(lNumberArea, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(QObject::tr("\tНачальные параметры позиционных районов:")), 1, 0, 1, 3);

    tAreaPar = new QTableWidget(DEFNUMAREAS, 3, this);
    tAreaPar->setHorizontalHeaderLabels(QStringList() << "Координата X" << "Координата Y" << "Радиус");
    QObject::connect(sliderArea, SIGNAL(valueChanged(int)), this, SLOT(initPar(int)));
    gridLayout->addWidget(tAreaPar, 2, 0, 1, 3);

    this->setLayout(gridLayout);

    initPar(DEFNUMAREAS);
}

AreaParameters::~AreaParameters()
{
    delete tAreaPar;
}

void AreaParameters::initPar(int _number)
{
    tAreaPar->setRowCount(_number);

    switch(_number)
    {
    case 5:
        /// Позиционный район №5
        tAreaPar->setItem(4, 0, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(4, 1, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(4, 2, new QTableWidgetItem(QString::number(50000.0)));

    case 4:
        /// Позиционный район №4
        tAreaPar->setItem(3, 0, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(3, 1, new QTableWidgetItem(QString::number(-100000.0)));
        tAreaPar->setItem(3, 2, new QTableWidgetItem(QString::number(50000.0)));

    case 3:
        /// Позиционный район №3
        tAreaPar->setItem(2, 0, new QTableWidgetItem(QString::number(100000.0)));
        tAreaPar->setItem(2, 1, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(2, 2, new QTableWidgetItem(QString::number(50000.0)));

    case 2:
        /// Позиционный район №2
        tAreaPar->setItem(1, 0, new QTableWidgetItem(QString::number(-100000.0)));
        tAreaPar->setItem(1, 1, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(1, 2, new QTableWidgetItem(QString::number(50000.0)));

    case 1:
        /// Позиционный район №1
        tAreaPar->setItem(0, 0, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(0, 1, new QTableWidgetItem(QString::number(100000.0)));
        tAreaPar->setItem(0, 2, new QTableWidgetItem(QString::number(50000.0)));

    default:
        break;

    }
}

}
