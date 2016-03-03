#-------------------------------------------------
#
# Project created by QtCreator 2016-02-01T14:13:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix|win32: LIBS += -L$$PWD/../../PTPV/Library/ -lGrapher2D
INCLUDEPATH += $$PWD/../../PTPV/Grapher2D

TARGET = CalcAngle
TEMPLATE = app

SOURCES += main.cpp\
    settingtotal.cpp \
    painter.cpp \
    calcangle.cpp \
    settingbase.cpp \
    settingtrack.cpp \
    results.cpp

HEADERS  += \
    settingtotal.h \
    painter.h \
    calcangle.h \
    settingbase.h \
    settingtrack.h \
    results.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
