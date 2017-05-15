#ifndef SETTINGS_H
#define SETTINGS_H

#include "mainthread.h"
#include "parametersofareas.h"
#include "parametersofetalons.h"
#include "painter.h"
#include "results.h"
#include "graphoftrackscount.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QStyle>
#include <QPushButton>
#include <QSlider>
#include <QLCDNumber>
#include <QCheckBox>

namespace ThreatLevel
{

/// Класс виджета настроек
class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit    Settings(ParametersOfAreas *_parametersOfAreas, ParametersOfEtalons *_parametersOfEtalons,
                         Painter *_painter, GraphOfTracksCount *_graphOfTracksCount, Results *_results, QWidget *_parent = 0);
    ~Settings();

private slots:
    /// Перезапуск потока вычислений
    void        reStart();

    /// Запуск и остановка потока вычислений
    void        stateChanged();

    /// Остановка потока вычислений
    void        stop();

private:
    /// Создание нового потока вычислений
    void        createOfThread();

    /// Завершение имеющегося потока вычислений
    void        completeOfThread();

    /// Указатели на виджеты
    QSlider         *sWaitingTime;  /// Виджет регулирования времени ожидания между циклами вычислений
    QLCDNumber      *lWaitingTime;  /// Виджет отображения времени ожидания между циклами вычислений
    QPushButton     *pPlayPause;    /// Виджет кнопки запуска и остановки таймера

    /// --------------------------------------------------
    /// Указатели на объекты классов
    /// --------------------------------------------------

    ParametersOfAreas       *parametersOfAreas;     /// Класс виджета редактирования параметров ЗКВ
    ParametersOfEtalons     *parametersOfEtalons;   /// Класс виджета редактирования параметров эталонов
    Painter                 *painter;               /// Класс виджета отрисовки эталонов, трасс и ЗКВ
    GraphOfTracksCount      *graphOfTracksCount;    /// Класс виджета графика количественного состава налёта
    Results                 *results;               /// Класс виджета отображения таблицы результатов
    MainThread              *mainThread;            /// Класс главного потока вычислений и отрисовки

    /// --------------------------------------------------
    /// Константы
    /// --------------------------------------------------

    /// Время ожидания между циклами вычислений
    static const int    WAITING_TIME_MIN    = 10;       /// Минимальное время ожидания      (10 мс)
    static const int    WAITING_TIME_DEF    = 110;      /// Время ожидания по умолчанию     (110 мс)
    static const int    WAITING_TIME_MAX    = 1010;     /// Максимальное время ожидания     (1010 мс)
};

}

#endif // SETTINGS_H
