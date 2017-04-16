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
    explicit    Settings(class Painter *_painter, QWidget *parent = 0);
    ~Settings();

private slots:
    /// Запуск и остановка таймера
    void        changeState();

    /// Изменение интервала таймера
    void        changeTimerInterval(int _interval);

private:
    /// Указатели на виджеты
    QLCDNumber      *lTimerInterval;    /// Виджет отображения интервала таймера
    QPushButton     *pStartStop;        /// Виджет кнопки запуска и остановки таймера

    /// Указатели на объекты классов
    class Painter   *painter;           /// Класс отрисовки трасс и позиционных районов

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Состояние таймера
    bool    isStart;    /// Флаг состояния таймера

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Параметры таймера
    static const int    TIMER_DEF_INTERVAL  = 1;    /// Интервал таймера по умолчанию
    static const int    TIMER_MAX_INTERVAL  = 10;   /// Максимальный интервал таймера
};

}

#endif // SETTINGS_H
