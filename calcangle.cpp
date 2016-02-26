#include "calcangle.h"

CalcAngle::CalcAngle(QWidget *_parent)
    : QMainWindow(_parent)
{
#ifdef Q_OS_WIN         /// Widnows
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp-1251"));
#elif defined Q_OS_UNIX /// Linux
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
#endif

    painter = new Painter();
    this->setCentralWidget(painter);

    tool = new Tool(painter);
    QDockWidget *dTool = new QDockWidget;
    dTool->setWidget(tool);
    dTool->setAllowedAreas(Qt::AllDockWidgetAreas);
    dTool->setFeatures(QDockWidget::DockWidgetMovable);
    dTool->setMaximumHeight(100);
    addDockWidget(Qt::TopDockWidgetArea, dTool);
}

CalcAngle::~CalcAngle()
{
    delete painter;
    delete tool;
}
