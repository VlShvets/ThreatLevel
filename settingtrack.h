#ifndef SETTINGTRACK_H
#define SETTINGTRACK_H

#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QSlider>
#include <QTableWidget>

#include "painter.h"

class SettingTrack : public QWidget
{
    Q_OBJECT

public:
    explicit SettingTrack(class Painter *_painter, QWidget *parent = 0);    /// Конструктор класса
    ~SettingTrack();                                                        /// Деструктор класса

private slots:
    void changeNumberTrack(int _count);     /// Изменение количества целей
    void changeParTrack(int _i, int _j);    /// Изменение параметров локаторов

private:
    void loadTable();           /// Загрузка таблицы с начальными данными

    class Painter *painter;

    QLCDNumber *lNumberTrack;   /// Дисплей количества целей
    QTableWidget *tParTrack;    /// Таблица параметров целей
};

#endif // SETTINGTRACK_H
