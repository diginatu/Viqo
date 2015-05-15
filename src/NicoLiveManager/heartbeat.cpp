#include "heartbeat.h"
#include "../../ui/mainwindow.h"

HeartBeat::HeartBeat(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent) :
  HttpGetter(mwin, parent)
{
  this->nlman = nlman;
}

HeartBeat::~HeartBeat()
{
}

void HeartBeat::get()
{
  // make request
  QNetworkRequest rq;
  QVariant postData = NicoLiveManager::makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/heartbeat?v=lv" + nlman->nowWaku.getBroadID()));

  requestGet(rq);
}

void HeartBeat::got(QNetworkReply* reply)
{
  reply->deleteLater();
  this->deleteLater();

  StrAbstractor heartbeat_data(QString(reply->readAll()));

  const QString status = heartbeat_data.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    mwin->insLog("getting the watch count failed\n");
    mwin->insLog(heartbeat_data.toString());
    return;
  }

  QString watchCount(heartbeat_data.midStr("<watchCount>","</watchCount>"));

  if (watchCount != "") {
    nlman->setWatchCount(watchCount);
    mwin->updateWatchCount();
  }
}
