#include "settings.h"
#include "../ui/mainwindow.h"

Settings::Settings(MainWindow* mwin, Ui::MainWindow* ui, QObject* parent) :
  QObject(parent)
{
  userSessionWay = UserSessionWay::Direct;
  ownerComment = true;
  dispNG = true;

  this->mwin = mwin;
  this->ui = ui;
}

void Settings::loadAll()
{
  loadSettings();
  loadStatus();
  loadFollowCommunities();
}

void Settings::saveStatus(int num)
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    QMessageBox::information(mwin, "Viqo", "保存領域がないので保存できません");
    return;
  }

  QJsonObject other;
  other["keep_top_comment"] = ui->keepTop->isChecked();
  other["is184_comment"] = ui->is184_chk->isChecked();
  other["auto_getting_new_waku"] = ui->autoNewWakuChk->isChecked();
  other["new_waku_open_browser"] = ui->autoNewWakuOpenBrowser->isChecked();
  other["new_waku_start"] = ui->autoNewWakuStart->isChecked();

  QJsonObject comment;
  comment["autoGettingUserName"] = ui->autoGettingUserName->isChecked();
  comment["autoGetUserNameUseAt"] = ui->autoGetUserNameUseAt->isChecked();
  comment["autoGetUserNameUsePage"] = ui->autoGetUserNameUsePage->isChecked();
  comment["autoGetUserNameOverWrite"] = ui->autoGetUserNameOverWrite->isChecked();

  QJsonObject command;
  command["comment_check"] = ui->command_comment_chk->isChecked();
  command["comment"] = ui->command_comment->text();
  command["nextWaku_check"] = ui->command_nextWaku_chk->isChecked();
  command["nextWaku"] = ui->command_nextWaku->text();
  command["command_beforeEnd_check"] = ui->command_beforeEnd_chk->isChecked();
  command["command_beforeEnd_minuts"] = ui->command_beforeEndMinuts_spn->value();
  command["command_beforeEnd"] = ui->command_beforeEnd->text();
  command["newWaku_check"] = ui->command_newWaku_chk->isChecked();
  command["newWaku"] = ui->command_newWaku->text();

  QJsonObject root;
  root["other"] = other;
  root["comment"] = comment;
  root["command"] = command;

  QJsonDocument jsd;
  jsd.setObject(root);

  QFile file(dir[0] + "/status_0" + QString::number(num) + ".json");
  if (!file.open(QIODevice::WriteOnly)) {
    file.close();
    mwin->insLog("opening status file failed");
    QMessageBox::information(mwin, "Viqo", "ファイル書き込みができません");
    return;
  }

  QTextStream out(&file);

  out << jsd.toJson();

  file.close();
}

