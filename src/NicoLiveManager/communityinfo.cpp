#include "nicolivemanager.h"
#include "../../ui/mainwindow.h"

void NicoLiveManager::communityInfoAPI(QString commID)
{
  if(mCommunityInfo!=nullptr)  delete mCommunityInfo;
  mCommunityInfo = new QNetworkAccessManager(this);

  connect(mCommunityInfo, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(communityInfoFinished(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://com.nicovideo.jp/community/" + commID));

  mCommunityInfo->get(rq);
}

void NicoLiveManager::communityInfoFinished(QNetworkReply* reply){
  StrAbstractor community_data(QString(reply->readAll()));

  const QString commid = reply->url().toString().mid(34);
  const QString title = community_data.midStr("<h1 id=\"community_name\">", "</h1>");

  reply->deleteLater();

  fwin->gotCommunityInfo(commid, title);
}

