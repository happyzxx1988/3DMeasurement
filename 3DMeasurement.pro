#-------------------------------------------------
#
# Project created by QtCreator 2020-12-11T10:33:32
#
#-------------------------------------------------

QT       += core gui xlsx sql serialbus serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DMeasurement
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    appcore.cpp \
    dataoper.cpp \
    DM_Modbus.cpp \
    syssetwidget.cpp

HEADERS += \
        mainwindow.h \
    appcore.h \
    connection.h \
    dataoper.h \
    DM_Modbus.h \
    objectinfo.h \
    syssetwidget.h

FORMS += \
        mainwindow.ui \
    syssetwidget.ui

RESOURCES += \
    img.qrc
RC_ICONS=images/log11.ico
