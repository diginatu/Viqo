#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getRawMyLiveHTML()
{
  QNetworkAccessManager* mManager = new QNetworkAccessManager(this);
  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://www.nicovideo.jp/my/live"));

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(rawMyLivefinished(QNetworkReply*)));
  mManager->get(rq);
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

  QString bfID;
  QString ID;
  while((ID = liveID.midStr("<a href=\"http://live.nicovideo.jp/watch/lv",
                            "?ref=")) != "") {
    if (bfID == ID) continue;

    // if ID contains no number charactor, it is not ID
    bool isID = true;
    for(int i = 0; i < ID.size(); ++i) {
      if( ID[i] > '9' || ID[i] < '0' ) {
        isID = false;
        break;
      }
    }
    if ( !isID ) continue;

    insertLiveWakuList(new LiveWaku(mwin, this, ID));
    mwin->insLog("added lv" + ID + " to the comunity list");

    bfID = ID;
  }

  mwin->insLog();
}
