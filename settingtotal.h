#ifndef SETTINGTOTAL_H
#define SETTINGTOTAL_H

#include <QHBoxLayout>
#include <QSplitter>
#include <QSlider>
#include <QLCDNumber>
#include <QPushButton>
#include <QStyle>
#include <QSpinBox>

#include "painter.h"

namespace ThreatLevel
{

class SettingTotal : public QWidget
{
    Q_OBJECT

public:
    explicit SettingTotal(class Painter *_painter, QWidget *parent = 0);
    ~SettingTotal();

private slots:
    void changeState();     /// Запуск и остановка отрисовщика

private:    
    class Painter *painter;

    bool isStart;                   /// Флаг состояния кнопки
    QPushButton *pStartStop;        /// Кнопка запуска и остановки процессов
    QPushButton *pStartFromStart;   /// Кнопка запуска с начала
};

}

#endif // SETTINGTOTAL_H
