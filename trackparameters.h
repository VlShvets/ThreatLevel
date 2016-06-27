#ifndef TRACKPARAMETERS_H
#define TRACKPARAMETERS_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

namespace ThreatLevel
{

class TrackParameters : public QWidget     /// Класс трасс
{
    Q_OBJECT

public:        
    const int MAXNUMGROUPS = 5;             /// Максимальное количество групп
    const int DEFNUMGROUPS = 5;             /// Количество групп по умолчанию
    const int MAXTRACKSINGROUP = 12;        /// Максимальное количество трасс в группе
    const int DEFTRACKSINGROUP = 1;         /// Количество трасс в группе по умолчанию

    const int DISTBETWEENTRACKS = 6000;     /// Расстояние между трассами в группе

    explicit TrackParameters(QWidget *parent = 0);
    ~TrackParameters();

    inline int getCount() const;                        /// Получить количество
    inline float getPar(int _row, int _column) const;   /// Получить параметр

private slots:
    void numGroupsChanged(int _num);
    void numTracksInGroupChanged(int _num);

private:    
    void initPar(int _nGroups, int _nTracksInGroup);    /// Начальная инициализация параметров

    QLCDNumber *lNumGroups;                             /// Количество групп
    QLCDNumber *lTracksInGroup;                         /// Количество трасс в группе
    QTableWidget *tTrackPar;                            /// Таблица параметров
};

int TrackParameters::getCount() const
{
    return tTrackPar->rowCount();
}

float TrackParameters::getPar(int _row, int _column) const
{
    return tTrackPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

}

#endif // SETTINGTRACK_H
