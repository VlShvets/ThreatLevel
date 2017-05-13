#include "mainwindow.h"

namespace ThreatLevel
{

/// Класс виджета главного окна
MainWindow::MainWindow(QWidget *_parent)
    : QMainWindow(_parent)
{

#ifdef Q_OS_WIN             /// Widnows     (Qt 5)
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp-1251"));
#elif defined Q_OS_UNIX     /// Linux       (Qt 4)
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
#endif

    this->setWindowTitle(tr("Определение уровня угроз"));

    parametersOfEtalons = new ParametersOfEtalons;
    parametersOfAreas   = new ParametersOfAreas;
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
    QDockWidget *dParametersOsAreas = new QDockWidget(this);
    dParametersOsAreas->setWidget(parametersOfAreas);
    dParametersOsAreas->setWindowTitle(tr("Параметры ЗКВ"));
    dParametersOsAreas->setAllowedAreas(Qt::AllDockWidgetAreas);
    dParametersOsAreas->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dParametersOsAreas->setMinimumHeight(300);
    dParametersOsAreas->setVisible(false);
    addDockWidget(Qt::LeftDockWidgetArea, dParametersOsAreas);

    /// Виджет редактирования параметров эталонов
    QDockWidget *dParametersOfTracks = new QDockWidget(this);
    dParametersOfTracks->setWidget(parametersOfEtalons);
    dParametersOfTracks->setWindowTitle(tr("Параметры эталонов"));
    dParametersOfTracks->setAllowedAreas(Qt::AllDockWidgetAreas);
    dParametersOfTracks->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dParametersOfTracks->setMinimumHeight(300);
    dParametersOfTracks->setVisible(false);
    addDockWidget(Qt::LeftDockWidgetArea, dParametersOfTracks);

    /// Виджет графика количественного состава
    QDockWidget *dGraphOfTracksCount = new QDockWidget(this);
    dGraphOfTracksCount->setWidget(graphOfTracksCount);
    dGraphOfTracksCount->setWindowTitle(tr("График количественного состава налета"));
    dGraphOfTracksCount->setAllowedAreas(Qt::AllDockWidgetAreas);
    dGraphOfTracksCount->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dGraphOfTracksCount->setMinimumHeight(300);
    addDockWidget(Qt::RightDockWidgetArea, dGraphOfTracksCount);

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
