#-------------------------------------------------
#
# Project created by QtCreator 2019-10-01T13:01:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT_UI
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
    qt/client.cpp \
    qt/clientsettings.cpp \
    qt/mainloopworker.cpp \
    qt/mainwindow.cpp \
    qt/settingsdialog.cpp \
    qt/statuscheckerworker.cpp \
    qt/threadcontroller.cpp \
    qt/worker.cpp
HEADERS += \
    qt/client.h \
    qt/clientsettings.h \
    qt/mainloopworker.h \
    qt/mainwindow.h \
    qt/settingsdialog.h \
    qt/statuscheckerworker.h \
    qt/threadcontroller.h \
    qt/worker.h

FORMS += \
    qt/mainwindow.ui \
    qt/settingsdialog.ui

INCLUDEPATH += $$PWD/qt
INCLUDEPATH += $$PWD/../TCPIP_CLIENT_DLL/public
