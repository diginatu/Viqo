#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QJsonObject>
#include <QStandardPaths>
#include <QFile>

#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}
class MainWindow;

class Settings : public QObject
{
  Q_OBJECT
public:
  explicit Settings(MainWindow* mwin, QObject* parent = 0);

  void saveStatus(Ui::MainWindow* ui);
  void loadStatus(Ui::MainWindow* ui);
  void oldLoad(Ui::MainWindow* ui);

  void saveSettings();
  void loadSettings();

  // setter and getter
  QString getUserMail() const;
  void setUserMail(const QString& value);

  QString getUserPass() const;
  void setUserPass(const QString& value);

  int getLoginWay() const;
  void setLoginWay(int value);

  QString getUserSession() const;
  void setUserSession(const QString& value);

  QString getCookieFile() const;
  void setCookieFile(const QString& value);

  bool getOwnerComment() const;
  void setOwnerComment(bool value);

signals:

public slots:

private:
  int loginWay;
  QString userSession;
  QString cookieFile;

  QString userMail;
  QString userPass;

  bool ownerComment;

  MainWindow* mwin;
};

#endif // SETTINGS_H
