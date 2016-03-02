#include "calcangle.h"

CalcAngle::CalcAngle(QWidget *_parent)
    : QMainWindow(_parent)
{
#ifdef Q_OS_WIN         /// Widnows
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp-1251"));
#elif defined Q_OS_UNIX /// Linux
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
#endif

    painter = new Painter;
    settingBase = new SettingBase(painter);
    settingTrack = new SettingTrack(painter);
    settingTotal = new SettingTotal(painter);

    setCentralWidget(painter);

    QDockWidget *dSettingForBase = new QDockWidget(this);
    dSettingForBase->setWidget(settingBase);
    dSettingForBase->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dSettingForBase->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dSettingForBase);

    QDockWidget *dSettingTrack = new QDockWidget(this);
    dSettingTrack->setWidget(settingTrack);
    dSettingTrack->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    dSettingTrack->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dSettingTrack);

    QToolBar *tSettingTotal = new QToolBar(this);
    tSettingTotal->addWidget(settingTotal);
    tSettingTotal->setMovable(false);
    addToolBar(tSettingTotal);
}

CalcAngle::~CalcAngle()
{
    delete settingBase;
    delete settingTrack;
    delete settingTotal;
    delete painter;
}
