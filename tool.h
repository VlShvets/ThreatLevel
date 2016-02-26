#ifndef TOOL_H
#define TOOL_H

#include <QWidget>

#include "painter.h"

class Tool : public QWidget
{
    Q_OBJECT
public:
    explicit Tool(Painter *_painter);
    ~Tool();

signals:

public slots:

private:
    Painter *painter;
};

#endif // TOOL_H
