#ifndef SETTINGBASE_H
#define SETTINGBASE_H

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QLCDNumber>
#include <QTableWidget>

#include "painter.h"

class SettingBase : public QWidget
{
    Q_OBJECT

public:
    explicit SettingBase(Painter *_painter, QWidget *parent = 0);
    ~SettingBase();

private slots:
    void changeNumberBase(int _count);  /// Изменение количества баз
    void changeParBase(int _i, int _j); /// Изменение параметров баз

private:
    void loadTable();           /// Загрузка таблицы с начальными данными

    class Painter *painter;

    QLCDNumber *lNumberBase;    /// Дисплей количества баз
    QTableWidget *tParBase;     /// Таблица параметров баз

};

#endif // SETTINGBASE_H
