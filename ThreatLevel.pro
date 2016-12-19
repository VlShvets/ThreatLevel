#-------------------------------------------------
#
# Project created by QtCreator 2016-02-01T14:13:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32: LIBS += -L$$PWD/lib/Grapher2D/win -lGrapher2D
unix: LIBS += -L$$PWD/lib/Grapher2D/unix -lGrapher2D
INCLUDEPATH += $$PWD/lib/Grapher2D

TARGET = ThreatLevel
TEMPLATE = app

SOURCES += main.cpp\
    painter.cpp \
    results.cpp \
    mainwindow.cpp \
    areaparameters.cpp \
    settings.cpp \
    trackparameters.cpp \
    trackgraph.cpp

HEADERS  += \
    painter.h \
    results.h \
    mainwindow.h \
    areaparameters.h \
    trackparameters.h \
    settings.h \
    area.h \
    track.h \
    trackgraph.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
