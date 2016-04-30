#include "results.h"

namespace ThreatLevel
{

Results::Results(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    vLayout->addWidget(new QLabel(QObject::tr("Позиционные районы\t\\\tРакеты")));

    tResults = new QTableWidget(this);
    tResults->setMinimumWidth(400);
    vLayout->addWidget(tResults);

    this->setLayout(vLayout);
}

Results::~Results()
{
    delete tResults;
}

void Results::loadTable(const QVector <Area> *_area, const QVector <Track> *_track, const QVector <Track> *_etalon)
{
    tResults->setRowCount(_area->count());
    tResults->setColumnCount(_track->count() + 1);

    for(int i = 0; i < _area->count(); ++i)
    {
        for(int j = 0; j < _track->count(); ++j)
            tResults->setItem(i, j, new QTableWidgetItem(QString::number(_track->at(j).target.at(i).time)));

        /// Подсвечивание самой опасной трассы для каждого ПР
        tResults->item(i, _area->at(i).nDangerousTrack)->setBackgroundColor(QColor(255, 0, 0, 100));

        /// Ошибка по времени для самой опасной трассы
        tResults->setItem(i, _track->count(), new QTableWidgetItem(QString::number(_track->at(_area->at(i).nDangerousTrack).target.at(i).time -
                                                                                   _etalon->at(_area->at(i).nDangerousTrack).target.at(i).time)));
        tResults->item(i, _track->count())->setBackgroundColor(QColor(255, 255, 0, 100));
    }
}

}
