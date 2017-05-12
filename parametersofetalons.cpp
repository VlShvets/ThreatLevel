#include "parametersofetalons.h"

namespace ThreatLevel
{

/// Класс виджета редактирования параметров трасс
ParametersOfEtalons::ParametersOfEtalons(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество групп КР: ")), 0, 0, 1, 1);

    /// Слайдер регулирования количества групп КР
    QSlider *sGroupsOfCMCount = new QSlider(Qt::Horizontal);
    sGroupsOfCMCount->setRange(GROUPS_OF_CM_MIN_COUNT, GROUPS_OF_CM_MAX_COUNT);
    sGroupsOfCMCount->setTickInterval(1);
    sGroupsOfCMCount->setValue(GROUPS_OF_CM_DEF_COUNT);
    sGroupsOfCMCount->setTickPosition(QSlider::TicksAbove);
    sGroupsOfCMCount->setFixedWidth(100);
    gridLayout->addWidget(sGroupsOfCMCount, 0, 1, 1, 1);

    /// Дисплей отображения количества групп КР
    lGroupsOfCMCount = new QLCDNumber(2);
    lGroupsOfCMCount->setSegmentStyle(QLCDNumber::Flat);
    lGroupsOfCMCount->setMode(QLCDNumber::Dec);
    lGroupsOfCMCount->display(GROUPS_OF_CM_DEF_COUNT);
    lGroupsOfCMCount->setFixedWidth(100);
    QObject::connect(sGroupsOfCMCount, SIGNAL(valueChanged(int)), lGroupsOfCMCount, SLOT(display(int)));
    QObject::connect(sGroupsOfCMCount, SIGNAL(valueChanged(int)), this, SLOT(initCMPar(int)));
    gridLayout->addWidget(lGroupsOfCMCount, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("Количество КР в группе: ")), 1, 0, 1, 1);

    /// Слайдер регулирования количества КР в группе
    QSlider *sCMInGroupCount = new QSlider(Qt::Horizontal);
    sCMInGroupCount->setRange(1, CM_IN_GROUP_MAX_COUNT);
    sCMInGroupCount->setTickInterval(1);
    sCMInGroupCount->setValue(CM_IN_GROUP_DEF_COUNT);
    sCMInGroupCount->setTickPosition(QSlider::TicksAbove);
    sCMInGroupCount->setFixedWidth(100);
    gridLayout->addWidget(sCMInGroupCount, 1, 1, 1, 1);

    /// Дисплей отображения количества КР в группе
    lCMInGroupCount = new QLCDNumber(2);
    lCMInGroupCount->setSegmentStyle(QLCDNumber::Flat);
    lCMInGroupCount->setMode(QLCDNumber::Dec);
    lCMInGroupCount->display(CM_IN_GROUP_DEF_COUNT);
    lCMInGroupCount->setFixedWidth(100);
    QObject::connect(sCMInGroupCount, SIGNAL(valueChanged(int)), lCMInGroupCount, SLOT(display(int)));
    QObject::connect(sCMInGroupCount, SIGNAL(valueChanged(int)), this, SLOT(CMInGroupCountChanged(int)));
    gridLayout->addWidget(lCMInGroupCount, 1, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("\tНачальные параметры КР:")), 2, 0, 1, 3);

    /// Таблица параметров КР
    tCMPar = new QTableWidget(GROUPS_OF_CM_DEF_COUNT * CM_IN_GROUP_DEF_COUNT, 6, this);
    tCMPar->setHorizontalHeaderLabels(QStringList() << tr("№ трассы") << tr("Нач. коорд. X") << tr("Нач. коорд. Y") <<
                                      tr("Скорость") << tr("Курс (град.)") << tr("Колич. состав"));
    gridLayout->addWidget(tCMPar, 3, 0, 1, 3);

    gridLayout->addWidget(new QLabel(tr("Количество БЦ: ")), 4, 0, 1, 1);

    /// Слайдер регулирования количества БЦ
    QSlider *sBGCount = new QSlider(Qt::Horizontal);
    sBGCount->setRange(0, BG_MAX_COUNT);
    sBGCount->setTickInterval(1);
    sBGCount->setValue(BG_DEF_COUNT);
    sBGCount->setTickPosition(QSlider::TicksAbove);
    sBGCount->setFixedWidth(100);
    gridLayout->addWidget(sBGCount, 4, 1, 1, 1);

    /// Дисплей отображения количества БЦ
    lBGCount = new QLCDNumber(1);
    lBGCount->setSegmentStyle(QLCDNumber::Flat);
    lBGCount->setMode(QLCDNumber::Dec);
    lBGCount->display(BG_DEF_COUNT);
    lBGCount->setFixedWidth(100);
    QObject::connect(sBGCount, SIGNAL(valueChanged(int)), lBGCount, SLOT(display(int)));
    QObject::connect(sBGCount, SIGNAL(valueChanged(int)), this, SLOT(initBGPar(int)));
    gridLayout->addWidget(lBGCount, 4, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("\tНачальные параметры БЦ:")), 5, 0, 1, 3);

    /// Таблица параметров БЦ
    tBGPar = new QTableWidget(BG_DEF_COUNT, 6, this);
    tBGPar->setHorizontalHeaderLabels(QStringList() << tr("№ трассы") << tr("Нач. коорд. X") << tr("Нач. коорд. Y") <<
                                         tr("Скорость") << tr("Кон. коорд. X") << tr("Кон. коорд. Y"));
    gridLayout->addWidget(tBGPar, 6, 0, 1, 3);

    this->setLayout(gridLayout);

    CMInGroupCount = CM_IN_GROUP_DEF_COUNT;
    initCMPar(GROUPS_OF_CM_DEF_COUNT);
    initBGPar(BG_DEF_COUNT);
}

