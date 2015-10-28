#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T16:34:43
#
#-------------------------------------------------

CONFIG += c++11

RC_ICONS = img/icon.ico

mac{
ICON = img/icon.icns
}

# libs location for linux
unix:!mac{
QMAKE_LFLAGS += -Wl,--rpath="/usr/lib/viqo"
}

# libs used in nicookie
win32: LIBS += -lwininet
win32-msvc*{
LIBS += -liepmapi
}
win32: LIBS += -lcrypt32
mac: LIBS += -framework Security
unix: LIBS += -lcrypto

DEPENDPATH += . ../QtNicoLiveLib
INCLUDEPATH += ../QtNicoLiveLib
LIBS += -L../build-QtNicoLiveLib-Desktop_Qt_5_4_2_GCC_64bit-Release -lQtNicoLiveLib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += core gui
QT += network
QT += sql

TARGET = viqo
TEMPLATE = app


SOURCES += src/main.cpp\
    src/usermanager.cpp \
    src/NicoLiveManager/nicolivemanager.cpp \
    src/NicoLiveManager/Alert/alert.cpp \
    src/NicoLiveManager/Alert/wakutcp.cpp \
    src/NicoLiveManager/rawmylivewaku.cpp \
    src/NicoLiveManager/LiveWaku/livewaku.cpp \
    src/NicoLiveManager/LiveWaku/nowlivewaku.cpp \
    src/NicoLiveManager/LiveWaku/commtcp.cpp \
    src/settings.cpp \
    src/NicoLiveManager/loginapi.cpp \
    src/NicoLiveManager/ownercomment.cpp \
    src/NicoLiveManager/tagapi.cpp \
    src/NicoLiveManager/getnewwaku.cpp \
    src/NicoLiveManager/configurestream.cpp \
    ui/accountwindow.cpp \
    ui/mainwindow.cpp \
    ui/newwakusettingswindow.cpp \
    ui/settingswindow.cpp \
    src/NicoLiveManager/nicookie.cpp \
    src/NicoLiveManager/autoextend.cpp \
    ui/getwakutimer.cpp \
    src/NicoLiveManager/heartbeat.cpp \
    ui/matchandaddbroadcast.cpp \
    src/NicoLiveManager/getstreaminfo.cpp \
    src/NicoLiveManager/getcommunityinfo.cpp

HEADERS  += src/NicoLiveManager/nicolivemanager.h \
    src/usermanager.h \
    src/NicoLiveManager/Alert/wakutcp.h \
    src/NicoLiveManager/LiveWaku/livewaku.h \
    src/NicoLiveManager/LiveWaku/nowlivewaku.h \
    src/NicoLiveManager/LiveWaku/commtcp.h \
    src/settings.h \
    ui/accountwindow.h \
    ui/mainwindow.h \
    ui/newwakusettingswindow.h \
    ui/settingswindow.h \
    src/NicoLiveManager/nicookie.h \
    src/NicoLiveManager/autoextend.h \
    ui/getwakutimer.h \
    src/NicoLiveManager/heartbeat.h \
    ui/matchandaddbroadcast.h \
    src/NicoLiveManager/getstreaminfo.h \
    src/NicoLiveManager/getcommunityinfo.h

FORMS    += ui/mainwindow.ui \
    ui/settingswindow.ui \
    ui/newwakusettingswindow.ui \
    ui/accountwindow.ui \
    ui/getwakutimer.ui \
    ui/matchandaddbroadcast.ui

RESOURCES += \
    resources.qrc

DISTFILES +=
