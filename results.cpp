#include "results.h"

Results::Results(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(QObject::tr("Определение уровня угроз"));

    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel(QObject::tr("\tПараметры баз:")), 0, 0, 1, 3);

    tResults = new QTableWidget(this);
    gridLayout->addWidget(tResults, 1, 0, 1, 3);

    this->setLayout(gridLayout);
}

Results::~Results()
{
    delete tResults;
}

void Results::loadTable()
{

}

