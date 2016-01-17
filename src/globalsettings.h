#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QString>

class GlobalSettings
{
public:
  static QString settingsDir;

private:
  GlobalSettings() = delete;
};

#endif // GLOBALSETTINGS_H
