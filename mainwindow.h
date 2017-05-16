#ifndef CALCANGLE_H
#define CALCANGLE_H

#include "settings.h"

#include <QApplication>
#include <QMainWindow>
#include <QTextCodec>
#include <QToolBar>
#include <QDockWidget>

namespace ThreatLevel
{

/// Класс виджета главного окна
class MainWindow : public QMainWindow
{
public:
    explicit    MainWindow(QWidget *_parent = 0);
    ~MainWindow();

private:
    /// --------------------------------------------------
    /// Указатели на объекты классов
    /// --------------------------------------------------

    ParametersOfAreas       *parametersOfAreas;     /// Класс виджета редактирования параметров ЗКВ
    ParametersOfEtalons     *parametersOfEtalons;   /// Класс виджета редактирования параметров эталонов
    Painter                 *painter;               /// Класс виджета отрисовки эталонов, трасс и ЗКВ
    GraphOfTracksCount      *graphOfTracksCount;    /// Класс виджета графика количественного состава налёта
    Results                 *results;               /// Класс виджета отображения таблицы результатов
    Settings                *settings;              /// Класс виджета настроек
};

}

#endif // CALCANGLE_H
