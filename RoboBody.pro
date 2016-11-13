#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T00:13:01
#
#-------------------------------------------------

QT       -= gui

TARGET = RoboBody
TEMPLATE = lib

DEFINES += ROBOBODY_LIBRARY

SOURCES += rover.cpp \
    rovermodel.cpp \
    abstractdevice.cpp \
    drive.cpp \
    roverimpl.cpp \
    servo.cpp \
    serialcomm.cpp \
    commdata.cpp

HEADERS += rover.h\
        robobody_global.h \
    rovermodel.h \
    abstractdevice.h \
    drive.h \
    roverimpl.h \
    servo.h \
    serialcomm.h \
    commdata.h \
    errcode.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG += c++11
