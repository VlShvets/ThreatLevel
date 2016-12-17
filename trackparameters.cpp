#include "trackparameters.h"

namespace ThreatLevel
{

/// Класс виджета редактирования параметров трасс
TrackParameters::TrackParameters(QWidget *parent) : QWidget(parent)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("Количество групп КР: ")), 0, 0, 1, 1);

    QSlider *sGroupsOfCM = new QSlider(Qt::Horizontal);
    sGroupsOfCM->setRange(1, MAX_COUNT_GROUPS_OF_CM);
    sGroupsOfCM->setTickInterval(1);
    sGroupsOfCM->setValue(DEF_COUNT_GROUPS_OF_CM);
    sGroupsOfCM->setTickPosition(QSlider::TicksAbove);
    sGroupsOfCM->setFixedWidth(100);
    gridLayout->addWidget(sGroupsOfCM, 0, 1, 1, 1);

    lCountGroupsOfCM = new QLCDNumber(2);
    lCountGroupsOfCM->setSegmentStyle(QLCDNumber::Flat);
    lCountGroupsOfCM->setMode(QLCDNumber::Dec);
    lCountGroupsOfCM->display(DEF_COUNT_GROUPS_OF_CM);
    lCountGroupsOfCM->setFixedWidth(100);
    QObject::connect(sGroupsOfCM, SIGNAL(valueChanged(int)), lCountGroupsOfCM, SLOT(display(int)));
    QObject::connect(sGroupsOfCM, SIGNAL(valueChanged(int)), this, SLOT(countGroupsOfCMChanged(int)));
    gridLayout->addWidget(lCountGroupsOfCM, 0, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("Количество КР в группе: ")), 1, 0, 1, 1);

    QSlider *sCMinGroup = new QSlider(Qt::Horizontal);
    sCMinGroup->setRange(1, MAX_COUNT_CM_IN_GROUP);
    sCMinGroup->setTickInterval(1);
    sCMinGroup->setValue(DEF_COUNT_CM_IN_GROUP);
    sCMinGroup->setTickPosition(QSlider::TicksAbove);
    sCMinGroup->setFixedWidth(100);
    gridLayout->addWidget(sCMinGroup, 1, 1, 1, 1);

    lCountCMinGroup = new QLCDNumber(2);
    lCountCMinGroup->setSegmentStyle(QLCDNumber::Flat);
    lCountCMinGroup->setMode(QLCDNumber::Dec);
    lCountCMinGroup->display(DEF_COUNT_CM_IN_GROUP);
    lCountCMinGroup->setFixedWidth(100);
    QObject::connect(sCMinGroup, SIGNAL(valueChanged(int)), lCountCMinGroup, SLOT(display(int)));
    QObject::connect(sCMinGroup, SIGNAL(valueChanged(int)), this, SLOT(countCMinGroupChanged(int)));
    gridLayout->addWidget(lCountCMinGroup, 1, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("\tНачальные параметры КР:")), 2, 0, 1, 3);

    tCMPar = new QTableWidget(DEF_COUNT_GROUPS_OF_CM * DEF_COUNT_CM_IN_GROUP, 6, this);
    tCMPar->setHorizontalHeaderLabels(QStringList() << tr("№ трассы") << tr("Нач. коорд. X") << tr("Нач. коорд. Y") <<
                                      tr("Курс (град.)") << tr("Скорость") << tr("Ускорение"));
    gridLayout->addWidget(tCMPar, 3, 0, 1, 3);

    gridLayout->addWidget(new QLabel(tr("Количество БЦ: ")), 4, 0, 1, 1);

    QSlider *sCountBG = new QSlider(Qt::Horizontal);
    sCountBG->setRange(1, MAX_COUNT_BG);
    sCountBG->setTickInterval(1);
    sCountBG->setValue(DEF_COUNT_BG);
    sCountBG->setTickPosition(QSlider::TicksAbove);
    sCountBG->setFixedWidth(100);
    gridLayout->addWidget(sCountBG, 4, 1, 1, 1);

    lCountBG = new QLCDNumber(1);
    lCountBG->setSegmentStyle(QLCDNumber::Flat);
    lCountBG->setMode(QLCDNumber::Dec);
    lCountBG->display(DEF_COUNT_BG);
    lCountBG->setFixedWidth(100);
    QObject::connect(sCountBG, SIGNAL(valueChanged(int)), lCountBG, SLOT(display(int)));
    QObject::connect(sCountBG, SIGNAL(valueChanged(int)), this, SLOT(countBGChanged(int)));
    gridLayout->addWidget(lCountBG, 4, 2, 1, 1);

    gridLayout->addWidget(new QLabel(tr("\tНачальные параметры БЦ:")), 5, 0, 1, 3);

    tBGPar = new QTableWidget(DEF_COUNT_BG, 7, this);
    tBGPar->setHorizontalHeaderLabels(QStringList() << tr("№ трассы") << tr("Нач. коорд. X") << tr("Нач. коорд. Y") <<
                                         tr("Кон. коорд. X") << tr("Кон. коорд. Y") << tr("Скорость") << tr("Ускорение"));
    gridLayout->addWidget(tBGPar, 6, 0, 1, 3);

    gridLayout->addWidget(new QLabel(tr("Количество БЦ: ")), 4, 0, 1, 1);

    this->setLayout(gridLayout);

    initCMPar(DEF_COUNT_GROUPS_OF_CM, DEF_COUNT_CM_IN_GROUP);
    initBGPar(DEF_COUNT_BG);
}

