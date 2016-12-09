#include "results.h"

namespace ThreatLevel
{

Results::Results(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    vLayout->addWidget(new QLabel(QObject::tr("Позиционные районы\t\\\tТрассы")));

    tResults = new QTableWidget(this);
    tResults->setMinimumWidth(600);
    vLayout->addWidget(tResults);

    this->setLayout(vLayout);
}

Results::~Results()
{
    delete tResults;
}

void Results::loadTable(const QVector<Area> &_area, const QVector<Track> &_track)
{
    tResults->setRowCount(_area.count() + 1);
    tResults->setColumnCount(_track.count() + 1);

    int sumTrackCount = 0;          /// Суммарный количественный состав
    float minErrTime = 999999.0;    /// Минимальное время поражения ПР

    for(int i = 0; i < _area.count(); ++i)
    {
        /// Количественный состав и среднеквадратическая разность времени поражения с погрешностью и точного времени поражения
        tResults->setItem(i, 0, new QTableWidgetItem(QString::number(_area.at(i).numTrack.count()) + " (" +
                                                     QString::number(_area.at(i).rmsDiffTime) + ")"));
        tResults->item(i, 0)->setBackgroundColor(QColor(255, 0, 0, 50));

        /// Вычисление суммарного количественного состава
        sumTrackCount += _area.at(i).numTrack.count();

        /// Время поражения ПР с погрешностью и номер трассы
        for(int j = 0; j < _area.at(i).numTrack.count(); ++j)
        {
            tResults->setItem(i, j + 1, new QTableWidgetItem(QString::number(_track.at(_area.at(i).numTrack.at(j)).errTime) + " (" +
                                                             QString::number(_area.at(i).numTrack.at(j) + 1) + ")"));
        }

        /// Вычисление минимального времени поражения ПР
        if(!_area.at(i).numTrack.isEmpty() && minErrTime > _track.at(_area.at(i).numTrack.at(0)).errTime)
            minErrTime = _track.at(_area.at(i).numTrack.at(0)).errTime;
    }

    /// Суммарный количественный состав и минимальное время поражения ПР
    tResults->setItem(_area.count(), 0, new QTableWidgetItem(QString::number(sumTrackCount) + " (" +
                                                                 QString::number(minErrTime) + ")"));
    tResults->item(_area.count(), 0)->setBackgroundColor(QColor(255, 0, 0, 100));
}

}
