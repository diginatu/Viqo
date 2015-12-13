#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QJsonObject>
#include <QStandardPaths>
#include <QFile>
#include <QPair>

#include "ui_mainwindow.h"

class MainWindow;

enum class UserSessionWay { Browser, Direct, Login };


class Settings : public QObject
{
  Q_OBJECT
public:
  explicit Settings(MainWindow* mwin, Ui::MainWindow* ui, QObject* parent = 0);

  void updateData();

  void loadAll();

  void saveStatus(int num = 1);
  void loadStatus(int num = 1);

  void saveSettings();
  void loadSettings();

  void loadMatchDateList();
  void saveMatchDataList();

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
  bool isAutoGettingUserName();
  bool isAutoGetUserNameUseAt();
  bool isAutoGetUserNameUsePage();
  bool isAutoGetUserNameOverWrite();

  // account
  QString getUserMail() const;
  void setUserMail(QString value);

  QString getUserPass() const;
  void setUserPass(QString value);

  UserSessionWay getLoginWay() const;
  void setLoginWay(UserSessionWay value);

  QString getBrowser() const;
  void setBrowser(QString value);

  QString getUserSession() const;
  void setUserSession(QString value);

  QString getCookieFile() const;
  void setCookieFile(QString value);

  // comment
  bool getOwnerComment() const;
  void setOwnerComment(bool value);

  bool getDispNG() const;
  void setDispNG(bool value);

  // match data [[name, match contents, keyword]][]
  //
  // match contents
  // B : broad ID
  // C : Community ID
  // U : nushi ID
  QList<QStringList> matchDataList;

  bool getMatchDataEnabled() const;
  void setMatchDataEnabled(bool value);

private:
  UserSessionWay userSessionWay;
  QString browser;
  QString userSession;

  QString userMail;
  QString userPass;

  bool ownerComment;
  bool dispNG;
  bool matchDataEnabled;

  MainWindow* mwin;
  Ui::MainWindow *ui;
};

#endif // SETTINGS_H
