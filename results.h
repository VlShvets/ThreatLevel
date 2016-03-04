#ifndef RESULTS_H
#define RESULTS_H

#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>

namespace ThreatLevel
{

class Results : public QWidget
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = 0);
    ~Results();

    void loadTable(const QVector<QVector<float> > *_times, int _nBase, int _nTrack);

private:
    int getIndexMinElement(const QVector <float> *_vector);


    QTableWidget *tResults;     /// Таблица результатов
};

}

#endif // RESULTS_H
