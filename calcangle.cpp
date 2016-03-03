#include "calcangle.h"

CalcAngle::CalcAngle(QWidget *_parent)
    : QMainWindow(_parent)
{
#ifdef Q_OS_WIN         /// Widnows
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp-1251"));
#elif defined Q_OS_UNIX /// Linux
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
#endif

    results = new Results;
    painter = new Painter(results);
    settingTotal = new SettingTotal(painter);
    settingBase = new SettingBase(painter);
    settingTrack = new SettingTrack(painter);

    setCentralWidget(painter);

    QToolBar *tSettingTotal = new QToolBar(this);
    tSettingTotal->addWidget(settingTotal);
    tSettingTotal->setMovable(false);
    addToolBar(tSettingTotal);

    QDockWidget *dSettingForBase = new QDockWidget(this);
    dSettingForBase->setWidget(settingBase);
    dSettingForBase->setAllowedAreas(Qt::LeftDockWidgetArea);
    dSettingForBase->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dSettingForBase);

    QDockWidget *dSettingTrack = new QDockWidget(this);
    dSettingTrack->setWidget(settingTrack);
    dSettingTrack->setAllowedAreas(Qt::LeftDockWidgetArea);
    dSettingTrack->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dSettingTrack);

    QDockWidget *dResults = new QDockWidget(this);
    dResults->setWidget(settingTrack);
    dResults->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dResults->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, dResults);
}

CalcAngle::~CalcAngle()
{
    delete settingBase;
    delete settingTrack;
    delete settingTotal;
    delete painter;
    delete results;
}
