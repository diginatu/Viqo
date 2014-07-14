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


SOURCES += src/main.cpp\
    src/commtcp.cpp \
    src/cookieread.cpp \
    src/livedata.cpp \
    src/mainwindow.cpp \
    src/nicohttp.cpp \
    src/strabstractor.cpp \
    src/usergetter.cpp \
    src/usermanager.cpp \
		src/NicoLiveManager/nicolivemanager.cpp \
    src/NicoLiveManager/playyerstatusapi.cpp \
    src/NicoLiveManager/heartbeatapi.cpp \
    src/NicoLiveManager/Alert/alert.cpp \
    src/NicoLiveManager/Alert/wakutcp.cpp

HEADERS  += src/NicoLiveManager/nicolivemanager.h \
    src/commtcp.h \
    src/cookieread.h \
    src/livedata.h \
    src/mainwindow.h \
    src/nicohttp.h \
    src/strabstractor.h \
    src/usergetter.h \
    src/usermanager.h \
    src/NicoLiveManager/Alert/wakutcp.h

FORMS    += ui/mainwindow.ui
