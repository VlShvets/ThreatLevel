#include "trackparameters.h"

namespace ThreatLevel
{

/// Класс виджета редактирования параметров трасс
TrackParameters::TrackParameters(QWidget *parent) : QWidget(parent)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество групп КР: ")), 0, 0, 1, 1);

    QSlider *sGroupsOfCMCount = new QSlider(Qt::Horizontal);
    sGroupsOfCMCount->setRange(1, GROUPS_OF_CM_MAX_COUNT);
    sGroupsOfCMCount->setTickInterval(1);
    sGroupsOfCMCount->setValue(GROUPS_OF_CM_DEF_COUNT);
    sGroupsOfCMCount->setTickPosition(QSlider::TicksAbove);
    sGroupsOfCMCount->setFixedWidth(100);
    gridLayout->addWidget(sGroupsOfCMCount, 0, 1, 1, 1);

    lGroupsOfCMCount = new QLCDNumber(2);
    lGroupsOfCMCount->setSegmentStyle(QLCDNumber::Flat);
    lGroupsOfCMCount->setMode(QLCDNumber::Dec);
    lGroupsOfCMCount->display(GROUPS_OF_CM_DEF_COUNT);
    lGroupsOfCMCount->setFixedWidth(100);
    QObject::connect(sGroupsOfCMCount, SIGNAL(valueChanged(int)), lGroupsOfCMCount, SLOT(display(int)));
    QObject::connect(sGroupsOfCMCount, SIGNAL(valueChanged(int)), this, SLOT(groupsOfCMCountChanged(int)));
    gridLayout->addWidget(lGroupsOfCMCount, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("Количество КР в группе: ")), 1, 0, 1, 1);

    QSlider *sCMInGroupCount = new QSlider(Qt::Horizontal);
    sCMInGroupCount->setRange(1, CM_IN_GROUP_MAX_COUNT);
    sCMInGroupCount->setTickInterval(1);
    sCMInGroupCount->setValue(CM_IN_GROUP_DEF_COUNT);
    sCMInGroupCount->setTickPosition(QSlider::TicksAbove);
    sCMInGroupCount->setFixedWidth(100);
    gridLayout->addWidget(sCMInGroupCount, 1, 1, 1, 1);

    lCMInGroupCount = new QLCDNumber(2);
    lCMInGroupCount->setSegmentStyle(QLCDNumber::Flat);
    lCMInGroupCount->setMode(QLCDNumber::Dec);
    lCMInGroupCount->display(CM_IN_GROUP_DEF_COUNT);
    lCMInGroupCount->setFixedWidth(100);
    QObject::connect(sCMInGroupCount, SIGNAL(valueChanged(int)), lCMInGroupCount, SLOT(display(int)));
    QObject::connect(sCMInGroupCount, SIGNAL(valueChanged(int)), this, SLOT(CMInGroupCountChanged(int)));
    gridLayout->addWidget(lCMInGroupCount, 1, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("\tНачальные параметры КР:")), 2, 0, 1, 3);

    tCMPar = new QTableWidget(GROUPS_OF_CM_DEF_COUNT * CM_IN_GROUP_DEF_COUNT, 6, this);
    tCMPar->setHorizontalHeaderLabels(QStringList() << tr("№ трассы") << tr("Нач. коорд. X") << tr("Нач. коорд. Y") <<
                                      tr("Курс (град.)") << tr("Скорость") << tr("Ускорение"));
    gridLayout->addWidget(tCMPar, 3, 0, 1, 3);

    gridLayout->addWidget(new QLabel(tr("Количество БЦ: ")), 4, 0, 1, 1);

    QSlider *sBGCount = new QSlider(Qt::Horizontal);
    sBGCount->setRange(0, BG_MAX_COUNT);
    sBGCount->setTickInterval(1);
    sBGCount->setValue(BG_DEF_COUNT);
    sBGCount->setTickPosition(QSlider::TicksAbove);
    sBGCount->setFixedWidth(100);
    gridLayout->addWidget(sBGCount, 4, 1, 1, 1);

    lBGCount = new QLCDNumber(1);
    lBGCount->setSegmentStyle(QLCDNumber::Flat);
    lBGCount->setMode(QLCDNumber::Dec);
    lBGCount->display(BG_DEF_COUNT);
    lBGCount->setFixedWidth(100);
    QObject::connect(sBGCount, SIGNAL(valueChanged(int)), lBGCount, SLOT(display(int)));
    QObject::connect(sBGCount, SIGNAL(valueChanged(int)), this, SLOT(BGCountChanged(int)));
    gridLayout->addWidget(lBGCount, 4, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("\tНачальные параметры БЦ:")), 5, 0, 1, 3);

    tBGPar = new QTableWidget(BG_DEF_COUNT, 7, this);
    tBGPar->setHorizontalHeaderLabels(QStringList() << tr("№ трассы") << tr("Нач. коорд. X") << tr("Нач. коорд. Y") <<
                                         tr("Кон. коорд. X") << tr("Кон. коорд. Y") << tr("Скорость") << tr("Ускорение"));
    gridLayout->addWidget(tBGPar, 6, 0, 1, 3);

    gridLayout->addWidget(new QLabel(tr("Количество БЦ: ")), 4, 0, 1, 1);

    this->setLayout(gridLayout);

    initCMPar(GROUPS_OF_CM_DEF_COUNT, CM_IN_GROUP_DEF_COUNT);
    initBGPar(BG_DEF_COUNT);
}

