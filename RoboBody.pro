#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T00:13:01
#
#-------------------------------------------------

QT       -= gui

TARGET = RoboBody
TEMPLATE = lib

DEFINES += ROBOBODY_LIBRARY

SOURCES += rover.cpp

HEADERS += rover.h\
        robobody_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
