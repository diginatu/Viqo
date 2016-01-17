#-------------------------------------------------
#
# Project created by QtCreator 2016-01-15T23:27:11
#
#-------------------------------------------------

QT += core gui
QT += network
QT += sql

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = viqo
TEMPLATE = app

# icons
RC_ICONS = img/icon.ico
mac{
ICON = img/icon.icns
}

SOURCES += src/main.cpp\
        src/ui/mainwindow.cpp \
    src/globalsettings.cpp

HEADERS  += src/ui/mainwindow.h \
    src/globalsettings.h

FORMS    += ui/mainwindow.ui

RESOURCES += resources.qrc
