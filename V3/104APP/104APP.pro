#-------------------------------------------------
#
# Project created by QtCreator 2023-05-11T20:11:47
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += concurrent
QT += widgets
QT += svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 104APP
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
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
    terminal.cpp \
    config.cpp \
    file.cpp \
    fileoperation.cpp \
    frame.cpp \
    session.cpp \
    clock.cpp \
    info.cpp \
    infobuffer.cpp \
    dialog_writevalueadd.cpp

HEADERS += \
        mainwindow.h \
    terminal.h \
    config.h \
    file.h \
    fileoperation.h \
    frame.h \
    session.h \
    clock.h \
    info.h \
    infobuffer.h \
    dialog_writevalueadd.h

FORMS += \
        mainwindow.ui \
    dialog_writevalueadd.ui

LIBS += -lpthread libwsock32 libws2_32

RESOURCES += \
    res.qrc
