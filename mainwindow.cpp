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
    settingArea = new SettingArea;
    settingTrack = new SettingTrack;
    painter = new Painter(settingArea, settingTrack, results);
    settingTotal = new SettingTotal(painter);

    setCentralWidget(painter);

    QToolBar *tSettingTotal = new QToolBar(this);
    tSettingTotal->addWidget(settingTotal);
    tSettingTotal->setMovable(false);
    addToolBar(tSettingTotal);

    QDockWidget *dSettingForArea = new QDockWidget(this);
    dSettingForArea->setWidget(settingArea);
    dSettingForArea->setAllowedAreas(Qt::LeftDockWidgetArea);
    dSettingForArea->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dSettingForArea);

    QDockWidget *dSettingTrack = new QDockWidget(this);
    dSettingTrack->setWidget(settingTrack);
    dSettingTrack->setAllowedAreas(Qt::LeftDockWidgetArea);
    dSettingTrack->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dSettingTrack);

    QDockWidget *dResults = new QDockWidget(this);
    dResults->setWidget(results);
    dResults->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dResults->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, dResults);
}

MainWindow::~MainWindow()
{
    delete settingTotal;
    delete painter;
    delete settingArea;
    delete settingTrack;
    delete results;
}

}
