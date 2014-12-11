#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getRawMyLiveHTML()
{
  if(mRawMyLiveManager!=nullptr)  delete mRawMyLiveManager;
  mRawMyLiveManager = new QNetworkAccessManager(this);

  connect(mRawMyLiveManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(rawMyLivefinished(QNetworkReply*)));

  if (mwin->settings.getUserSession() == "") {
    mwin->insLog("NicoLiveManager::getRawMyLiveHTML user_session is not set yet");
    return;
  }

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://www.nicovideo.jp/my/live"));

  mRawMyLiveManager->get(rq);
}

void NicoLiveManager::rawMyLivefinished(QNetworkReply* reply)
{
  mwin->insLog("NicoLiveManager::rawMyLivefinished");
  QByteArray repdata = reply->readAll();

  StrAbstractor liveID(repdata);

  // seek to Programs from the joined channels/communities if exist
  if (liveID.forwardStr("<div class=\"articleBody \" id=\"ch\">") == -1) {
    mwin->insLog("no joined channels/communities\n");
    return;
  }

  QString ID;
  QString community;
  while((community = liveID.midStr("http://com.nicovideo.jp/community/co", "\">")) != "") {
    ID = liveID.midStr("http://live.nicovideo.jp/watch/lv", "?ref=");

    // if ID contains no number charactor, it is not ID
    bool isID = true;
    for(int i = 0; i < ID.size(); ++i) {
      if( ID[i] > '9' || ID[i] < '0' ) {
        isID = false;
        break;
      }
    }
    if ( !isID ) continue;

    insertLiveWakuList(new LiveWaku(mwin, this, ID, community, this));
    mwin->insLog("added lv" + ID + " to the comunity list");
  }

  mwin->insLog();
  reply->deleteLater();
}
