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

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += core gui
QT += network
QT += sql

TARGET = viqo
TEMPLATE = app


SOURCES += src/main.cpp\
    src/strabstractor.cpp \
    src/usermanager.cpp \
    src/NicoLiveManager/nicolivemanager.cpp \
    src/NicoLiveManager/heartbeatapi.cpp \
    src/NicoLiveManager/Alert/alert.cpp \
    src/NicoLiveManager/Alert/wakutcp.cpp \
    src/NicoLiveManager/rawmylivewaku.cpp \
    src/NicoLiveManager/LiveWaku/livewaku.cpp \
    src/NicoLiveManager/LiveWaku/nowlivewaku.cpp \
    src/NicoLiveManager/LiveWaku/commtcp.cpp \
    src/settings.cpp \
    src/NicoLiveManager/loginapi.cpp \
    src/NicoLiveManager/postkeyapi.cpp \
    src/NicoLiveManager/ownercomment.cpp \
    src/NicoLiveManager/tagapi.cpp \
    src/NicoLiveManager/getnewwaku.cpp \
    src/NicoLiveManager/configurestream.cpp \
    ui/accountwindow.cpp \
    ui/followcommunity.cpp \
    ui/mainwindow.cpp \
    ui/newwakusettingswindow.cpp \
    ui/settingswindow.cpp \
    src/NicoLiveManager/communityinfo.cpp \
    src/NicoLiveManager/nicookie.cpp \
    src/NicoLiveManager/httpgetter.cpp \
    src/NicoLiveManager/autoextend.cpp \
    src/NicoLiveManager/usernamegetter.cpp

HEADERS  += src/NicoLiveManager/nicolivemanager.h \
    src/strabstractor.h \
    src/usermanager.h \
    src/NicoLiveManager/Alert/wakutcp.h \
    src/NicoLiveManager/LiveWaku/livewaku.h \
    src/NicoLiveManager/LiveWaku/nowlivewaku.h \
    src/NicoLiveManager/LiveWaku/commtcp.h \
    src/settings.h \
    ui/accountwindow.h \
    ui/followcommunity.h \
    ui/mainwindow.h \
    ui/newwakusettingswindow.h \
    ui/settingswindow.h \
    src/NicoLiveManager/nicookie.h \
    src/NicoLiveManager/httpgetter.h \
    src/NicoLiveManager/autoextend.h \
    src/NicoLiveManager/usernamegetter.h

FORMS    += ui/mainwindow.ui \
    ui/settingswindow.ui \
    ui/newwakusettingswindow.ui \
    ui/accountwindow.ui \
    ui/followcommunity.ui

RESOURCES += \
    resources.qrc

DISTFILES +=
