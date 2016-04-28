#ifndef TRACKPARAMETERS_H
#define TRACKPARAMETERS_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

namespace ThreatLevel
{

const int MAXNUMGROUPS = 5;
const int DEFNUMGROUPS = 3;
const int MAXTRACKSINGROUP = 6;
const int DEFTRACKSINGROUP = 4;

const int DISTBETWEENTRACKS = 6000;

class TrackParameters : public QWidget     /// Класс трасс
{
    Q_OBJECT

public:
    explicit TrackParameters(QWidget *parent = 0);
    ~TrackParameters();

    inline int getCount();                              /// Получить количество
    inline float getPar(int _row, int _column);         /// Получить параметр

private slots:
    void numGroupsChanged(int _num);
    void numTracksInGroupChanged(int _num);

private:    
    void initPar(int _nGroups, int _nTracksInGroup);    /// Начальная инициализация параметров

    QLCDNumber *lNumGroups;                             /// Количество групп
    QLCDNumber *lTracksInGroup;                         /// Количество трасс в группе
    QTableWidget *tTrackPar;                            /// Таблица параметров
};

int TrackParameters::getCount()
{
    return tTrackPar->rowCount();
}

float TrackParameters::getPar(int _row, int _column)
{
    return tTrackPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

}

#endif // SETTINGTRACK_H
