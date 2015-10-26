#include "getcommunityinfo.h"
#include "../../ui/mainwindow.h"

GetCommunityInfo::GetCommunityInfo(MainWindow* mwin, QString commID, QObject* parent) :
  HttpGetter(parent)
{
  this->mwin = mwin;
  this->commID = commID;
}

void GetCommunityInfo::get()
{
  // make request
  QNetworkRequest rq;
  QVariant postData = NicoLiveManager::makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://com.nicovideo.jp/community/" + commID));

  requestGet(rq);
}

void GetCommunityInfo::gotReply(QNetworkReply* reply)
{
  reply->deleteLater();
  this->deleteLater();

  nicolive::StrAbstractor communityData(QString(reply->readAll()));

  const QString title = communityData.midStr("<h1 id=\"community_name\">", "</h1>");

  emit got(commID, title);
}
