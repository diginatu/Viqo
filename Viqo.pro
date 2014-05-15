#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T16:34:43
#
#-------------------------------------------------

QT += core gui
QT += network
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Viqo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    commtcp.cpp \
    cookieread.cpp \
	usermanager.cpp \
    livedata.cpp \
    strabstractor.cpp \
    nicohttp.cpp

HEADERS  += mainwindow.h \
    commtcp.h \
    cookieread.h \
	usermanager.h \
    livedata.h \
    strabstractor.h \
    nicohttp.h

FORMS    += mainwindow.ui
