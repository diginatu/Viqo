#include "../nicolivemanager.h"
#include "../../mainwindow.h"

void NicoLiveManager::loginAlertAPI(const QString& mail, const QString& pass)
{
  if(mLoginAlertManager!=nullptr)  delete mLoginAlertManager;
  mLoginAlertManager = new QNetworkAccessManager(this);

  connect(mLoginAlertManager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(loginAlertFinished(QNetworkReply*)));

  QNetworkRequest rq(QUrl("https://secure.nicovideo.jp/secure/login?site=nicolive_antenna"));
  rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QUrlQuery params;
  params.addQueryItem("mail", QUrl::toPercentEncoding(mail));
  params.addQueryItem("password", QUrl::toPercentEncoding(pass));

  mLoginAlertManager->post(rq, params.toString(QUrl::FullyEncoded).toUtf8());
}

void NicoLiveManager::loginAlertFinished(QNetworkReply* reply)
{
  mwin->insLog("NicoLiveManager::loginAlertFinished");
  QString repdata = QString(reply->readAll());

  StrAbstractor commTcpi(repdata);

  QString status = commTcpi.midStr("status=\"","\"");
  if (status == "fail") {
    const QString code = commTcpi.midStr("<code>","</code>");
    const QString description = commTcpi.midStr("<description>","</description>");
    mwin->insLog(code + "\n" + description + "\n");
    return;
  }

  QString ticket = commTcpi.midStr("<ticket>","</ticket>");

  adminAlertAPI(ticket);
  mwin->insLog();
  reply->deleteLater();
}


void NicoLiveManager::adminAlertAPI(const QString& ticket)
{
  if(mAdminAlertManager!=nullptr)  delete mAdminAlertManager;
  mAdminAlertManager = new QNetworkAccessManager(this);

  connect(mAdminAlertManager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(adminAlertFinished(QNetworkReply*)));

  QByteArray data;
  data.append("ticket=" + ticket);

  QNetworkRequest rq(QUrl("http://live.nicovideo.jp/api/getalertstatus"));
  rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  mAdminAlertManager->post(rq, data);
}

void NicoLiveManager::adminAlertFinished(QNetworkReply* reply)
{
  mwin->insLog("NicoLiveManager::adminAlertFinished");
  QString repdata = QString(reply->readAll());

  StrAbstractor wakuTcpi(repdata);

  QString status = wakuTcpi.midStr("status=\"","\"");
  if (status == "fail") {
    QString code = wakuTcpi.midStr("<code>","</code>");
    mwin->insLog(code);
    return;
  }

  QString mycommunities = wakuTcpi.midStr("<communities>","</communities>");

  StrAbstractor communityi(mycommunities);
  QString mycommunity;
  this->mycommunities.clear();
  while ((mycommunity = communityi.midStr("<community_id>","</community_id>")) != "") {
    this->mycommunities << mycommunity;
  }

  waku_addr = wakuTcpi.midStr("<addr>", "</addr>");
  waku_port = wakuTcpi.midStr("<port>", "</port>").toInt();
  waku_thread = wakuTcpi.midStr("<thread>", "</thread>");

  // mwin->insLog("waku addr: " + waku_addr +
  //              "\nport: " + QString::number(waku_port) +
  //              "\nthread:" + waku_thread + "\n");

  if ( wakutcp != nullptr ) {
    if ( wakutcp->isConnected() )
      wakutcp->close();
    wakutcp->deleteLater();
  }

  wakutcp = new WakuTcp(waku_addr, waku_port, waku_thread, mwin, this);
  wakutcp->doConnect();

  mwin->insLog();
  reply->deleteLater();
}

void NicoLiveManager::alertReconnect()
{
  QString mail = mwin->settings.getUserMail();
  QString pass = mwin->settings.getUserPass();

  if ( mail == "" || pass == "") {
    mwin->insLog("mail or pass are not specified");
    return;
  }
  loginAlertAPI(mail, pass);
  getRawMyLiveHTML();
  QTimer::singleShot(30000, this, SLOT(getRawMyLiveHTML()));
}
