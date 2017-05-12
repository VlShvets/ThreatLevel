#ifndef SETTINGS_H
#define SETTINGS_H

#include "mainthread.h"
#include "parametersofareas.h"
#include "parametersofetalons.h"
#include "painter.h"
#include "results.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QSlider>
#include <QLCDNumber>
#include <QPushButton>
#include <QStyle>
#include <QSpinBox>

namespace ThreatLevel
{

/// Класс виджета настроек
class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit    Settings(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons,
                         Painter *_painter, Results *_results, QWidget *_parent = 0);
    ~Settings();

private slots:
    /// Создание нового потока вычислений (удаление уже имеющегося потока)
    void        reStart();

    /// Запуск и остановка потока вычислений
    void        stateChanged();

    /// Изменение интервала таймера потока вычислений
    void        intervalOfTimerChanged(int _interval);

private:
    /// Указатели на виджеты
    QLCDNumber      *lTimerInterval;    /// Виджет отображения интервала таймера
    QPushButton     *pStartStop;        /// Виджет кнопки запуска и остановки таймера

    /// --------------------------------------------------
    /// Указатели на объекты классов
    /// --------------------------------------------------

    ParametersOfAreas       *parametersOfAreas;     /// Класс виджета редактирования параметров ЗКВ
    ParametersOfEtalons     *parametersOfEtalons;   /// Класс виджета редактирования параметров эталонов
    Painter                 *painter;               /// Класс виджета отрисовки эталонов, трасс и ЗКВ
    Results                 *results;               /// Класс виджета отображения таблицы результатов
    class MainThread              *mainThread;            /// Класс главного потока вычислений и отрисовки

    /// --------------------------------------------------
    /// Переменные
    /// --------------------------------------------------

    /// Состояние таймера
    bool    isStart;    /// Флаг состояния таймера

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Параметры таймера
    static const int    TIMER_MIN_INTERVAL  = 1;    /// Минимальный интервал таймера
    static const int    TIMER_DEF_INTERVAL  = 1;    /// Интервал таймера по умолчанию
    static const int    TIMER_MAX_INTERVAL  = 10;   /// Максимальный интервал таймера
};

}

#endif // SETTINGS_H
