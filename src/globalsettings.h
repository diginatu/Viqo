#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QString>

class GlobalSettings
{
public:
    // global settings
    static bool debugToStd;
    static QString settingsDir;

    // niconico
    static QString nicoUserMail;
    static QString nicoUserPass;
    static QString userSession;

    // account settings
    static QString gettingUsersessionWay;

private:
    GlobalSettings() = delete;
};

#endif // GLOBALSETTINGS_H
