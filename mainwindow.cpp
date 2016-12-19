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
    trackGraph = new TrackGraph;
    results = new Results(trackGraph);
    painter = new Painter(areaParameters, trackParameters, results);
    settings = new Settings(painter);

    /// Виджет отрисовки трасс и позиционных районов
    setCentralWidget(painter);

    /// Виджет настроек
    QToolBar *tSettings = new QToolBar(this);
    tSettings->addWidget(settings);
    tSettings->setWindowTitle(tr("Панель управления"));
    tSettings->setMovable(false);
    addToolBar(tSettings);

    /// Виджет редактирования параметров позиционных районов
    QDockWidget *dAreaParameters = new QDockWidget(this);
    dAreaParameters->setWidget(areaParameters);
    dAreaParameters->setWindowTitle(tr("Параметры ПР"));
    dAreaParameters->setAllowedAreas(Qt::AllDockWidgetAreas);
    dAreaParameters->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dAreaParameters->setMinimumHeight(300);
    addDockWidget(Qt::LeftDockWidgetArea, dAreaParameters);

    /// Виджет редактирования параметров трасс
    QDockWidget *dTrackParameters = new QDockWidget(this);
    dTrackParameters->setWidget(trackParameters);
    dTrackParameters->setWindowTitle(tr("Параметры трасс"));
    dTrackParameters->setAllowedAreas(Qt::AllDockWidgetAreas);
    dTrackParameters->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dTrackParameters->setMinimumHeight(300);
    addDockWidget(Qt::LeftDockWidgetArea, dTrackParameters);

    /// Виджет графика количественного состава
    QDockWidget *dTrackGraph = new QDockWidget(this);
    dTrackGraph->setWidget(trackGraph);
    dTrackGraph->setWindowTitle(tr("График количественного состава налета"));
    dTrackGraph->setAllowedAreas(Qt::AllDockWidgetAreas);
    dTrackGraph->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dTrackGraph->setMinimumHeight(300);
    addDockWidget(Qt::RightDockWidgetArea, dTrackGraph);

    /// Виджет отображения результатов
    QDockWidget *dResults = new QDockWidget(this);
    dResults->setWidget(results);
    dResults->setWindowTitle(tr("Таблица результатов"));
    dResults->setAllowedAreas(Qt::AllDockWidgetAreas);
    dResults->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dResults->setMinimumHeight(300);
    addDockWidget(Qt::RightDockWidgetArea, dResults);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete painter;
    delete results;
    delete areaParameters;
    delete trackParameters;
    delete trackGraph;
}

}