void Settings::loadStatus(int num)
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    QMessageBox::information(mwin, "Viqo", "保存領域がないので保存できません");
    return;
  }
  QFile file(dir[0] + "/status_0" + QString::number(num) + ".json");
  if ( !file.exists() ) {
    file.close();
    if (num == 0) {
      oldLoad();
    }
    mwin->insLog("no save file : " + QString::number(num));
    return;
  }

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    file.close();
    mwin->insLog("opening status file failed");
    QMessageBox::information(mwin, "Viqo", "ステートファイルオープンに失敗しました");
    return;
  }

  QJsonDocument jsd = QJsonDocument::fromJson(file.readAll());

  QJsonObject other = jsd.object()["other"].toObject();
  ui->keepTop->setChecked(other["keep_top_comment"].toBool(true));
  ui->is184_chk->setChecked(other["is184_comment"].toBool());
  ui->autoNewWakuChk->setChecked(other["auto_getting_new_waku"].toBool());
  ui->autoNewWakuOpenBrowser->setChecked(other["new_waku_open_browser"].toBool(true));
  ui->autoNewWakuStart->setChecked(other["new_waku_start"].toBool());

  QJsonObject comment = jsd.object()["comment"].toObject();
  ui->autoGettingUserName->setChecked(comment["autoGettingUserName"].toBool(true));
  ui->autoGetUserNameUseAt->setChecked(comment["autoGetUserNameUseAt"].toBool(true));
  ui->autoGetUserNameUsePage->setChecked(comment["autoGetUserNameUsePage"].toBool(true));
  ui->autoGetUserNameOverWrite->setChecked(comment["autoGetUserNameOverWrite"].toBool(true));

  QJsonObject command = jsd.object()["command"].toObject();
  ui->command_comment->setText(command["comment"].toString());
  ui->command_comment_chk->setChecked(command["comment_check"].toBool());
  ui->command_nextWaku->setText(command["nextWaku"].toString());
  ui->command_nextWaku_chk->setChecked(command["nextWaku_check"].toBool());
  ui->command_beforeEnd_chk->setChecked(command["command_beforeEnd_check"].toBool());
  ui->command_beforeEndMinuts_spn->setValue(command["command_beforeEnd_minuts"].toInt(3));
  ui->command_beforeEnd->setText(command["command_beforeEnd"].toString());
  ui->command_newWaku->setText(command["newWaku"].toString());
  ui->command_newWaku_chk->setChecked(command["newWaku_check"].toBool());

  file.close();
}

void Settings::oldLoad()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }
  QFile file(dir[0] + "/settings.json");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    file.close();
    mwin->insLog("opening status file failed");
    return;
  }

  QJsonDocument jsd = QJsonDocument::fromJson(file.readAll());

  QJsonObject cookie;
  cookie = jsd.object()["cookie"].toObject();
  userSessionWay = UserSessionWay(cookie["browser"].toInt());
  userSession = cookie["user_session"].toString();
  cookieFile = cookie["file_name"].toString();

  QJsonObject other;
  other = jsd.object()["other"].toObject();
  ui->keepTop->setChecked(other["keep_top_comment"].toBool());

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
    QMessageBox::information(mwin, "Viqo", "保存領域がないので保存できません");
    return;
  }

  QJsonObject login_way;
  login_way["login_way"] = static_cast<int>(userSessionWay);
  login_way["user_session"] = userSession;
  login_way["cookie_file_name"] = cookieFile;

  QJsonObject user_data;
  user_data["mail"] = userMail;
  user_data["pass"] = userPass;

  QJsonObject comment;
  comment["owner_comment"] = ownerComment;
  comment["dispNG"] = dispNG;


  QJsonObject root;
  root["user_data"] = user_data;
  root["login_way"] = login_way;
  root["comment"] = comment;

  QJsonDocument jsd;
  jsd.setObject(root);

  QFile file(dir[0] + "/settings.json");
  if (!file.open(QIODevice::WriteOnly)) {
    file.close();
    mwin->insLog("opening status file failed");
    QMessageBox::information(mwin, "Viqo", "設定ファイルに書き込みがせきません");
    return;
  }

  QTextStream out(&file);

  out << jsd.toJson();

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
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    file.close();
    mwin->insLog("opening setting file failed");
    return;
  }

  QJsonDocument jsd = QJsonDocument::fromJson(file.readAll());

  QJsonObject login_way = jsd.object()["login_way"].toObject();
  userSessionWay = UserSessionWay(login_way["login_way"].toInt());
  userSession = login_way["user_session"].toString();
  cookieFile = login_way["cookie_file_name"].toString();

  QJsonObject user_data = jsd.object()["user_data"].toObject();
  userMail = user_data["mail"].toString();
  userPass = user_data["pass"].toString();

  QJsonObject comment = jsd.object()["comment"].toObject();
  if (comment.contains("owner_comment"))
    ownerComment = comment["owner_comment"].toBool();
  if (comment.contains("dispNG"))
    dispNG = comment["dispNG"].toBool();

  file.close();
}

