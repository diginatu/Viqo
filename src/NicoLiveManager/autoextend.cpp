#include "autoextend.h"
#include "../../ui/mainwindow.h"

AutoExtend::AutoExtend(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent) :
  HttpGetter(mwin, nlman, parent),
  mExtendManager(nullptr)
{

}

void AutoExtend::get()
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(got(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = nlman->makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://watch.live.nicovideo.jp/api/getsalelist?v=lv" + nlman->nowWaku.getBroadID()));

  mManager->get(rq);
}

AutoExtend::~AutoExtend()
{

}

void AutoExtend::got(QNetworkReply *reply)
{
  StrAbstractor data(QString(reply->readAll()));

  StrAbstractor* aitem;
  while ((aitem = data.mid("<item>", "</item>")) != nullptr) {
    // QString price = aitem->midStr("<price>", "</price>");
    QString num   = aitem->midStr("<num>", "</num>");
    QString code  = aitem->midStr("<code>", "</code>");
    QString item  = aitem->midStr("<item>", "");

    if (item == "freeextend") {
      getExtend(code, item, num);
    }
  }

  reply->deleteLater();
  this->deleteLater();
}

void AutoExtend::getExtend(QString code, QString item, QString num)
{
  if(mExtendManager!=nullptr) delete mExtendManager;
  mExtendManager = new QNetworkAccessManager(this);

  connect(mExtendManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(gotExtend(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = nlman->makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl(
              QString("http://watch.live.nicovideo.jp/api/usepoint?v=lv%1&code=%2&item=%3&token=%4&num=%5")
              .arg(nlman->nowWaku.getBroadID())
              .arg(code).arg(item)
              .arg(nlman->nowWaku.getBroadcastToken())
              .arg(num)
              ));

  mExtendManager->get(rq);
}

void AutoExtend::gotExtend(QNetworkReply* reply)
{
  StrAbstractor data(QString(reply->readAll()));

  qDebug() << data.toString();

  reply->deleteLater();
}

