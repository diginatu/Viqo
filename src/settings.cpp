#include "settings.h"
#include "mainwindow.h"

Settings::Settings(MainWindow* mwin, QObject* parent) :
  QObject(parent)
{
  loginWay = 0;
  userSession = "";
  cookieFile = "";

  userMail = "";
  userPass = "";

  ownerComment = true;

  this->mwin = mwin;
}

void Settings::saveStatus(Ui::MainWindow* ui)
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }

  QJsonObject other;
  other["auto_getting_user_name"] = ui->auto_getting_user_name_chk->isChecked();
  other["keep_top_comment"] = ui->keep_top_chk->isChecked();

  QJsonObject command;
  command["comment_check"] = ui->command_comment_chk->isChecked();
  command["comment"] = ui->command_comment->text();
  command["nextWaku_check"] = ui->command_nextWaku_chk->isChecked();
  command["nextWaku"] = ui->command_nextWaku->text();

  QJsonObject root;
  root["other"] = other;
  root["command"] = command;

  QJsonDocument jsd;
  jsd.setObject(root);

  QFile file(dir[0] + "/status_01.json");
  file.open(QIODevice::WriteOnly);

  QTextStream out(&file);

  out << jsd.toJson().data();

  file.close();
}

void Settings::loadStatus(Ui::MainWindow* ui)
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }
  QFile file(dir[0] + "/status_01.json");
  if ( !file.exists() ) {
    file.close();
    oldLoad(ui);
    return;
  }

  file.open(QIODevice::ReadOnly | QIODevice::Text);

  QJsonDocument jsd = QJsonDocument::fromJson(file.readAll());

  QJsonObject other;
  other = jsd.object()["other"].toObject();
  ui->auto_getting_user_name_chk->setChecked(other["auto_getting_user_name"].toBool());
  ui->keep_top_chk->setChecked(other["keep_top_comment"].toBool());

  QJsonObject command;
  command = jsd.object()["command"].toObject();
  ui->command_comment->setText(command["comment"].toString());
  ui->command_comment_chk->setChecked(command["comment_check"].toBool());
  ui->command_nextWaku->setText(command["nextWaku"].toString());
  ui->command_nextWaku_chk->setChecked(command["nextWaku_check"].toBool());

  file.close();
}

void Settings::oldLoad(Ui::MainWindow* ui)
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }
  QFile file(dir[0] + "/settings.json");
  if ( !file.exists() ) {
    file.close();
    return;
  }

  file.open(QIODevice::ReadOnly | QIODevice::Text);

  QJsonDocument jsd = QJsonDocument::fromJson(file.readAll());

  QJsonObject cookie;
  cookie = jsd.object()["cookie"].toObject();
  loginWay = cookie["browser"].toInt();
  userSession = cookie["user_session"].toString();
  cookieFile = cookie["file_name"].toString();

  QJsonObject other;
  other = jsd.object()["other"].toObject();
  ui->auto_getting_user_name_chk->setChecked(other["auto_getting_user_name"].toBool());
  ui->keep_top_chk->setChecked(other["keep_top_comment"].toBool());

  QJsonObject command;
  command = jsd.object()["command"].toObject();
  ui->command_comment->setText(command["comment"].toString());
  ui->command_comment_chk->setChecked(command["comment_check"].toBool());
  ui->command_nextWaku->setText(command["nextWaku"].toString());
  ui->command_nextWaku_chk->setChecked(command["nextWaku_check"].toBool());

  QJsonObject user_data;
  user_data = jsd.object()["user_data"].toObject();
  userMail = user_data["mail"].toString();
  userPass = user_data["pass"].toString();

  file.close();
}

void Settings::saveSettings()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }

  QJsonObject login_way;
  login_way["login_way"] = loginWay;
  login_way["user_session"] = userSession;
  login_way["cookie_file_name"] = cookieFile;

  QJsonObject user_data;
  user_data["mail"] = userMail;
  user_data["pass"] = userPass;

  QJsonObject comment;
  comment["owner_comment"] = ownerComment;


  QJsonObject root;
  root["user_data"] = user_data;
  root["login_way"] = login_way;
  root["comment"] = comment;

  QJsonDocument jsd;
  jsd.setObject(root);

  QFile file(dir[0] + "/settings.json");
  file.open(QIODevice::WriteOnly);

  QTextStream out(&file);

  out << jsd.toJson().data();

  file.close();
}

void Settings::loadSettings()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }
  QFile file(dir[0] + "/settings.json");
  if ( !file.exists() ) {
    file.close();
    return;
  }

  file.open(QIODevice::ReadOnly | QIODevice::Text);

  auto jsd = QJsonDocument::fromJson(file.readAll());

  auto login_way = jsd.object()["login_way"].toObject();
  loginWay = login_way["login_way"].toInt();
  userSession = login_way["user_session"].toString();
  cookieFile = login_way["cookie_file_name"].toString();

  auto user_data = jsd.object()["user_data"].toObject();
  userMail = user_data["mail"].toString();
  userPass = user_data["pass"].toString();

  auto comment = jsd.object()["comment"].toObject();
  if (comment.contains("owner_comment"))
    ownerComment = comment["owner_comment"].toBool();

  file.close();
}

QString Settings::getUserMail() const
{
  return userMail;
}
void Settings::setUserMail(const QString& value)
{
  userMail = value;
}

QString Settings::getUserPass() const
{
  return userPass;
}
void Settings::setUserPass(const QString& value)
{
  userPass = value;
}

int Settings::getLoginWay() const
{
  return loginWay;
}
void Settings::setLoginWay(int value)
{
  loginWay = value;
}

QString Settings::getUserSession() const
{
  return userSession;
}
void Settings::setUserSession(const QString& value)
{
  userSession = value;
}

QString Settings::getCookieFile() const
{
  return cookieFile;
}
void Settings::setCookieFile(const QString& value)
{
  cookieFile = value;
}
bool Settings::getOwnerComment() const
{
  return ownerComment;
}

void Settings::setOwnerComment(bool value)
{
  ownerComment = value;
}

