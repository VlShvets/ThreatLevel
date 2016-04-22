#ifndef SETTINGTRACK_H
#define SETTINGTRACK_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

namespace ThreatLevel
{

class SettingTrack : public QWidget
{
    Q_OBJECT

public:
    explicit SettingTrack(class Painter *_painter, QWidget *parent = 0);
    ~SettingTrack();

private slots:    
    void loadTable(int _number);            /// Загрузка таблицы с начальными данными
    void changeTrackPar(int _i, int _j);    /// Изменение параметров локаторов

private:
    class Painter *painter;

    QTableWidget *tTrackPar;    /// Таблица параметров целей
};

}

#endif // SETTINGTRACK_H
