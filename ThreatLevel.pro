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
    settingtotal.cpp \
    painter.cpp \
    settingtrack.cpp \
    results.cpp \
    mainwindow.cpp \
    settingarea.cpp

HEADERS  += \
    settingtotal.h \
    painter.h \
    settingtrack.h \
    results.h \
    mainwindow.h \
    settingarea.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
