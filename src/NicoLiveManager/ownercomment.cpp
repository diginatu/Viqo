#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getPublishStatusAPI()
{
  QNetworkAccessManager* mManager = new QNetworkAccessManager(this);
  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getpublishstatus?v=lv" + nowWaku.getBroadID()));

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(publishStatusFinished(QNetworkReply*)));
  mManager->get(rq);
}

void NicoLiveManager::publishStatusFinished(QNetworkReply* reply){
  QByteArray repdata = reply->readAll();
  StrAbstractor heartbeat_data(repdata);

  const QString status = heartbeat_data.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    const QString code = heartbeat_data.midStr("<code>","</code>");
    mwin->insLog("NicoLiveManager::getTokenFinished fail " + code + "\n");
    return;
  }

  const QString token = heartbeat_data.midStr("<token>","</token>");
  nowWaku.setOwnerCommentToken(token);
}

void NicoLiveManager::submitOwnerCommentAPI(const QString& text, const QString& name)
{
  QNetworkAccessManager* mManager = new QNetworkAccessManager(this);
  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://watch.live.nicovideo.jp/api/broadcast/lv" +
                 nowWaku.getBroadID() +
                 "?body=" + text +
                 "&token=" + nowWaku.getOwnerCommentToken() +
                 "&name=" + name ));

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(submitOwnerCommentFinished(QNetworkReply*)));
  mManager->get(rq);
}

void NicoLiveManager::submitOwnerCommentFinished(QNetworkReply* reply){
  mwin->submittedComment();
  QByteArray repdata = reply->readAll();
  StrAbstractor heartbeat_data(repdata);

  const QString status = heartbeat_data.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    mwin->insLog("NicoLiveManager::submitOwnerCommentFinished error\n");
    return;
  }
}
