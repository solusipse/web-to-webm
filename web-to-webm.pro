#-------------------------------------------------
#
# Project created by QtCreator 2015-01-14T09:00:32
#
#-------------------------------------------------

QT       += core gui
QT       += webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = web-to-webm
TEMPLATE = app

SOURCES +=  src/main.cpp\
            src/mainwindow.cpp \
            src/utilities.cpp \
            src/downloader.cpp \
            src/converter.cpp \
            src/window.cpp

HEADERS  += src/mainwindow.h \
            src/utilities.h \
            src/downloader.h \
            src/converter.h \
            src/window.h

FORMS    += src/mainwindow.ui

ICON = $${PWD}/resources/icons/web-to-webm.icns

RESOURCES += \
    resources/Resources.qrc
