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

/// Класс виджета настроек
class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(class Painter *_painter, QWidget *parent = 0);
    ~Settings();

private slots:
    /// Запуск и остановка таймера
    void changeState();

    /// Изменение интервала таймера
    void changeTimerInterval(int _interval);

private:
    QLCDNumber *lTimerInterval;     /// Интервал таймера
    QPushButton *pStartStop;        /// Кнопка запуска и остановки

    class Painter *painter;

    bool isStart;                   /// Флаг состояния кнопки

    const int MAX_TIMER_INTERVAL = 10;    /// Максимальный интервал таймера
    const int DEF_TIMER_INTERVAL = 1;     /// Интервал таймера по умолчанию
};

}

#endif // SETTINGS_H
