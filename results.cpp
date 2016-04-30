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

void Results::loadTable(const QVector<Area> *_area, const QVector<Track> *_track)
{
    tResults->setRowCount(_area->count());
    tResults->setColumnCount(_track->count() + 1);

    for(int i = 0; i < _area->count(); ++i)
    {
        for(int j = 0; j < _track->count(); ++j)
            tResults->setItem(i, j, new QTableWidgetItem(QString::number(_track->at(j).target.at(i).errTime)));

        /// Подсвечивание самой опасной трассы для каждого ПР
        if(_area->at(i).dangerousTrack != NULL)
            tResults->item(i, _area->at(i).dangerousTrack->num)->setBackgroundColor(QColor(255, 0, 0, 100));
//        tResults->setItem(i, _nTrack, new QTableWidgetItem(QString::number(getIndexMinElement(&_area->at(i)) + 1)));
//        tResults->item(i, getIndexMinElement(&_area->at(i)))->setBackgroundColor(QColor(255, 0, 0, 100));
//        tResults->item(i, _nTrack)->setBackgroundColor(QColor(255, 0, 0, 100));
    }

//    QVector <float> temp;
//    for(int j = 0; j < _nTrack; ++j)
//    {
//        /// Определение наиболее вероятной базы каждой трассы
//        for(int i = 0; i < _track; ++i)
//            temp.push_back(_area->at(i).at(j));
//        tResults->setItem(_track, j, new QTableWidgetItem(QString::number(getIndexMinElement(&temp) + 1)));
//        tResults->item(getIndexMinElement(&temp), j)->setTextColor(Qt::darkRed);
//        tResults->item(_track, j)->setTextColor(Qt::darkRed);
//        temp.clear();
//    }
}

}
