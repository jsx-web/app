#-------------------------------------------------
#
# Project created by QtCreator 2023-04-19T08:58:28
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += concurrent
QT += widgets
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test1
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
        widget.cpp \
    client.cpp \
    frame.cpp \
    session.cpp \
    config.cpp \
    file.cpp \
    readfiledialog.cpp \
    fileoperation.cpp \
    dialog_showall.cpp \
    bufferqueues.cpp

HEADERS += \
        widget.h \
    client.h \
    config.h \
    frame.h \
    session.h \
    file.h \
    readfiledialog.h \
    fileoperation.h \
    dialog_showall.h \
    bufferqueues.h

FORMS += \
        widget.ui \
    readfiledialog.ui \
    dialog_showall.ui

LIBS += -lpthread libwsock32 libws2_32

RESOURCES += \
    res.qrc
