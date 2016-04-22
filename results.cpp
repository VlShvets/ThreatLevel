#include "results.h"

namespace ThreatLevel
{

Results::Results(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    vLayout->addWidget(new QLabel(QObject::tr("Позиционные районы\t\\\tРакеты")));

    tResults = new QTableWidget(3, 5, this);
    tResults->setMinimumWidth(400);
    vLayout->addWidget(tResults);

    this->setLayout(vLayout);
}

Results::~Results()
{
    delete tResults;
}

void Results::loadTable(const QVector <QVector <float> > *_times, int _nBase, int _nTrack)
{
    tResults->setRowCount(_nBase + 1);
    tResults->setColumnCount(_nTrack + 1);

    for(int i = 0; i < _nBase; ++i)
    {
        for(int j = 0; j < _nTrack; ++j)
            tResults->setItem(i, j, new QTableWidgetItem(QString::number(_times->at(i).at(j))));

        /// Определение самой опасной трассы для каждой базы
        tResults->setItem(i, _nTrack, new QTableWidgetItem(QString::number(getIndexMinElement(&_times->at(i)) + 1)));
        tResults->item(i, getIndexMinElement(&_times->at(i)))->setBackgroundColor(QColor(255, 0, 0, 100));
        tResults->item(i, _nTrack)->setBackgroundColor(QColor(255, 0, 0, 100));
    }

    QVector <float> temp;
    for(int j = 0; j < _nTrack; ++j)
    {
        /// Определение наиболее вероятной базы каждой трассы
        for(int i = 0; i < _nBase; ++i)
            temp.push_back(_times->at(i).at(j));
        tResults->setItem(_nBase, j, new QTableWidgetItem(QString::number(getIndexMinElement(&temp) + 1)));
        tResults->item(getIndexMinElement(&temp), j)->setTextColor(Qt::darkRed);
        tResults->item(_nBase, j)->setTextColor(Qt::darkRed);
        temp.clear();
    }

    tResults->takeItem(_nBase, _nTrack);
}

int Results::getIndexMinElement(const QVector<float> *_vector)
{
    int indexMinElement = 0;
    for(int i = 1; i < _vector->count(); ++i)
    {
        if(_vector->at(i) < _vector->at(indexMinElement))
            indexMinElement = i;
    }
    return indexMinElement;
}

}
