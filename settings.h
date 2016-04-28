#ifndef SETTINGS_H
#define SETTINGS_H

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

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(class Painter *_painter, QWidget *parent = 0);
    ~Settings();

private slots:
    void changeState();     /// Запуск и остановка отрисовщика

private:    
    class Painter *painter;

    bool isStart;                   /// Флаг состояния кнопки
    QPushButton *pStartStop;        /// Кнопка запуска и остановки процессов
    QPushButton *pStartFromStart;   /// Кнопка запуска с начала
};

}

#endif // SETTINGS_H