ParametersOfEtalons::~ParametersOfEtalons()
{
    delete lGroupsOfCMCount;
    delete lCMInGroupCount;
    delete tCMPar;
    delete lBGCount;
    delete tBGPar;
}

/// Изменение количества КР в группе
void ParametersOfEtalons::CMInGroupCountChanged(int _count)
{
    CMInGroupCount = _count;

    initCMPar(tCMPar->rowCount());
}

/// Начальная инициализация параметров КР
void ParametersOfEtalons::initCMPar(int _count)
{
    tCMPar->setRowCount(_count);

    switch(_count)
    {
    case 10:
        /// Группа КР №10
        tCMPar->setItem(9, 0, new QTableWidgetItem(QString::number(10)));
        tCMPar->setItem(9, 1, new QTableWidgetItem(QString::number(-360000.0)));
        tCMPar->setItem(9, 2, new QTableWidgetItem(QString::number(-880000.0)));
        tCMPar->setItem(9, 3, new QTableWidgetItem(QString::number(250.0)));
        tCMPar->setItem(9, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.5 * M_PI_4 + M_PI / 36.0))));
        tCMPar->setItem(9, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    case 9:
        /// Группа КР №9
        tCMPar->setItem(8, 0, new QTableWidgetItem(QString::number(9)));
        tCMPar->setItem(8, 1, new QTableWidgetItem(QString::number(360000.0)));
        tCMPar->setItem(8, 2, new QTableWidgetItem(QString::number(-880000.0)));
        tCMPar->setItem(8, 3, new QTableWidgetItem(QString::number(180.0)));
        tCMPar->setItem(8, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.5 * M_PI_4 + M_PI / 36.0))));
        tCMPar->setItem(8, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    case 8:
        /// Группа КР №8
        tCMPar->setItem(7, 0, new QTableWidgetItem(QString::number(8)));
        tCMPar->setItem(7, 1, new QTableWidgetItem(QString::number(700000.0)));
        tCMPar->setItem(7, 2, new QTableWidgetItem(QString::number(-700000.0)));
        tCMPar->setItem(7, 3, new QTableWidgetItem(QString::number(215.0)));
        tCMPar->setItem(7, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.0 * M_PI_4 + M_PI / 36.0))));
        tCMPar->setItem(7, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    case 7:
        /// Группа КР №7
        tCMPar->setItem(6, 0, new QTableWidgetItem(QString::number(7)));
        tCMPar->setItem(6, 1, new QTableWidgetItem(QString::number(-800000.0)));
        tCMPar->setItem(6, 2, new QTableWidgetItem(QString::number(-800000.0)));
        tCMPar->setItem(6, 3, new QTableWidgetItem(QString::number(250.0)));
        tCMPar->setItem(6, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.6 * M_PI_4 + M_PI / 36.0))));
        tCMPar->setItem(6, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    case 6:
        /// Группа КР №6
        tCMPar->setItem(5, 0, new QTableWidgetItem(QString::number(6)));
        tCMPar->setItem(5, 1, new QTableWidgetItem(QString::number(0.0)));
        tCMPar->setItem(5, 2, new QTableWidgetItem(QString::number(-800000.0)));
        tCMPar->setItem(5, 3, new QTableWidgetItem(QString::number(180.0)));
        tCMPar->setItem(5, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.0 + M_PI / 36.0))));
        tCMPar->setItem(5, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    case 5:
        /// Группа КР №5
        tCMPar->setItem(4, 0, new QTableWidgetItem(QString::number(5)));
        tCMPar->setItem(4, 1, new QTableWidgetItem(QString::number(-320000.0)));
        tCMPar->setItem(4, 2, new QTableWidgetItem(QString::number(-780000.0)));
        tCMPar->setItem(4, 3, new QTableWidgetItem(QString::number(250.0)));
        tCMPar->setItem(4, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.5 * M_PI_4 + M_PI / 36.0))));
        tCMPar->setItem(4, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    case 4:
        /// Группа КР №4
        tCMPar->setItem(3, 0, new QTableWidgetItem(QString::number(4)));
        tCMPar->setItem(3, 1, new QTableWidgetItem(QString::number(320000.0)));
        tCMPar->setItem(3, 2, new QTableWidgetItem(QString::number(-780000.0)));
        tCMPar->setItem(3, 3, new QTableWidgetItem(QString::number(180.0)));
        tCMPar->setItem(3, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.5 * M_PI_4 + M_PI / 36.0))));
        tCMPar->setItem(3, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    case 3:
        /// Группа КР №3
        tCMPar->setItem(2, 0, new QTableWidgetItem(QString::number(3)));
        tCMPar->setItem(2, 1, new QTableWidgetItem(QString::number(600000.0)));
        tCMPar->setItem(2, 2, new QTableWidgetItem(QString::number(-600000.0)));
        tCMPar->setItem(2, 3, new QTableWidgetItem(QString::number(215.0)));
        tCMPar->setItem(2, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.25 * M_PI_4 + M_PI / 36.0))));
        tCMPar->setItem(2, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    case 2:
        /// Группа КР №2
        tCMPar->setItem(1, 0, new QTableWidgetItem(QString::number(2)));
        tCMPar->setItem(1, 1, new QTableWidgetItem(QString::number(-700000.0)));
        tCMPar->setItem(1, 2, new QTableWidgetItem(QString::number(-700000.0)));
        tCMPar->setItem(1, 3, new QTableWidgetItem(QString::number(250.0)));
        tCMPar->setItem(1, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(M_PI_4 + M_PI / 36.0))));
        tCMPar->setItem(1, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    case 1:
        /// Группа КР №1
        tCMPar->setItem(0, 0, new QTableWidgetItem(QString::number(1)));
        tCMPar->setItem(0, 1, new QTableWidgetItem(QString::number(0.0)));
        tCMPar->setItem(0, 2, new QTableWidgetItem(QString::number(-700000.0)));
        tCMPar->setItem(0, 3, new QTableWidgetItem(QString::number(180.0)));
        tCMPar->setItem(0, 4, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.0 + M_PI / 36.0))));
        tCMPar->setItem(0, 5, new QTableWidgetItem(QString::number(CMInGroupCount)));

    default:
        break;
    }
}

