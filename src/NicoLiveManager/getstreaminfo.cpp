#include "getstreaminfo.h"
#include "../../ui/mainwindow.h"

GetStreamInfo::GetStreamInfo(MainWindow* mwin, const QString& broadID, QObject* parent) :
  HttpGetter(parent)
{
  this->mwin = mwin;
  this->broadID = broadID;
}

GetStreamInfo::~GetStreamInfo()
{

}

void GetStreamInfo::get()
{
  // make request
  QNetworkRequest rq;
  QVariant postData = NicoLiveManager::makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getstreaminfo/lv" + broadID));

  requestGet(rq);
}

void GetStreamInfo::gotReply(QNetworkReply* reply)
{
  reply->deleteLater();
  this->deleteLater();

  nicolive::StrAbstractor receivedData(QString(reply->readAll()));

  const QString status = receivedData.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    mwin->insLog("getStreamInfo failed\n");
    mwin->insLog(receivedData.toString());
    return;
  }

  const QString broadID = receivedData.midStr("<request_id>lv", "</request_id>");
  const QString title = receivedData.midStr("<title>", "</title>");
  const QString description = receivedData.midStr("<description>", "</description>");
  const QString communityID = receivedData.midStr("<default_community>co", "</default_community>");

  emit got(broadID, title, description, communityID);
}

