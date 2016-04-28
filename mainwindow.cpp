#include "mainwindow.h"

namespace ThreatLevel
{

MainWindow::MainWindow(QWidget *_parent)
    : QMainWindow(_parent)
{
#ifdef Q_OS_WIN         /// Widnows
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp-1251"));
#elif defined Q_OS_UNIX /// Linux
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
#endif

    this->setWindowTitle(QObject::tr("Определение уровня угроз"));

    results = new Results;
    areaParameters = new AreaParameters;
    trackParameters = new TrackParameters;
    painter = new Painter(areaParameters, trackParameters, results);
    settings = new Settings(painter);

    setCentralWidget(painter);

    QToolBar *tSettings = new QToolBar(this);
    tSettings->addWidget(settings);
    tSettings->setMovable(false);
    addToolBar(tSettings);

    QDockWidget *dAreaParameters = new QDockWidget(this);
    dAreaParameters->setWidget(areaParameters);
    dAreaParameters->setAllowedAreas(Qt::LeftDockWidgetArea);
    dAreaParameters->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dAreaParameters);

    QDockWidget *dTrackParameters = new QDockWidget(this);
    dTrackParameters->setWidget(trackParameters);
    dTrackParameters->setAllowedAreas(Qt::LeftDockWidgetArea);
    dTrackParameters->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dTrackParameters);

    QDockWidget *dResults = new QDockWidget(this);
    dResults->setWidget(results);
    dResults->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dResults->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, dResults);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete painter;
    delete areaParameters;
    delete trackParameters;
    delete results;
}

}
