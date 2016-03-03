#ifndef RESULTS_H
#define RESULTS_H

#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>

class Results : public QWidget
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = 0);
    ~Results();

    void loadTable();

signals:

public slots:
    QTableWidget *tResults;  /// Таблица результатов
};

#endif // RESULTS_H
