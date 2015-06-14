#include "getstreaminfo.h"
#include "../../ui/mainwindow.h"

GetStreamInfo::GetStreamInfo(MainWindow* mwin, NicoLiveManager* nlman, const QString& broadID, QObject* parent) :
  HttpGetter(mwin, parent)
{
  this->broadID = broadID;
  this->nlman = nlman;
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

void GetStreamInfo::got(QNetworkReply* reply)
{
  reply->deleteLater();
  this->deleteLater();

  StrAbstractor receivedData(QString(reply->readAll()));

  qDebug() << receivedData.toString();

  const QString status = receivedData.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    mwin->insLog("getStreamInfo failed\n");
    mwin->insLog(receivedData.toString());
    return;
  }

  const QString broadID = receivedData.midStr("<request_id>lv", "</request_id>");
  const QString title = receivedData.midStr("<title>", "</title>");
  const QString description = receivedData.midStr("<description>", "</description>");
  const QString CommunityID = receivedData.midStr("<default_community>co", "</default_community>");

  auto insertNewWaku = [&](){
    nlman->insertLiveWakuList(new LiveWaku(mwin, nlman, broadID, CommunityID));
    mwin->refleshLiveWaku();
  };

  foreach (QStringList match, mwin->settings.matchDataList) {
    if (match[1].indexOf('T')!=-1 && title.indexOf(match[2])!=-1) {
      insertNewWaku();
      break;
    }
    if (match[1].indexOf('D')!=-1 && description.indexOf(match[2])!=-1) {
      insertNewWaku();
      break;
    }
  }
}

