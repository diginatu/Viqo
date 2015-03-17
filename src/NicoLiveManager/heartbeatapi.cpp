#include "nicolivemanager.h"
#include "../../ui/mainwindow.h"

void NicoLiveManager::getHeartBeatAPI()
{
  if(mHeartBeat!=nullptr)  delete mHeartBeat;
  mHeartBeat = new QNetworkAccessManager(this);

  connect(mHeartBeat, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(heartBeatFinished(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/heartbeat?v=lv" + nowWaku.getBroadID()));

  mHeartBeat->get(rq);
}

void NicoLiveManager::heartBeatFinished(QNetworkReply* reply){
  QString repdata = QString(reply->readAll());
  StrAbstractor heartbeat_data(repdata);

  const QString status = heartbeat_data.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    mwin->insLog("getting the watch count failed\n");
    return;
  }

  watchCount = heartbeat_data.midStr("<watchCount>","</watchCount>");

  if (watchCount == "") return;
  mwin->setWatchCount(watchCount);
  reply->deleteLater();
}
