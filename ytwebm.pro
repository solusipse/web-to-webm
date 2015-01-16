#-------------------------------------------------
#
# Project created by QtCreator 2015-01-14T09:00:32
#
#-------------------------------------------------

QT       += core gui
QT       += webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ytwebm
TEMPLATE = app

SOURCES +=  src/main.cpp\
            src/mainwindow.cpp \
            src/utilities.cpp

HEADERS  += src/mainwindow.h \
            src/utilities.h

FORMS    += src/mainwindow.ui

RESOURCES += \
    resources/Resources.qrc
