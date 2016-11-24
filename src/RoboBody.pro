#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T00:13:01
#
#-------------------------------------------------

QT       -= gui
QT       += serialport

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
    commdata.cpp \
    pwmdrive.cpp \
    crc16.c \
    roverwheeldrive.cpp \
    angledrive.cpp \
    roverwheelangle.cpp

HEADERS += rover.h\
        robobody_global.h \
    rovermodel.h \
    abstractdevice.h \
    drive.h \
    roverimpl.h \
    servo.h \
    serialcomm.h \
    commdata.h \
    errcode.h \
    pwmdrive.h \
    RS485.h \
    warcode.h \
    crc16.h \
    roverwheeldrive.h \
    angledrive.h \
    roverwheelangle.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG += c++11

INCLUDEPATH += ../3rdparty/include

include(../3rdparty/include/qextserialport/qextserialport.prf)

include(../common.pri)
