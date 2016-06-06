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
    const int MAXTIMERINTERVAL = 10;    /// Максимальный интервал таймера
    const int DEFTIMERINTERVAL = 1;     /// Интервал таймера по умолчанию

    explicit Settings(class Painter *_painter, QWidget *parent = 0);
    ~Settings();

private slots:
    void changeState();                         /// Запуск и остановка
    void changeTimerInterval(int _interval);    /// Изменение интервала таймера

private:
    QLCDNumber *lTimerInterval;     /// Интервал таймера
    QPushButton *pStartStop;        /// Кнопка запуска и остановки

    bool isStart;                   /// Флаг состояния кнопки

    class Painter *painter;
};

}

#endif // SETTINGS_H
