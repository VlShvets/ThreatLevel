#include "mainwindow.h"

namespace ThreatLevel
{

/// Класс виджета главного окна
MainWindow::MainWindow(QWidget *_parent)
    : QMainWindow(_parent)
{
#ifdef Q_OS_WIN         /// Widnows
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp-1251"));
#elif defined Q_OS_UNIX /// Linux
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
#endif

    this->setWindowTitle(tr("Определение уровня угроз"));

    areaParameters = new AreaParameters;
    trackParameters = new TrackParameters;
    graphSumTrack = new GraphSumTrack;
    results = new Results;
    painter = new Painter(areaParameters, trackParameters, graphSumTrack, results);
    settings = new Settings(painter);

    /// Виджет отрисовки трасс и позиционных районов
    setCentralWidget(painter);

    /// Виджет настроек
    QToolBar *tSettings = new QToolBar(this);
    tSettings->addWidget(settings);
    tSettings->setMovable(false);
    addToolBar(tSettings);

    /// Виджет редактирования параметров позиционных районов
    QDockWidget *dAreaParameters = new QDockWidget(this);
    dAreaParameters->setWidget(areaParameters);
    dAreaParameters->setAllowedAreas(Qt::LeftDockWidgetArea);
    dAreaParameters->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dAreaParameters);

    /// Виджет редактирования параметров трасс
    QDockWidget *dTrackParameters = new QDockWidget(this);
    dTrackParameters->setWidget(trackParameters);
    dTrackParameters->setAllowedAreas(Qt::LeftDockWidgetArea);
    dTrackParameters->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dTrackParameters);

    /// Виджет графика количественного состава
    QDockWidget *dGraphSumTrack = new QDockWidget(this);
    dGraphSumTrack->setWidget(graphSumTrack);
    dGraphSumTrack->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dGraphSumTrack->setFeatures(QDockWidget::DockWidgetMovable);
    dGraphSumTrack->setMinimumWidth(650);
    dGraphSumTrack->setMinimumHeight(300);
    addDockWidget(Qt::RightDockWidgetArea, dGraphSumTrack);

    /// Виджет отображения результатов
    QDockWidget *dResults = new QDockWidget(this);
    dResults->setWidget(results);
    dResults->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dResults->setFeatures(QDockWidget::DockWidgetMovable);
    dResults->setMinimumWidth(650);
    dResults->setMinimumHeight(300);
    addDockWidget(Qt::RightDockWidgetArea, dResults);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete painter;
    delete areaParameters;
    delete trackParameters;
    delete results;
    delete graphSumTrack;
}

}
