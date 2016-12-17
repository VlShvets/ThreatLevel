#include "areaparameters.h"

namespace ThreatLevel
{

/// Класс виджета редактирования параметров позиционных районов
AreaParameters::AreaParameters(QWidget *parent) : QWidget(parent)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(tr("Количество ПР: ")), 0, 0, 1, 1);

    QSlider *sliderArea = new QSlider(Qt::Horizontal);
    sliderArea->setRange(1, MAX_COUNT_AREAS);
    sliderArea->setTickInterval(1);
    sliderArea->setValue(DEF_COUNT_AREAS);
    sliderArea->setTickPosition(QSlider::TicksAbove);
    sliderArea->setFixedWidth(100);
    gridLayout->addWidget(sliderArea, 0, 1, 1, 1);

    QLCDNumber *lNumberArea = new QLCDNumber(1);
    lNumberArea->setSegmentStyle(QLCDNumber::Flat);
    lNumberArea->setMode(QLCDNumber::Dec);
    lNumberArea->display(DEF_COUNT_AREAS);
    lNumberArea->setFixedWidth(100);
    QObject::connect(sliderArea, SIGNAL(valueChanged(int)), lNumberArea, SLOT(display(int)));
    gridLayout->addWidget(lNumberArea, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("\tНачальные параметры позиционных районов:")), 1, 0, 1, 3);

    tAreaPar = new QTableWidget(DEF_COUNT_AREAS, 5, this);
    tAreaPar->setHorizontalHeaderLabels(QStringList() << tr("Координата X") << tr("Координата Y") <<
                                        tr("Радиус") << tr("T критическое") << tr("R обнаружения"));
    QObject::connect(sliderArea, SIGNAL(valueChanged(int)), this, SLOT(initPar(int)));
    gridLayout->addWidget(tAreaPar, 2, 0, 1, 3);

    this->setLayout(gridLayout);

    initPar(DEF_COUNT_AREAS);
}

AreaParameters::~AreaParameters()
{
    delete tAreaPar;
}

/// Начальная инициализация параметров ПР
void AreaParameters::initPar(int _count)
{
    tAreaPar->setRowCount(_count);

    switch(_count)
    {
    case 5:
        /// Позиционный район №5
        tAreaPar->setItem(4, 0, new QTableWidgetItem(QString::number(250000.0)));
        tAreaPar->setItem(4, 1, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(4, 2, new QTableWidgetItem(QString::number(25000.0)));
        tAreaPar->setItem(4, 3, new QTableWidgetItem(QString::number(300.0)));
        tAreaPar->setItem(4, 4, new QTableWidgetItem(QString::number(320000.0)));

    case 4:
        /// Позиционный район №4
        tAreaPar->setItem(3, 0, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(3, 1, new QTableWidgetItem(QString::number(-250000.0)));
        tAreaPar->setItem(3, 2, new QTableWidgetItem(QString::number(50000.0)));
        tAreaPar->setItem(3, 3, new QTableWidgetItem(QString::number(300.0)));
        tAreaPar->setItem(3, 4, new QTableWidgetItem(QString::number(320000.0)));

    case 3:
        /// Позиционный район №3
        tAreaPar->setItem(2, 0, new QTableWidgetItem(QString::number(-250000.0)));
        tAreaPar->setItem(2, 1, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(2, 2, new QTableWidgetItem(QString::number(75000.0)));
        tAreaPar->setItem(2, 3, new QTableWidgetItem(QString::number(300.0)));
        tAreaPar->setItem(2, 4, new QTableWidgetItem(QString::number(320000.0)));

    case 2:
        /// Позиционный район №2
        tAreaPar->setItem(1, 0, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(1, 1, new QTableWidgetItem(QString::number(250000.0)));
        tAreaPar->setItem(1, 2, new QTableWidgetItem(QString::number(100000.0)));
        tAreaPar->setItem(1, 3, new QTableWidgetItem(QString::number(300.0)));
        tAreaPar->setItem(1, 4, new QTableWidgetItem(QString::number(320000.0)));

    case 1:
        /// Позиционный район №1
        tAreaPar->setItem(0, 0, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(0, 1, new QTableWidgetItem(QString::number(0.0)));
        tAreaPar->setItem(0, 2, new QTableWidgetItem(QString::number(150000.0)));
        tAreaPar->setItem(0, 3, new QTableWidgetItem(QString::number(300.0)));
        tAreaPar->setItem(0, 4, new QTableWidgetItem(QString::number(320000.0)));

    default:
        break;

    }
}

}
