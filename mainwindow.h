#ifndef CALCANGLE_H
#define CALCANGLE_H

#include "parametersofareas.h"
#include "parametersofetalons.h"
#include "settings.h"
#include "graphoftrackscount.h"

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
    /// Указатели на объекты классов
    ParametersOfAreas       *parametersOfAreas;     /// Класс редактирования параметров ЗКВ
    ParametersOfEtalons     *parametersOfEtalons;   /// Класс редактирования параметров эталонов
    GraphOfTracksCount      *graphOfTracksCount;    /// Класс графика количественного состава
    Results                 *results;               /// Класс отображения результатов
    Painter                 *painter;               /// Класс отрисовки трасс и позиционных районов
    Settings                *settings;              /// Класс настроек
};

}

#endif // CALCANGLE_H
