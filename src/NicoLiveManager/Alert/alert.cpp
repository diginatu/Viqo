#include "../nicolivemanager.h"
#include "../../mainwindow.h"

void NicoLiveManager::loginAlertAPI(QString mail, QString pass)
{
  QNetworkAccessManager* mManager = new QNetworkAccessManager(this);

  QNetworkRequest rq(QUrl("https://secure.nicovideo.jp/secure/login?site=nicolive_antenna"));
  rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QUrlQuery params;
  params.addQueryItem("mail", QUrl::toPercentEncoding(mail));
  params.addQueryItem("password", QUrl::toPercentEncoding(pass));

  connect(mManager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(loginAlertFinished(QNetworkReply*)));

  mManager->post(rq, params.toString(QUrl::FullyEncoded).toUtf8());
  // qDebug() << params.toString(QUrl::FullyEncoded).toUtf8();
}

void NicoLiveManager::loginAlertFinished(QNetworkReply* reply)
{
  mwin->insLog("NicoLiveManager::loginAlertFinished");
  QByteArray repdata = reply->readAll();

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
}


void NicoLiveManager::adminAlertAPI(QString ticket)
{
  QNetworkAccessManager* mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(adminAlertFinished(QNetworkReply*)));

  QByteArray data;
  data.append("ticket=" + ticket);

  QNetworkRequest rq(QUrl("http://live.nicovideo.jp/api/getalertstatus"));
  rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  mManager->post(rq, data);
}

void NicoLiveManager::adminAlertFinished(QNetworkReply* reply)
{
  mwin->insLog("NicoLiveManager::adminAlertFinished");
  QByteArray repdata = reply->readAll();

  StrAbstractor wakuTcpi(repdata);

  QString status = wakuTcpi.midStr("status=\"","\"");
  if (status == "fail") {
    QString code = wakuTcpi.midStr("<code>","</code>");
    mwin->insLog(code);
    return;
  }

  QByteArray mycommunities;
  mycommunities.append(wakuTcpi.midStr("<communities>","</communities>"));

  StrAbstractor communityi(mycommunities);
  QString mycommunity;
  while ((mycommunity = communityi.midStr("<community_id>","</community_id>")) != "") {
    this->mycommunities << mycommunity;
  }

  //	qDebug() << this->mycommunities;

  waku_addr = wakuTcpi.midStr("<addr>", "</addr>");
  waku_port = wakuTcpi.midStr("<port>", "</port>").toInt();
  waku_thread = wakuTcpi.midStr("<thread>", "</thread>");

  mwin->insLog("waku addr: " + waku_addr +
               "\nport: " + QString::number(waku_port) +
               "\nthread:" + waku_thread + "\n");

  if ( wakutcp != NULL ) wakutcp->deleteLater();

  wakutcp = new WakuTcp(waku_addr, waku_port, waku_thread, mwin, this);
  wakutcp->doConnect();

  mwin->insLog();
}
