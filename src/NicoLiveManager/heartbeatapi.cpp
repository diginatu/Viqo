#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getHeartBeatAPI()
{
  if(mHeartBeatManager!=nullptr)  delete mHeartBeatManager;
  mHeartBeatManager = new QNetworkAccessManager(this);

  connect(mHeartBeatManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(heartBeatFinished(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/heartbeat?v=lv" + nowWaku.getBroadID()));

  mHeartBeatManager->get(rq);
}

void NicoLiveManager::heartBeatFinished(QNetworkReply* reply){
  QByteArray repdata = reply->readAll();
  StrAbstractor heartbeat_data(repdata);

  const QString status = heartbeat_data.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    mwin->insLog("getting the watch count failed\n");
    return;
  }

  watchCount = heartbeat_data.midStr("<watchCount>","</watchCount>");

  mwin->setWatchCount(watchCount);
  reply->deleteLater();
}
