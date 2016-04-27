#ifndef SETTINGTRACK_H
#define SETTINGTRACK_H

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

class SettingTrack : public QWidget     /// Класс трасс
{
    Q_OBJECT

public:
    explicit SettingTrack(QWidget *parent = 0);
    ~SettingTrack();

    inline int getCount();                          /// Получить количество
    inline float getPar(int _row, int _column);     /// Получить параметр

private slots:
    void initPar(int _number);                      /// Начальная инициализация параметров

private:
    QLCDNumber *lNumGroups;                         /// Количество групп
    QLCDNumber *lTracksInGroup;                     /// Количество трасс в группе
    QTableWidget *tTrackPar;                        /// Таблица параметров
};

int SettingTrack::getCount()
{
    return tTrackPar->rowCount();
}

float SettingTrack::getPar(int _row, int _column)
{
    return tTrackPar->item(_row, _column)->text().toFloat();
}

}

#endif // SETTINGTRACK_H
