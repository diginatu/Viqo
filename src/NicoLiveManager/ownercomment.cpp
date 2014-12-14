#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getPublishStatusAPI()
{
  if(mOwnerCommentManager!=nullptr)  delete mOwnerCommentManager;
  mOwnerCommentManager = new QNetworkAccessManager(this);

  connect(mOwnerCommentManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(publishStatusFinished(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getpublishstatus?v=lv" + nowWaku.getBroadID()));

  mOwnerCommentManager->get(rq);
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

  reply->deleteLater();
}

void NicoLiveManager::submitOwnerCommentAPI(const QString& text, const QString& name)
{
  if(mOwnerCommentSManager!=nullptr)  delete mOwnerCommentSManager;
  mOwnerCommentSManager = new QNetworkAccessManager(this);

  connect(mOwnerCommentSManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(submitOwnerCommentFinished(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://watch.live.nicovideo.jp/api/broadcast/lv" +
                 nowWaku.getBroadID() +
                 "?body=" + QUrl::toPercentEncoding(text) +
                 "&token=" + nowWaku.getOwnerCommentToken() +
                 "&name=" + QUrl::toPercentEncoding(name)));

  mOwnerCommentSManager->get(rq);
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
  reply->deleteLater();
}