TrackParameters::~TrackParameters()
{
    delete lCountGroupsOfCM;
    delete lCountCMinGroup;
    delete tCMPar;
    delete lCountBG;
    delete tBGPar;
}

/// Изменение количества групп КР
void TrackParameters::countGroupsOfCMChanged(int _count)
{
    tCMPar->setRowCount(_count * lCountCMinGroup->intValue());

    initCMPar(_count, lCountCMinGroup->intValue());
}

/// Изменение количества КР в группе
void TrackParameters::countCMinGroupChanged(int _count)
{
    tCMPar->setRowCount(lCountGroupsOfCM->intValue() * _count);

    initCMPar(lCountGroupsOfCM->intValue(), _count);
}

/// Изменение количества БЦ
void TrackParameters::countBGChanged(int _count)
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
            tCMPar->setItem(_countGroupsOfCM * i + 9, 1, new QTableWidgetItem(QString::number(-180000.0 - DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 9, 2, new QTableWidgetItem(QString::number(-440000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 9, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.5 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 9, 4, new QTableWidgetItem(QString::number(250.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 9, 5, new QTableWidgetItem(QString::number(-0.05)));

        case 9:
            /// Группа КР №9
            tCMPar->setItem(_countGroupsOfCM * i + 8, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 9)));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 1, new QTableWidgetItem(QString::number(180000.0 + DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 2, new QTableWidgetItem(QString::number(-440000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.5 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 4, new QTableWidgetItem(QString::number(180.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 8, 5, new QTableWidgetItem(QString::number(0.05)));

        case 8:
            /// Группа КР №8
            tCMPar->setItem(_countGroupsOfCM * i + 7, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 8)));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 1, new QTableWidgetItem(QString::number(350000.0 + DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 2, new QTableWidgetItem(QString::number(-350000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.0 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 4, new QTableWidgetItem(QString::number(215.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 7, 5, new QTableWidgetItem(QString::number(0.0)));

        case 7:
            /// Группа КР №7
            tCMPar->setItem(_countGroupsOfCM * i + 6, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 7)));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 1, new QTableWidgetItem(QString::number(-400000.0 - DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 2, new QTableWidgetItem(QString::number(-400000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(M_PI_4 + M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 4, new QTableWidgetItem(QString::number(250.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 6, 5, new QTableWidgetItem(QString::number(0.0)));

        case 6:
            /// Группа КР №6
            tCMPar->setItem(_countGroupsOfCM * i + 5, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 6)));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 1, new QTableWidgetItem(QString::number(0.0 + DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 2, new QTableWidgetItem(QString::number(-400000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.0 + M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 4, new QTableWidgetItem(QString::number(180.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 5, 5, new QTableWidgetItem(QString::number(0.0)));

        case 5:
            /// Группа КР №5
            tCMPar->setItem(_countGroupsOfCM * i + 4, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 5)));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 1, new QTableWidgetItem(QString::number(-160000.0 - DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 2, new QTableWidgetItem(QString::number(-390000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(0.5 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 4, new QTableWidgetItem(QString::number(250.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 4, 5, new QTableWidgetItem(QString::number(-0.05)));

        case 4:
            /// Группа КР №4
            tCMPar->setItem(_countGroupsOfCM * i + 3, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 4)));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 1, new QTableWidgetItem(QString::number(160000.0 + DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 2, new QTableWidgetItem(QString::number(-390000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.5 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 4, new QTableWidgetItem(QString::number(180.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 3, 5, new QTableWidgetItem(QString::number(0.05)));

        case 3:
            /// Группа КР №3
            tCMPar->setItem(_countGroupsOfCM * i + 2, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 3)));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 1, new QTableWidgetItem(QString::number(300000.0 + DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 2, new QTableWidgetItem(QString::number(-300000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(7.0 * M_PI_4 +
                                                                                                                M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 4, new QTableWidgetItem(QString::number(215.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 2, 5, new QTableWidgetItem(QString::number(0.0)));

        case 2:
            /// Группа КР №2
            tCMPar->setItem(_countGroupsOfCM * i + 1, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 2)));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 1, new QTableWidgetItem(QString::number(-350000.0 - DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 2, new QTableWidgetItem(QString::number(-350000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 3, new QTableWidgetItem(QString::number(qRadiansToDegrees(M_PI_4 + M_PI / 36.0))));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 4, new QTableWidgetItem(QString::number(250.0)));
            tCMPar->setItem(_countGroupsOfCM * i + 1, 5, new QTableWidgetItem(QString::number(0.0)));

        case 1:
            /// Группа КР №1
            tCMPar->setItem(_countGroupsOfCM * i, 0, new QTableWidgetItem(QString::number(_countGroupsOfCM * i + 1)));
            tCMPar->setItem(_countGroupsOfCM * i, 1, new QTableWidgetItem(QString::number(0.0 + DIST_BETWEEN_TRACKS * (i / 3))));
            tCMPar->setItem(_countGroupsOfCM * i, 2, new QTableWidgetItem(QString::number(-350000.0 - DIST_BETWEEN_TRACKS * (i % 3))));
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
        tBGPar->setItem(4, 0, new QTableWidgetItem(QString::number(MAX_COUNT_GROUPS_OF_CM * MAX_COUNT_CM_IN_GROUP + 5)));
        tBGPar->setItem(4, 1, new QTableWidgetItem(QString::number(-180000.0)));
        tBGPar->setItem(4, 2, new QTableWidgetItem(QString::number(-440000.0)));
        tBGPar->setItem(4, 3, new QTableWidgetItem(QString::number(250000.0)));
        tBGPar->setItem(4, 4, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(4, 5, new QTableWidgetItem(QString::number(250.0)));
        tBGPar->setItem(4, 6, new QTableWidgetItem(QString::number(-0.05)));

    case 4:
        /// БЦ №4
        tBGPar->setItem(3, 0, new QTableWidgetItem(QString::number(MAX_COUNT_GROUPS_OF_CM * MAX_COUNT_CM_IN_GROUP + 4)));
        tBGPar->setItem(3, 1, new QTableWidgetItem(QString::number(180000.0)));
        tBGPar->setItem(3, 2, new QTableWidgetItem(QString::number(-440000.0)));
        tBGPar->setItem(3, 3, new QTableWidgetItem(QString::number(-250000.0)));
        tBGPar->setItem(3, 4, new QTableWidgetItem(QString::number(50000.0)));
        tBGPar->setItem(3, 5, new QTableWidgetItem(QString::number(180.0)));
        tBGPar->setItem(3, 6, new QTableWidgetItem(QString::number(0.05)));

    case 3:
        /// БЦ №3
        tBGPar->setItem(2, 0, new QTableWidgetItem(QString::number(MAX_COUNT_GROUPS_OF_CM * MAX_COUNT_CM_IN_GROUP + 3)));
        tBGPar->setItem(2, 1, new QTableWidgetItem(QString::number(350000.0)));
        tBGPar->setItem(2, 2, new QTableWidgetItem(QString::number(-350000.0)));
        tBGPar->setItem(2, 3, new QTableWidgetItem(QString::number(-250000.0)));
        tBGPar->setItem(2, 4, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(2, 5, new QTableWidgetItem(QString::number(215.0)));
        tBGPar->setItem(2, 6, new QTableWidgetItem(QString::number(0.0)));

    case 2:
        /// БЦ №2
        tBGPar->setItem(1, 0, new QTableWidgetItem(QString::number(MAX_COUNT_GROUPS_OF_CM * MAX_COUNT_CM_IN_GROUP + 2)));
        tBGPar->setItem(1, 1, new QTableWidgetItem(QString::number(-400000.0)));
        tBGPar->setItem(1, 2, new QTableWidgetItem(QString::number(-400000.0)));
        tBGPar->setItem(1, 3, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(1, 4, new QTableWidgetItem(QString::number(250000.0)));
        tBGPar->setItem(1, 5, new QTableWidgetItem(QString::number(250.0)));
        tBGPar->setItem(1, 6, new QTableWidgetItem(QString::number(0.0)));

    case 1:
        /// БЦ №1
        tBGPar->setItem(0, 0, new QTableWidgetItem(QString::number(MAX_COUNT_GROUPS_OF_CM * MAX_COUNT_CM_IN_GROUP + 1)));
        tBGPar->setItem(0, 1, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(0, 2, new QTableWidgetItem(QString::number(-400000.0)));
        tBGPar->setItem(0, 3, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(0, 4, new QTableWidgetItem(QString::number(0.0)));
        tBGPar->setItem(0, 5, new QTableWidgetItem(QString::number(180.0)));
        tBGPar->setItem(0, 6, new QTableWidgetItem(QString::number(0.0)));

    default:
        break;
    }
}

}
