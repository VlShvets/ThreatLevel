#include "calcangle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CalcAngle calcAngle;
    calcAngle.showMaximized();

    return a.exec();
}