void Settings::saveFollowCommunities()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    QMessageBox::information(mwin, "Viqo", "保存領域がないので保存できません");
    return;
  }

  QJsonArray follow_communities;

  typedef QPair<QString,QString> StringPair;
  foreach (StringPair community, followCommunities) {
    follow_communities.append(QJsonArray() << community.first << community.second);
  }

  QJsonObject root;
  root["follow_communities"] = follow_communities;

  QJsonDocument jsd;
  jsd.setObject(root);

  QFile file(dir[0] + "/follow_communities.json");
  if (!file.open(QIODevice::WriteOnly)) {
    file.close();
    mwin->insLog("opening status file failed");
    QMessageBox::information(mwin, "Viqo", "設定ファイルに書き込みがせきません");
    return;
  }

  QTextStream out(&file);
  out << jsd.toJson(QJsonDocument::Compact);
  file.close();
}

void Settings::loadFollowCommunities()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }
  QFile file(dir[0] + "/follow_communities.json");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    file.close();
    mwin->insLog("opening Follow Communities setting file failed");
    return;
  }

  QJsonDocument jsd = QJsonDocument::fromJson(file.readAll());

  QJsonArray follow_communities = jsd.object()["follow_communities"].toArray();
  followCommunities.clear();
  foreach (QJsonValue community, follow_communities) {
    followCommunities << qMakePair(community.toArray()[0].toString(), community.toArray()[1].toString());
  }

  file.close();
}

bool Settings::getIs184()
{
  return ui->is184_chk->isChecked();
}

bool Settings::isAutoNextWaku()
{
  return ui->auto_next_waku->isChecked();
}

bool Settings::isCommandCommentChecked() {
  return ui->command_comment_chk->isChecked();
}
QString Settings::getCommandComment() {
  return ui->command_comment->text();
}

bool Settings::isCommandNextWakuChecked() {
  return ui->command_nextWaku_chk->isChecked();
}
QString Settings::getCommandNextWaku() {
  return ui->command_nextWaku->text();
}

bool Settings::isCommandNewWakuChecked()
{
  return ui->command_newWaku_chk->isChecked();
}
QString Settings::getCommandNewWaku()
{
  return ui->command_newWaku->text();
}

bool Settings::isAutoNewWaku()
{
  return ui->autoNewWakuChk->isChecked();
}

bool Settings::isAutoNewWakuOpenBrowser()
{
  return ui->autoNewWakuOpenBrowser->isChecked();
}

bool Settings::isAutoNewWakuStart()
{
  return ui->autoNewWakuStart->isChecked();
}

bool Settings::isAutoGettingUserName()
{
  return ui->autoGettingUserName->isChecked();
}

bool Settings::isAutoGetUserNameUseAt()
{
  return ui->autoGetUserNameUseAt->isChecked();
}

bool Settings::isAutoGetUserNameUsePage()
{
  return ui->autoGetUserNameUsePage->isChecked();
}

bool Settings::isAutoGetUserNameOverWrite()
{
  return ui->autoGetUserNameOverWrite->isChecked();
}

QString Settings::getUserMail() const
{
  return userMail;
}
void Settings::setUserMail(QString value)
{
  userMail = value;
}

QString Settings::getUserPass() const
{
  return userPass;
}
void Settings::setUserPass(QString value)
{
  userPass = value;
}

UserSessionWay Settings::getLoginWay() const
{
  return userSessionWay;
}
void Settings::setLoginWay(UserSessionWay value)
{
  userSessionWay = value;
}

QString Settings::getUserSession() const
{
  return userSession;
}

void Settings::setUserSession(QString value)
{
  userSession = value;
}

QString Settings::getCookieFile() const
{
  return cookieFile;
}

void Settings::setCookieFile(QString value)
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

bool Settings::getDispNG() const
{
  return dispNG;
}

void Settings::setDispNG(bool value)
{
  dispNG = value;
}
