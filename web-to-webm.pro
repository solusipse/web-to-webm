#-------------------------------------------------
#
# Project created by QtCreator 2015-01-14T09:00:32
#
#-------------------------------------------------

QT       += core gui
QT       += webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.7.0

TARGET = web-to-webm
TEMPLATE = app

SOURCES +=  src/main.cpp\
            src/mainwindow.cpp \
            src/utilities.cpp \
            src/downloader.cpp \
            src/converter.cpp \
            src/window.cpp \
            src/updater.cpp

HEADERS  += src/mainwindow.h \
            src/utilities.h \
            src/downloader.h \
            src/converter.h \
            src/window.h \
            src/updater.h

FORMS    += src/mainwindow.ui \
            src/updater.ui

DEFINES += VERSION=\\\"$$VERSION\\\"

win32 {
    CONFIG(debug, debug|release) {
        renamer.commands = $(CXX) $$PWD/src/renamer.cpp && move renamer.exe debug\renamer.exe
    } else {
        renamer.commands = $(CXX) $$PWD/src/renamer.cpp && move renamer.exe release\renamer.exe
    }
}
unix:!macx {
    renamer.commands = $(CXX) $$PWD/src/renamer.cpp -o $$OUT_PWD/$(dir $(TARGET))renamer
}
macx {
    renamer.commands = mkdir -p $$OUT_PWD/$(dir $(TARGET)) && $(CXX) $$PWD/src/renamer.cpp -o $$OUT_PWD/$(dir $(TARGET))renamer
}


QMAKE_EXTRA_TARGETS += renamer
POST_TARGETDEPS += renamer

ICON = $${PWD}/resources/icons/web-to-webm.icns

RESOURCES += \
    resources/Resources.qrc
