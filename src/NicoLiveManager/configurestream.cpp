#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::configureStreamAPI(QString key, QString value, LiveWaku* waku)
{
  if (waku != nullptr)
    gotNewWaku = waku;

  if(mConfigure!=nullptr)  delete mConfigure;
  mConfigure = new QNetworkAccessManager(this);

  connect(mConfigure, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(configureStreamFinished(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  auto url = QUrl(QString("http://watch.live.nicovideo.jp/api/configurestream/"
                          "lv%1?key=%2&version=2&value=%3&token=%4")
                  .arg(gotNewWaku->getBroadID()).arg(key).arg(value)
                  .arg(gotNewWaku->getBroadcastToken()));

  rq.setUrl(url);

  mConfigure->get(rq);
}

void NicoLiveManager::configureStreamFinished(QNetworkReply* reply){
  qDebug() << reply->readAll();

  StrAbstractor rep(reply->readAll());
  reply->deleteLater();
  QString key = rep.midStr("key=\"", "\"/>");
  if (key == "hq") {
    configureStreamAPI("excluded", "0");
  } else if (key == "excluded") {
    delete gotNewWaku;
  }
}