void ParametersOfEtalons::initBGPar(int _count)
{
    tBGPar->setRowCount(_count);

    switch(_count)
    {
    case 5:
        /// БЦ №5
        tBGPar->setItem(4, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT + 5)));
        tBGPar->setItem(4, 1, new QTableWidgetItem(QString::number(-360000.0)));
        tBGPar->setItem(4, 2, new QTableWidgetItem(QString::number(-880000.0)));
        tBGPar->setItem(4, 3, new QTableWidgetItem(QString::number(250.0)));
        tBGPar->setItem(4, 4, new QTableWidgetItem(QString::number(250000.0)));
        tBGPar->setItem(4, 5, new QTableWidgetItem(QString::number(0.0)));

    case 4:
        /// БЦ №4
        tBGPar->setItem(3, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT + 4)));
        tBGPar->setItem(3, 1, new QTableWidgetItem(QString::number(360000.0)));
        tBGPar->setItem(3, 2, new QTableWidgetItem(QString::number(-880000.0)));
        tBGPar->setItem(3, 3, new QTableWidgetItem(QString::number(180.0)));
        tBGPar->setItem(3, 4, new QTableWidgetItem(QString::number(-250000.0)));
        tBGPar->setItem(3, 5, new QTableWidgetItem(QString::number(50000.0)));

    case 3:
        /// БЦ №3
        tBGPar->setItem(2, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT + 3)));
        tBGPar->setItem(2, 1, new QTableWidgetItem(QString::number(700000.0)));
        tBGPar->setItem(2, 2, new QTableWidgetItem(QString::number(-700000.0)));
        tBGPar->setItem(2, 3, new QTableWidgetItem(QString::number(215.0)));
        tBGPar->setItem(2, 4, new QTableWidgetItem(QString::number(-250000.0)));
        tBGPar->setItem(2, 5, new QTableWidgetItem(QString::number(0.0)));

    case 2:
        /// БЦ №2
        tBGPar->setItem(1, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT + 2)));
        tBGPar->setItem(1, 1, new QTableWidgetItem(QString::number(-800000.0)));
        tBGPar->setItem(1, 2, new QTableWidgetItem(QString::number(-800000.0)));
        tBGPar->setItem(1, 3, new QTableWidgetItem(QString::number(250.0)));
        tBGPar->setItem(1, 4, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(1, 5, new QTableWidgetItem(QString::number(250000.0)));

    case 1:
        /// БЦ №1
        tBGPar->setItem(0, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT + 1)));
        tBGPar->setItem(0, 1, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(0, 2, new QTableWidgetItem(QString::number(-800000.0)));
        tBGPar->setItem(0, 3, new QTableWidgetItem(QString::number(180.0)));
        tBGPar->setItem(0, 4, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(0, 5, new QTableWidgetItem(QString::number(0.0)));

    default:
        break;
    }
}

}
