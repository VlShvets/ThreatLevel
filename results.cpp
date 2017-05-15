#include "results.h"

namespace ThreatLevel
{

int Area::detectTracksCount;
int Area::idenTracksMaxCount;

int Area::CMSumCount;
int Area::CMMaxSumCount;

int Area::BGSumCount;
int Area::BGMaxSumCount;

int Area::raidSumCount;
int Area::raidMaxSumCount;

int Track::numTrackMinErrTime;

/// Класс виджета отображения таблицы результатов
Results::Results(GraphOfTracksCount *_graphOfTrackCount, QWidget *_parent)
    : QWidget(_parent), graphOfTrackCount(_graphOfTrackCount), areas(NULL), tracks(NULL)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(new QLabel(tr("ЗКВ\t\\\tТрассы\tКР + БЦ = ")));

    /// Дисплей отображения числа количественного состава налета по всем ЗКВ
    lcdMaxSumTrack = new QLCDNumber(this);
    lcdMaxSumTrack->setSegmentStyle(QLCDNumber::Flat);
    lcdMaxSumTrack->setDigitCount(LCD_DIGIT_COUNT);
    hLayout->addWidget(lcdMaxSumTrack);

    vLayout->addLayout(hLayout);

    /// Таблица результатов
    tResults = new QTableWidget(this);
    tResults->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tResults->setSelectionMode(QAbstractItemView::SingleSelection);
    tResults->setSelectionBehavior(QAbstractItemView::SelectRows);
    QObject::connect(tResults->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                     this                      , SLOT(tableSelectionChanged(QItemSelection, QItemSelection)));
    vLayout->addWidget(tResults);

    this->setLayout(vLayout);

    startTimer(RESULTS_INTERVAL);
}

Results::~Results()
{
    resetTable();

    delete tResults;
    delete lcdMaxSumTrack;
}

/// Очистка таблицы результатов
void Results::resetTable()
{
    areas   = NULL;
    tracks  = NULL;

    tResults->setRowCount(0);
    tResults->setColumnCount(0);
}