TrackParameters::~TrackParameters()
{
    delete lGroupsOfCMCount;
    delete lCMInGroupCount;
    delete tCMPar;
    delete lBGCount;
    delete tBGPar;
}

/// Изменение количества групп КР
void TrackParameters::groupsOfCMCountChanged(int _count)
{
    tCMPar->setRowCount(_count * lCMInGroupCount->intValue());

    initCMPar(_count, lCMInGroupCount->intValue());
}

/// Изменение количества КР в группе
void TrackParameters::CMInGroupCountChanged(int _count)
{
    tCMPar->setRowCount(lGroupsOfCMCount->intValue() * _count);

    initCMPar(lGroupsOfCMCount->intValue(), _count);
}

/// Изменение количества БЦ
void TrackParameters::BGCountChanged(int _count)
{
    tBGPar->setRowCount(_count);

    initBGPar(_count);
}

/// Начальная инициализация параметров КР
void TrackParameters::initCMPar(int _countGroupsOfCM, int _countCMinGroup)
{
    for(int i = 0; i < _countCMinGroup; ++i)
    {
        switch(_countGroupsOfCM)
        {
        case 10:
            /// Группа КР №10
            tCMPar->setItem(_countGroupsOfCM * i + 9, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 10)));
            tCMPar->setItem(_countGroupsOfCM * i + 9, 1, new QTableWidgetItem(QString::number(-360000.0 - CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 9, 2, new QTableWidgetItem(QString::number(-880000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 9, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.5 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 9, 4, new QTableWidgetItem(QString::number(250.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 9, 5, new QTableWidgetItem(QString::number(-0.05)));

        case 9:
            /// Группа КР №9
            tCMPar->setItem(_countGroupsOfCM * i + 8, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 9)));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 1, new QTableWidgetItem(QString::number(360000.0 + CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 2, new QTableWidgetItem(QString::number(-880000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.5 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 4, new QTableWidgetItem(QString::number(180.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 5, new QTableWidgetItem(QString::number(0.05)));

        case 8:
            /// Группа КР №8
            tCMPar->setItem(_countGroupsOfCM * i + 7, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 8)));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 1, new QTableWidgetItem(QString::number(700000.0 + CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 2, new QTableWidgetItem(QString::number(-700000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.0 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 4, new QTableWidgetItem(QString::number(215.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 5, new QTableWidgetItem(QString::number(0.0)));

        case 7:
            /// Группа КР №7
            tCMPar->setItem(_countGroupsOfCM * i + 6, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 7)));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 1, new QTableWidgetItem(QString::number(-800000.0 - CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 2, new QTableWidgetItem(QString::number(-800000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(M_PI_4 + M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 4, new QTableWidgetItem(QString::number(250.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 5, new QTableWidgetItem(QString::number(0.0)));

        case 6:
            /// Группа КР №6
            tCMPar->setItem(_countGroupsOfCM * i + 5, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 6)));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 1, new QTableWidgetItem(QString::number(0.0 + CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 2, new QTableWidgetItem(QString::number(-800000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.0 + M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 4, new QTableWidgetItem(QString::number(180.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 5, new QTableWidgetItem(QString::number(0.0)));

        case 5:
            /// Группа КР №5
            tCMPar->setItem(_countGroupsOfCM * i + 4, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 5)));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 1, new QTableWidgetItem(QString::number(-320000.0 - CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 2, new QTableWidgetItem(QString::number(-780000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.5 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 4, new QTableWidgetItem(QString::number(250.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 5, new QTableWidgetItem(QString::number(-0.05)));

        case 4:
            /// Группа КР №4
            tCMPar->setItem(_countGroupsOfCM * i + 3, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 4)));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 1, new QTableWidgetItem(QString::number(320000.0 + CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 2, new QTableWidgetItem(QString::number(-780000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.5 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 4, new QTableWidgetItem(QString::number(180.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 5, new QTableWidgetItem(QString::number(0.05)));

        case 3:
            /// Группа КР №3
            tCMPar->setItem(_countGroupsOfCM * i + 2, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 3)));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 1, new QTableWidgetItem(QString::number(600000.0 + CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 2, new QTableWidgetItem(QString::number(-600000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.0 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 4, new QTableWidgetItem(QString::number(215.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 5, new QTableWidgetItem(QString::number(0.0)));

        case 2:
            /// Группа КР №2
            tCMPar->setItem(_countGroupsOfCM * i + 1, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 2)));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 1, new QTableWidgetItem(QString::number(-700000.0 - CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 2, new QTableWidgetItem(QString::number(-700000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(M_PI_4 + M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 4, new QTableWidgetItem(QString::number(250.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 5, new QTableWidgetItem(QString::number(0.0)));

        case 1:
            /// Группа КР №1
            tCMPar->setItem(_countGroupsOfCM * i, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 1)));
            tCMPar->setItem(_countGroupsOfCM * i, 1, new QTableWidgetItem(QString::number(0.0 + CM_IN_GROUP_DISTANCE * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i, 2, new QTableWidgetItem(QString::number(-700000.0 - CM_IN_GROUP_DISTANCE * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.0 + M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i, 4, new QTableWidgetItem(QString::number(180.0)));
            tCMPar->setItem(_countGroupsOfCM * i, 5, new QTableWidgetItem(QString::number(0.0)));

        default:
            break;
        }
    }
}

void TrackParameters::initBGPar(int _countBG)
{
    switch (_countBG) {
    case 5:
        /// БЦ №5
        tBGPar->setItem(4, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT * CM_IN_GROUP_MAX_COUNT + 5)));
        tBGPar->setItem(4, 1, new QTableWidgetItem(QString::number(-360000.0)));
        tBGPar->setItem(4, 2, new QTableWidgetItem(QString::number(-880000.0)));
        tBGPar->setItem(4, 3, new QTableWidgetItem(QString::number(250000.0)));
        tBGPar->setItem(4, 4, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(4, 5, new QTableWidgetItem(QString::number(250.0)));
        tBGPar->setItem(4, 6, new QTableWidgetItem(QString::number(-0.05)));

    case 4:
        /// БЦ №4
        tBGPar->setItem(3, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT * CM_IN_GROUP_MAX_COUNT + 4)));
        tBGPar->setItem(3, 1, new QTableWidgetItem(QString::number(360000.0)));
        tBGPar->setItem(3, 2, new QTableWidgetItem(QString::number(-880000.0)));
        tBGPar->setItem(3, 3, new QTableWidgetItem(QString::number(-250000.0)));
        tBGPar->setItem(3, 4, new QTableWidgetItem(QString::number(50000.0)));
        tBGPar->setItem(3, 5, new QTableWidgetItem(QString::number(180.0)));
        tBGPar->setItem(3, 6, new QTableWidgetItem(QString::number(0.05)));

    case 3:
        /// БЦ №3
        tBGPar->setItem(2, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT * CM_IN_GROUP_MAX_COUNT + 3)));
        tBGPar->setItem(2, 1, new QTableWidgetItem(QString::number(700000.0)));
        tBGPar->setItem(2, 2, new QTableWidgetItem(QString::number(-700000.0)));
        tBGPar->setItem(2, 3, new QTableWidgetItem(QString::number(-250000.0)));
        tBGPar->setItem(2, 4, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(2, 5, new QTableWidgetItem(QString::number(215.0)));
        tBGPar->setItem(2, 6, new QTableWidgetItem(QString::number(0.0)));

    case 2:
        /// БЦ №2
        tBGPar->setItem(1, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT * CM_IN_GROUP_MAX_COUNT + 2)));
        tBGPar->setItem(1, 1, new QTableWidgetItem(QString::number(-800000.0)));
        tBGPar->setItem(1, 2, new QTableWidgetItem(QString::number(-800000.0)));
        tBGPar->setItem(1, 3, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(1, 4, new QTableWidgetItem(QString::number(250000.0)));
        tBGPar->setItem(1, 5, new QTableWidgetItem(QString::number(250.0)));
        tBGPar->setItem(1, 6, new QTableWidgetItem(QString::number(0.0)));

    case 1:
        /// БЦ №1
        tBGPar->setItem(0, 0, new QTableWidgetItem(QString::number(GROUPS_OF_CM_MAX_COUNT * CM_IN_GROUP_MAX_COUNT + 1)));
        tBGPar->setItem(0, 1, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(0, 2, new QTableWidgetItem(QString::number(-800000.0)));
        tBGPar->setItem(0, 3, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(0, 4, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(0, 5, new QTableWidgetItem(QString::number(180.0)));
        tBGPar->setItem(0, 6, new QTableWidgetItem(QString::number(0.0)));

    default:
        break;
    }
}

}
