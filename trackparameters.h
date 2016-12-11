#ifndef TRACKPARAMETERS_H
#define TRACKPARAMETERS_H

#include <QGridLayout>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

namespace ThreatLevel
{

/// Класс виджета редактирования параметров трасс
class TrackParameters : public QWidget
{
    Q_OBJECT

public:
    explicit TrackParameters(QWidget *parent = 0);
    ~TrackParameters();

    /// Получить количество трасс
    inline int getCount() const;

    /// Получить параметр трассы по номеру строки и номеру столбца
    inline float getPar(int _row, int _column) const;

private slots:
    /// Изменение количества групп
    void numGroupsChanged(int _num);

    /// Изменение количества трасс в группе
    void numTracksInGroupChanged(int _num);

private:
    /// Начальная инициализация параметров трасс
    void initPar(int _nGroups, int _nTracksInGroup);

    QLCDNumber *lNumGroups;     /// Виджет отображения количества групп
    QLCDNumber *lTracksInGroup; /// Виджет отображения количества трасс в группе
    QTableWidget *tTrackPar;    /// Виджет таблицы параметров трасс

    static const int MAX_NUM_GROUPS = 10;           /// Максимальное количество групп
    static const int DEF_NUM_GROUPS = 5;            /// Количество групп по умолчанию

    static const int MAX_TRACKS_IN_GROUP = 12;      /// Максимальное количество трасс в группе
    static const int DEF_TRACKS_IN_GROUP = 1;       /// Количество трасс в группе по умолчанию

    static const int DIST_BETWEEN_TRACKS = 6000;    /// Расстояние между трассами в группе
};

/// Получить количество трасс
int TrackParameters::getCount() const
{
    return tTrackPar->rowCount();
}

/// Получить параметр по номеру строки и номеру столбца
float TrackParameters::getPar(int _row, int _column) const
{
    return tTrackPar->item(_row, _column)->data(Qt::DisplayRole).toFloat();
}

}

#endif // SETTINGTRACK_H