void Results::timerEvent(QTimerEvent *)
{
    if(!areas || !tracks)
        return;

    tResults->setRowCount(areas->count() + 1);
    tResults->setColumnCount(Area::idenTracksMaxCount + 8);
    tResults->setHorizontalHeaderLabels(QStringList() << tr("№ ЗКВ") << tr("КР") << tr("max КР") <<
                                        tr("БЦ") << tr("max БЦ") << tr("КР + БЦ") << tr("max КР + БЦ") << tr("СКО"));

    QMap <int, Area>::const_iterator area = areas->constBegin();
    for(int i = 0; area != areas->end(); ++area, ++i)
    {
        /// Номер ЗКВ
        tResults->setItem(i, 0, new QTableWidgetItem(QString::number(area.key())));

        /// --------------------------------------------------
        /// Результирующие параметры по конкретному ЗКВ
        /// --------------------------------------------------

        /// Количество идентифицированных с ЗКВ крылатых ракет
        tResults->setItem(i, 1, new QTableWidgetItem(QString::number(area.value().CMCount)));
        tResults->item(i, 1)->setBackgroundColor(QColor(255, 0, 0, 25));

        /// Максимальное количество идентифицированных с ЗКВ крылатых ракет
        tResults->setItem(i, 2, new QTableWidgetItem(QString::number(area.value().CMMaxCount)));
        tResults->item(i, 2)->setBackgroundColor(QColor(255, 0, 0, 25));

        /// Количество идентифицированных с ЗКВ баллистических целей
        tResults->setItem(i, 3, new QTableWidgetItem(QString::number(area.value().BGCount)));
        tResults->item(i, 3)->setBackgroundColor(QColor(255, 0, 0, 25));

        /// Максимальное количество идентифицированных с ЗКВ баллистических целей
        tResults->setItem(i, 4, new QTableWidgetItem(QString::number(area.value().BGMaxCount)));
        tResults->item(i, 4)->setBackgroundColor(QColor(255, 0, 0, 25));

        /// Количественный состав налета по ЗКВ с учетом эквивалента БЦ
        tResults->setItem(i, 5, new QTableWidgetItem(QString::number(area.value().raidCount)));
        tResults->item(i, 5)->setBackgroundColor(QColor(255, 0, 0, 25));

        /// Максимальный количественный состав налета по ЗКВ с учетом эквивалента БЦ
        tResults->setItem(i, 6, new QTableWidgetItem(QString::number(area.value().raidMaxCount)));
        tResults->item(i, 6)->setBackgroundColor(QColor(255, 0, 0, 25));

        /// Cреднеквадратическая разность времени поражения с погрешностью и точного времени поражения
        if(area.value().numTrack.count() != 0 &&
           tracks->contains(area.value().numTrack.at(0)))
        {
            tResults->setItem(i, 7, new QTableWidgetItem(QString::number(tracks->value(area.value().numTrack.at(0)).rmsDiffTime) + " (" +
                                                         QString::number(area.value().numTrack.at(0)) + ")"));
        }

        /// Время движения до ЗКВ с погрешностью и номер трассы
        for(int j = 0; j < area.value().numTrack.count(); ++j)
        {
            if(tracks->contains(area.value().numTrack.at(j)))
            {
                tResults->setItem(i, j + 8, new QTableWidgetItem(QString::number(tracks->value(area.value().numTrack.at(j)).timeToPA) + " (" +
                                                                 QString::number(area.value().numTrack.at(j)) + ")"));
            }
        }

        /// Подсвечивание СКО и времении движения до ЗКВ наиболее опасной трассы
        if(area.value().numTrack.count() != 0 &&
           area.value().numTrack.at(0) == Track::numTrackMinErrTime)
        {
            tResults->item(i, 0)->setBackgroundColor(QColor(255, 0, 0, 100));
            tResults->item(i, 7)->setBackgroundColor(QColor(255, 0, 0, 100));
            tResults->item(i, 8)->setBackgroundColor(QColor(255, 0, 0, 100));
        }

        /// Очистка неиспользующихся ячеек СКО
        if(area.value().numTrack.count() == 0)
            tResults->takeItem(i, 7);

        /// Очистка неиспользующихся ячеек времени движения до ЗКВ
        for(int j = tracks->count() + 8; j > area.value().numTrack.count() + 7; --j)
            tResults->takeItem(i, j);
    }

    /// Знак суммы
    tResults->setItem(areas->count(), 0, new QTableWidgetItem(tr("Σ")));

    /// --------------------------------------------------
    /// Результирующие параметры по всем ЗКВ
    /// --------------------------------------------------

    /// Суммарное количество крылатых ракет
    tResults->setItem(areas->count(), 1, new QTableWidgetItem(QString::number(Area::CMSumCount) + " + " +
                                                              QString::number(Area::detectTracksCount)));
    tResults->item(areas->count(), 1)->setBackgroundColor(QColor(255, 0, 0, 50));

    /// Максимальное суммарное количество крылатых ракет
    tResults->setItem(areas->count(), 2, new QTableWidgetItem(QString::number(Area::CMMaxSumCount)));
    tResults->item(areas->count(), 2)->setBackgroundColor(QColor(255, 0, 0, 50));

    /// Суммарное количество баллистических целей
    tResults->setItem(areas->count(), 3, new QTableWidgetItem(QString::number(Area::BGSumCount)));
    tResults->item(areas->count(), 3)->setBackgroundColor(QColor(255, 0, 0, 50));

    /// Максимальное суммарное количество баллистических целей
    tResults->setItem(areas->count(), 4, new QTableWidgetItem(QString::number(Area::BGMaxSumCount)));
    tResults->item(areas->count(), 4)->setBackgroundColor(QColor(255, 0, 0, 50));

    /// Количественный состав налета по всем ЗКВ с учетом эквивалента БЦ
    tResults->setItem(areas->count(), 5, new QTableWidgetItem(QString::number(Area::raidSumCount) + " + " +
                                                              QString::number(Area::detectTracksCount)));
    tResults->item(areas->count(), 5)->setBackgroundColor(QColor(255, 0, 0, 50));

    /// Максимальный количественный состав налета по всем ЗКВ с учетом эквивалента БЦ
    tResults->setItem(areas->count(), 6, new QTableWidgetItem(QString::number(Area::raidMaxSumCount)));
    tResults->item(areas->count(), 6)->setBackgroundColor(QColor(255, 0, 0, 50));

    /// Отображение числа количественного состава налета по всем ЗКВ с учетом эквивалента БЦ
    lcdMaxSumTrack->display(Area::raidSumCount + Area::detectTracksCount);
}

}
