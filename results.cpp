#include "results.h"

namespace ThreatLevel
{

int Area::CMSumCount;
int Area::CMMaxSumCount;

int Area::BGSumCount;
int Area::BGMaxSumCount;

int Area::trackSumCount;
int Area::trackMaxSumCount;

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
void Results::loadTable(const QMap <int, Area> &_areas, const QMap <int, Track> &_tracks)
{
    tResults->setRowCount(_areas.count() + 1);
    tResults->setColumnCount(_tracks.count() + 8);
    tResults->setHorizontalHeaderLabels(QStringList() << tr("№ ПР") << tr("КР") << tr("max КР") <<
                                        tr("БЦ") << tr("max БЦ") << tr("КР + БЦ") << tr("max КР + БЦ") << tr("СКО"));

    QMap <int, Area>::const_iterator area =_areas.constBegin();
    for(int i = 0; area != _areas.end(); ++area, ++i)
    {
        /// Номер ПР
        tResults->setItem(i, 0, new QTableWidgetItem(QString::number(area.key())));

        /// Количество ассоциированных с ПР крылатых ракет
        tResults->setItem(i, 1, new QTableWidgetItem(QString::number(area.value().CMCount)));
        tResults->item(i, 1)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Максимальное количество ассоциированных с ПР крылатых ракет
        tResults->setItem(i, 2, new QTableWidgetItem(QString::number(area.value().CMMaxCount)));
        tResults->item(i, 2)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Количество ассоциированных с ПР баллистических целей
        tResults->setItem(i, 3, new QTableWidgetItem(QString::number(area.value().BGCount)));
        tResults->item(i, 3)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Максимальное количество ассоциированных с ПР баллистических целей
        tResults->setItem(i, 4, new QTableWidgetItem(QString::number(area.value().BGMaxCount)));
        tResults->item(i, 4)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Количественный состав налета по ПР
        tResults->setItem(i, 5, new QTableWidgetItem(QString::number(area.value().trackCount)));
        tResults->item(i, 5)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Максимальный количественный состав налета по ПР
        tResults->setItem(i, 6, new QTableWidgetItem(QString::number(area.value().trackMaxCount)));
        tResults->item(i, 6)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Cреднеквадратическая разность времени поражения с погрешностью и точного времени поражения
        tResults->setItem(i, 7, new QTableWidgetItem(QString::number(area.value().rmsDiffTime)));
        tResults->item(i, 7)->setBackgroundColor(QColor(255, 0, 0, 25));

        /// Время поражения ПР с погрешностью и номер трассы
        for(int j = 0; j < area.value().numTrack.count(); ++j)
        {
            tResults->setItem(i, j + 8, new QTableWidgetItem(QString::number(_tracks[area.value().numTrack.at(j)].errTime) + " (" +
                                                             QString::number(area.value().numTrack.at(j)) + ")"));
        }

        /// Очистка неиспользующихся ячеек времени поражения ПР с погрешностью
        for(int j = area.value().numTrack.count(); j < _tracks.count() + 8; ++j)
            tResults->takeItem(i, j + 8);
    }

    /// Знак суммы
    tResults->setItem(_areas.count(), 0, new QTableWidgetItem(tr("Σ")));

    /// Суммарное количество крылатых ракет
    tResults->setItem(_areas.count(), 1, new QTableWidgetItem(QString::number(Area::CMSumCount)));
    tResults->item(_areas.count(), 1)->setBackgroundColor(QColor(255, 0, 0, 100));

    /// Максимальное суммарное количество крылатых ракет
    tResults->setItem(_areas.count(), 2, new QTableWidgetItem(QString::number(Area::CMMaxSumCount)));
    tResults->item(_areas.count(), 2)->setBackgroundColor(QColor(255, 0, 0, 100));

    /// Суммарное количество баллистических целей
    tResults->setItem(_areas.count(), 3, new QTableWidgetItem(QString::number(Area::BGSumCount)));
    tResults->item(_areas.count(), 3)->setBackgroundColor(QColor(255, 0, 0, 100));

    /// Максимальное суммарное количество баллистических целей
    tResults->setItem(_areas.count(), 4, new QTableWidgetItem(QString::number(Area::BGMaxSumCount)));
    tResults->item(_areas.count(), 4)->setBackgroundColor(QColor(255, 0, 0, 100));

    /// Количественный состав налета по всем ПР
    tResults->setItem(_areas.count(), 5, new QTableWidgetItem(QString::number(Area::trackSumCount)));
    tResults->item(_areas.count(), 5)->setBackgroundColor(QColor(255, 0, 0, 100));

    /// Максимальный количественный состав налета по всем ПР
    tResults->setItem(_areas.count(), 6, new QTableWidgetItem(QString::number(Area::trackMaxSumCount)));
    tResults->item(_areas.count(), 6)->setBackgroundColor(QColor(255, 0, 0, 100));

    /// Минимальное время поражения ПР с погрешностью
    tResults->setItem(_areas.count(), 8, new QTableWidgetItem(QString::number(_tracks[Track::numTrackMinErrTime].errTime)));
    tResults->item(_areas.count(), 8)->setBackgroundColor(QColor(255, 0, 0, 25));

    /// Максимальный количественный состав налета
    lcdMaxSumTrack->display(Area::trackSumCount);
}

/// Очистка таблицы результатов
void Results::resetTable()
{
    tResults->setRowCount(0);
    tResults->setColumnCount(0);
}

}
