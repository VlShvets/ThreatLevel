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
    explicit    MainWindow(QWidget *_parent = 0);
    ~MainWindow();

private:
    /// Указатели на объекты классов
    AreaParameters      *areaParameters;    /// Класс редактирования параметров позиционных районов
    TrackParameters     *trackParameters;   /// Класс редактирования параметров трасс
    TrackGraph          *trackGraph;        /// Класс графика количественного состава
    Results             *results;           /// Класс отображения результатов
    Painter             *painter;           /// Класс отрисовки трасс и позиционных районов
    Settings            *settings;          /// Класс настроек
};

}

#endif // CALCANGLE_H
