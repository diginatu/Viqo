#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T16:34:43
#
#-------------------------------------------------

QT += core gui
QT += network
QT += sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Viqo
TEMPLATE = app


SOURCES += src/main.cpp\
    src/cookieread.cpp \
    src/livedata.cpp \
    src/mainwindow.cpp \
    src/nicohttp.cpp \
    src/strabstractor.cpp \
    src/usergetter.cpp \
    src/usermanager.cpp \
    src/NicoLiveManager/nicolivemanager.cpp \
    src/NicoLiveManager/heartbeatapi.cpp \
    src/NicoLiveManager/Alert/alert.cpp \
    src/NicoLiveManager/Alert/wakutcp.cpp \
    src/NicoLiveManager/rawmylivewaku.cpp \
    src/NicoLiveManager/LiveWaku/livewaku.cpp \
    src/NicoLiveManager/LiveWaku/nowlivewaku.cpp \
    src/NicoLiveManager/LiveWaku/commtcp.cpp \
    src/settingswindow.cpp \
    src/settings.cpp \
    src/NicoLiveManager/loginapi.cpp \
    src/NicoLiveManager/postkeyapi.cpp \
    src/NicoLiveManager/ownercomment.cpp \
    src/NicoLiveManager/tagapi.cpp \
    src/NicoLiveManager/getnewwaku.cpp \
    src/newwakusettingswindow.cpp

HEADERS  += src/NicoLiveManager/nicolivemanager.h \
    src/cookieread.h \
    src/livedata.h \
    src/mainwindow.h \
    src/nicohttp.h \
    src/strabstractor.h \
    src/usergetter.h \
    src/usermanager.h \
    src/NicoLiveManager/Alert/wakutcp.h \
    src/NicoLiveManager/LiveWaku/livewaku.h \
    src/NicoLiveManager/LiveWaku/nowlivewaku.h \
    src/NicoLiveManager/LiveWaku/commtcp.h \
    src/settingswindow.h \
    src/settings.h \
    src/newwakusettingswindow.h

FORMS    += ui/mainwindow.ui \
    ui/settingswindow.ui \
    ui/newwakusettingswindow.ui
