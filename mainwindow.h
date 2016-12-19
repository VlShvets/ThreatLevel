#ifndef CALCANGLE_H
#define CALCANGLE_H

#include <QApplication>
#include <QMainWindow>
#include <QTextCodec>
#include <QToolBar>
#include <QDockWidget>

#include "areaparameters.h"
#include "trackparameters.h"
#include "settings.h"

namespace ThreatLevel
{

/// Класс виджета главного окна
class MainWindow : public QMainWindow
{
public:
    explicit        MainWindow(QWidget *_parent = 0);
    ~MainWindow();

private:
    AreaParameters  *areaParameters;    /// Виджет редактирования параметров позиционных районов
    TrackParameters *trackParameters;   /// Виджет редактирования параметров трасс
    TrackGraph      *trackGraph;        /// Виджет графика количественного состава
    Results         *results;           /// Виджет отображения результатов
    Painter         *painter;           /// Виджет отрисовки трасс и позиционных районов
    Settings        *settings;          /// Виджет настроек
};

}

#endif // CALCANGLE_H
