#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T16:34:43
#
#-------------------------------------------------

QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Viqo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    commtcp.cpp \
    talk.cpp

HEADERS  += mainwindow.h \
    commtcp.h \
    talk.h

FORMS    += mainwindow.ui
