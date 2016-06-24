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

void Results::loadTable(const QVector<Area> &_area)
{
    tResults->setRowCount(_area.count());
    tResults->setColumnCount(_area.at(0).target.count() + 1);

    for(int i = 0; i < _area.count(); ++i)
    {
        for(int j = 0; j < _area.at(i).target.count(); ++j)
            tResults->setItem(i, j, new QTableWidgetItem(QString::number(_area.at(i).target.at(j).errTime) + " (" +
                                                         QString::number(_area.at(i).target.at(j).num + 1) + ")"));

        /// Погрешность времени поражения ПР самой опасной трассой
        tResults->setItem(i, _area.at(i).target.count(), new QTableWidgetItem(QString::number(_area.at(i).sigmaT) + " (" +
                                                                   QString::number(_area.at(i).target.at(0).num + 1) + ")"));
        tResults->item(i, _area.at(i).target.count())->setBackgroundColor(QColor(255, 0, 0, 50));
    }
}

}
