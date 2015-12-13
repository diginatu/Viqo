#include "alertmanager.h"
#include "../../ui/mainwindow.h"
#include "nicolivemanager.h"

AlertManager::AlertManager(MainWindow* mwin, NicoLiveManager* nlman, QObject *parent) :
  QObject(parent),
  mwin(mwin),
  nlman(nlman)
{
}

QStringList AlertManager::getMycommunities() const
{
  return mycommunities;
}

void AlertManager::login(const QString& mail, const QString& pass,
                         bool willConnect)
{
  if ( mail.isEmpty() || pass.isEmpty() ) {
    qDebug() << "mail or pass is not specified";
    QMessageBox::information(mwin, "Viqo",
            QStringLiteral("メールまたはパスワードが設定されていません"));
    return;
  }

  auto login = new nicolive::AlertLogin(this);

  connect(login, &nicolive::AlertLogin::error,
          this, [=](QString code, QString description){
    qDebug() << "login error : " << code << "\n" << description;
    QMessageBox::information(mwin, "Viqo",
            QStringLiteral("放送開始通知のログイン(AlertLogin)に失敗しました\n\
                           メールアドレスとパスワードを確認してください\n") +
            code + " : " + description);
  });
  connect(login, &nicolive::AlertLogin::got, this, [=](QString _ticket){
    ticket = _ticket;
    emit gotLogin();
    if (willConnect) admin(true);
  });

  login->get(mail, pass);
}

void AlertManager::admin(bool willConnect)
{
  if (ticket.isEmpty()) {
    qDebug() << "admin error : ticket is empty";
    return;
  }

  auto admin = new nicolive::AlertAdmin(this);

  connect(admin, &nicolive::AlertAdmin::error, this, [=](QString code){
    qDebug() << "admin error : " << code;
    QMessageBox::information(mwin, "Viqo",
           QStringLiteral("放送開始通知の情報取得(AlertAdmin)に失敗しました\ncode : ")
           + code);
  });
  connect(admin, &nicolive::AlertAdmin::got,
          this, [&, willConnect](QStringList _myCommunities, QString _addr,
          qint16 _port, QString _thread){
    addr = _addr;
    port = _port;
    thread = _thread;
    mycommunities = qMove(_myCommunities);
    emit gotAdmin();
    if (willConnect) doConnect();
  });

  admin->get(ticket);
}

void AlertManager::doConnect()
{
  alertConnection = QSharedPointer<nicolive::AlertConnection>::create(
        addr, port, thread, this);

  connect(alertConnection.data(), &nicolive::AlertConnection::error,
          this, [=](QString errorPosition, QString code){
    qDebug() << "alert error " << errorPosition << " : " << code;
    QMessageBox::information(mwin, "Viqo",
           QStringLiteral("放送開始通知の接続に失敗しました\n") +
           errorPosition + " : " + code);
  });
  connect(alertConnection.data(), &nicolive::AlertConnection::gotNewWaku,
          this, &AlertManager::newWakuCatched);

  alertConnection->doConnect();
}

void AlertManager::disconnect()
{
  alertConnection->close();
}

void AlertManager::newWakuCatched(QString broadID, QString communityID, QString nushiID)
{
  nlman->allGotWakuInfo(communityID, broadID);

  auto insertNewWaku = [&](){
    nlman->insertLiveWakuList(new LiveWaku(mwin, nlman, broadID, communityID));
    mwin->refleshLiveWaku();
  };

  for (QString commu : mycommunities) {
    if (commu == communityID) insertNewWaku();
  }

  if (mwin->settings.getMatchDataEnabled()) {
    for (QStringList match : mwin->settings.matchDataList) {
      if (match[1].indexOf('B')!=-1 && match[2] == broadID) {
        insertNewWaku();
        break;
      }
      if (match[1].indexOf('C')!=-1 && match[2] == communityID) {
        insertNewWaku();
        break;
      }
      if (match[1].indexOf('U')!=-1 && match[2] == nushiID) {
        insertNewWaku();
        break;
      }
    }
  }
}

