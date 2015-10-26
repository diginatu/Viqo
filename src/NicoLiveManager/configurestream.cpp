#include "nicolivemanager.h"
#include "../../ui/mainwindow.h"

void NicoLiveManager::configureStreamAPI(QString key, QString value, LiveWaku* waku)
{
  if (waku != nullptr)
    gotNewWaku = waku;

  if(mConfigure!=nullptr) mConfigure->deleteLater();
  mConfigure = new QNetworkAccessManager(this);

  connect(mConfigure, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(configureStreamFinished(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  auto url = QUrl(QString("http://watch.live.nicovideo.jp/api/configurestream/"
                          "lv%1?key=%2&version=2&value=%3&token=%4")
                  .arg(gotNewWaku->getBroadID()).arg(key).arg(value)
                  .arg(gotNewWaku->getBroadcastToken()));

  rq.setUrl(url);

  mConfigure->get(rq);
}

void NicoLiveManager::configureStreamFinished(QNetworkReply* reply){
  nicolive::StrAbstractor rep(QString(reply->readAll()));
  reply->deleteLater();

  QString status = rep.midStr("status=\"", "\"", false);
  if (status != "ok") {
    mwin->insLog("auto start failed");
    mwin->insLog(rep.toString());
    QMessageBox::information(mwin, "Viqo", "自動放送開始に失敗しました");
    return;
  }

  QString key = rep.midStr("key=\"", "\"", false);

  if (key == "hq") {
    configureStreamAPI("exclude", "0");
  } else if (key == "exclude") {
    gotNewWaku->deleteLater();
  }
}
