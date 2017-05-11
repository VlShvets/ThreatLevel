#include "mainwindow.h"

namespace ThreatLevel
{

/// Класс виджета главного окна
MainWindow::MainWindow(QWidget *_parent)
    : QMainWindow(_parent)
{

#ifdef Q_OS_WIN         /// Widnows (Qt 5)
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp-1251"));
#elif defined Q_OS_UNIX /// Linux   (Qt 4)
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
#endif

    this->setWindowTitle(tr("Определение уровня угроз"));

    parametersOfAreas   = new ParametersOfAreas;
    parametersOfEtalons = new ParametersOfEtalons;
    painter             = new Painter;
    graphOfTracksCount  = new GraphOfTracksCount;
    results             = new Results(graphOfTracksCount);
    settings            = new Settings(parametersOfAreas, parametersOfEtalons, painter, results);

    /// Виджет отрисовки трасс и позиционных районов
    setCentralWidget(painter);

    /// Виджет настроек
    QToolBar *tSettings = new QToolBar(this);
    tSettings->addWidget(settings);
    tSettings->setWindowTitle(tr("Панель управления"));
    tSettings->setMovable(false);
    addToolBar(tSettings);

    /// Виджет редактирования параметров ЗКВ
    QDockWidget *dAreaParameters = new QDockWidget(this);
    dAreaParameters->setWidget(parametersOfAreas);
    dAreaParameters->setWindowTitle(tr("Параметры ЗКВ"));
    dAreaParameters->setAllowedAreas(Qt::AllDockWidgetAreas);
    dAreaParameters->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dAreaParameters->setMinimumHeight(300);
    addDockWidget(Qt::LeftDockWidgetArea, dAreaParameters);

    /// Виджет редактирования параметров эталонов
    QDockWidget *dTrackParameters = new QDockWidget(this);
    dTrackParameters->setWidget(parametersOfEtalons);
    dTrackParameters->setWindowTitle(tr("Параметры эталонов"));
    dTrackParameters->setAllowedAreas(Qt::AllDockWidgetAreas);
    dTrackParameters->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dTrackParameters->setMinimumHeight(300);
    addDockWidget(Qt::LeftDockWidgetArea, dTrackParameters);

    /// Виджет графика количественного состава
    QDockWidget *dTrackGraph = new QDockWidget(this);
    dTrackGraph->setWidget(graphOfTracksCount);
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
    delete results;
    delete graphOfTracksCount;
    delete painter;
    delete parametersOfEtalons;
    delete parametersOfAreas;
}

}
