#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QJsonObject>
#include <QStandardPaths>
#include <QFile>
#include <QPair>

#include "ui_mainwindow.h"

class MainWindow;
class FollowCommunity;

enum UserSessionWay { Direct, Firefox, Login };

class Settings : public QObject
{
  Q_OBJECT
public:
  explicit Settings(MainWindow* mwin, Ui::MainWindow* ui, QObject* parent = 0);

  void loadAll();

  void saveStatus(int num = 1);
  void loadStatus(int num = 1);
  void oldLoad();

  void saveSettings();
  void loadSettings();

  void saveFollowCommunities();
  void loadFollowCommunities();

  // main window settings
  bool getIs184();
  bool isAutoNextWaku();
  bool isCommandCommentChecked();
  QString getCommandComment();
  bool isCommandNextWakuChecked();
  QString getCommandNextWaku();
  bool isCommandNewWakuChecked();
  QString getCommandNewWaku();
  bool isAutoNewWaku();
  bool isAutoNewWakuOpenBrowser();
  bool isAutoNewWakuStart();

  // account
  QString getUserMail() const;
  void setUserMail(QString value);

  QString getUserPass() const;
  void setUserPass(QString value);

  UserSessionWay getLoginWay() const;
  void setLoginWay(UserSessionWay value);

  QString getUserSession() const;
  void setUserSession(QString value);

  QString getCookieFile() const;
  void setCookieFile(QString value);

  // comment
  bool getOwnerComment() const;
  void setOwnerComment(bool value);

  bool getDispNG() const;
  void setDispNG(bool value);

  QList< QPair<QString,QString> > followCommunities;
signals:

public slots:

private slots:

private:
  UserSessionWay userSessionWay;
  QString userSession;
  QString cookieFile;

  QString userMail;
  QString userPass;

  bool ownerComment;
  bool dispNG;

  MainWindow* mwin;
  Ui::MainWindow *ui;
};

#endif // SETTINGS_H
