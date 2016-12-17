#include "results.h"

namespace ThreatLevel
{

int Area::sumCMcount;
int Area::maxSumCMcount;

int Area::sumBGcount;
int Area::maxSumBGcount;

int Area::sumTrackCount;
int Area::maxSumTrackCount;

int Track::numTrackMinErrTime;

/// Класс виджета отображения результатов
Results::Results(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(new QLabel(tr("Позиционные районы\t\\\tТрассы\t\tКР + БЦ = ")));

    lcdMaxSumTrack = new QLCDNumber(this);
    lcdMaxSumTrack->setSegmentStyle(QLCDNumber::Flat);
    lcdMaxSumTrack->setDigitCount(LCD_DIGIT_COUNT);
    hLayout->addWidget(lcdMaxSumTrack);

    vLayout->addLayout(hLayout);

    tResults = new QTableWidget(this);
    tResults->setSelectionMode(QAbstractItemView::NoSelection);
    vLayout->addWidget(tResults);

    this->setLayout(vLayout);
}

Results::~Results()
{
    delete tResults;
    delete lcdMaxSumTrack;
}

/// Загрузка результатов
void Results::loadTable(const QVector<Area> &_area, const QVector<Track> &_track)
{
    tResults->setRowCount(_area.count() + 1);
    tResults->setColumnCount(_track.count() + 7);
    tResults->setHorizontalHeaderLabels(QStringList() << tr("КР") << tr("max КР") << tr("БЦ") << tr("max БЦ") <<
                                        tr("КР + БЦ") << tr("max КР + БЦ") << tr("СКО"));

    for(int i = 0; i < _area.count(); ++i)
    {
        /// Количество ассоциированных с ПР крылатых ракет
        tResults->setItem(i, 0, new QTableWidgetItem(QString::number(_area.at(i).CMcount)));
        tResults->item(i, 0)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Максимальное количество ассоциированных с ПР крылатых ракет
        tResults->setItem(i, 1, new QTableWidgetItem(QString::number(_area.at(i).maxCMcount)));
        tResults->item(i, 1)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Количество ассоциированных с ПР баллистических целей
        tResults->setItem(i, 2, new QTableWidgetItem(QString::number(_area.at(i).BGcount)));
        tResults->item(i, 2)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Максимальное количество ассоциированных с ПР баллистических целей
        tResults->setItem(i, 3, new QTableWidgetItem(QString::number(_area.at(i).maxBGcount)));
        tResults->item(i, 3)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Количественный состав налета по ПР
        tResults->setItem(i, 4, new QTableWidgetItem(QString::number(_area.at(i).numTrack.count())));
        tResults->item(i, 4)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Максимальный количественный состав налета по ПР
        tResults->setItem(i, 5, new QTableWidgetItem(QString::number(_area.at(i).maxTrackCount)));
        tResults->item(i, 5)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Cреднеквадратическая разность времени поражения с погрешностью и точного времени поражения
        tResults->setItem(i, 6, new QTableWidgetItem(QString::number(_area.at(i).rmsDiffTime)));
        tResults->item(i, 6)->setBackgroundColor(QColor(255, 0, 0, 25));

        /// Время поражения ПР с погрешностью и номер трассы
        for(int j = 0; j < _area.at(i).numTrack.count(); ++j)
        {
            tResults->setItem(i, j + 7, new QTableWidgetItem(QString::number(_track.at(_area.at(i).numTrack.at(j)).errTime) + " (" +
                                                             QString::number(_area.at(i).numTrack.at(j) + 1) + ")"));
        }

        /// Очистка неиспользующихся ячеек времени поражения ПР с погрешностью
        for(int j = _area.at(i).numTrack.count(); j < _track.count() + 6; ++j)
            tResults->takeItem(i, j + 7);
    }

    if(Track::numTrackMinErrTime != -1)
    {
        /// Суммарное количество крылатых ракет
        tResults->setItem(_area.count(), 0, new QTableWidgetItem(QString::number(Area::sumCMcount)));
        tResults->item(_area.count(), 0)->setBackgroundColor(QColor(255, 0, 0, 100));

        /// Максимальное суммарное количество крылатых ракет
        tResults->setItem(_area.count(), 1, new QTableWidgetItem(QString::number(Area::maxSumCMcount)));
        tResults->item(_area.count(), 1)->setBackgroundColor(QColor(255, 0, 0, 100));

        /// Суммарное количество баллистических целей
        tResults->setItem(_area.count(), 2, new QTableWidgetItem(QString::number(Area::sumBGcount)));
        tResults->item(_area.count(), 2)->setBackgroundColor(QColor(255, 0, 0, 100));

        /// Максимальное суммарное количество баллистических целей
        tResults->setItem(_area.count(), 3, new QTableWidgetItem(QString::number(Area::maxSumBGcount)));
        tResults->item(_area.count(), 3)->setBackgroundColor(QColor(255, 0, 0, 100));

        /// Количественный состав налета по всем ПР
        tResults->setItem(_area.count(), 4, new QTableWidgetItem(QString::number(Area::sumTrackCount)));
        tResults->item(_area.count(), 4)->setBackgroundColor(QColor(255, 0, 0, 100));

        /// Максимальный количественный состав налета по всем ПР
        tResults->setItem(_area.count(), 5, new QTableWidgetItem(QString::number(Area::maxSumTrackCount)));
        tResults->item(_area.count(), 5)->setBackgroundColor(QColor(255, 0, 0, 100));

        /// Минимальное время поражения ПР с погрешностью
        tResults->setItem(_area.count(), 7, new QTableWidgetItem(QString::number(_track.at(Track::numTrackMinErrTime).errTime)));
        tResults->item(_area.count(), 7)->setBackgroundColor(QColor(255, 0, 0, 25));
    }

    /// Максимальный количественный состав налета
    lcdMaxSumTrack->display(Area::sumTrackCount);
}

/// Очистка таблицы результатов
void Results::resetTable()
{
    tResults->setRowCount(0);
    tResults->setColumnCount(0);
}

}
