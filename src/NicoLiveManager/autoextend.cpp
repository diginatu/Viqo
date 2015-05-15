#include "autoextend.h"
#include "../../ui/mainwindow.h"

AutoExtend::AutoExtend(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent) :
  HttpGetter(mwin, parent),
  mExtendManager(nullptr)
{
  this->nlman = nlman;
}

AutoExtend::~AutoExtend()
{

}

void AutoExtend::get()
{
  // make request
  QNetworkRequest rq;
  QVariant postData = nlman->makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://watch.live.nicovideo.jp/api/getsalelist?v=lv" + nlman->nowWaku.getBroadID()));

  requestGet(rq);
}

void AutoExtend::got(QNetworkReply *reply)
{
  StrAbstractor data(QString(reply->readAll()));

  bool haveFree = false;
  StrAbstractor* aitem;
  while ((aitem = data.mid("<item>", "</item>")) != nullptr) {
    // QString price = aitem->midStr("<price>", "</price>");
    QString num  = aitem->midStr("<num>", "</num>");
    QString code = aitem->midStr("<code>", "</code>");
    QString item = aitem->midStr("<item>", "");

    if (item == "freeextend") {
      getExtend(code, item, num);
      haveFree = true;
      break;
    }
  }

  if ( !haveFree ) {
    mwin->insLog("no freeExtend");
    this->deleteLater();
  }

  reply->deleteLater();
}

void AutoExtend::getExtend(QString code, QString item, QString num)
{
  if(mExtendManager!=nullptr) delete mExtendManager;
  mExtendManager = new QNetworkAccessManager(this);

  connect(mExtendManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(gotExtend(QNetworkReply*)));


  QNetworkRequest rq(QUrl("http://watch.live.nicovideo.jp/api/usepoint/lv" +
                          nlman->nowWaku.getBroadID()));
  rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  QVariant postData = nlman->makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setRawHeader("accept-language", "ja");

  QUrlQuery params;
  params.addQueryItem("num", num);
  // params.addQueryItem("remain", "%2D1");
  params.addQueryItem("date", QString::number(QDateTime::currentDateTimeUtc().toTime_t()));
  params.addQueryItem("item", item);
  params.addQueryItem("code", QUrl::toPercentEncoding(code));
  params.addQueryItem("token", QUrl::toPercentEncoding(nlman->nowWaku.getBroadcastToken()));

  mExtendManager->post(rq, params.toString(QUrl::FullyEncoded).toUtf8());
}

void AutoExtend::gotExtend(QNetworkReply* reply)
{
  StrAbstractor data(QString(reply->readAll()));

  QString mode = data.midStr("<mode>", "</mode>");

  if (mode == "extend")
    mwin->insLog("auto extended");

  StrAbstractor* error = data.mid("<error>", "</error>");
  if (error != nullptr) {
    mwin->insLog("autoExtend : error");
    mwin->insLog("code");
    mwin->insLog(error->midStr("<code>", "</code>"));
    mwin->insLog("description");
    mwin->insLog(error->midStr("<description>", "</description>"));

    QMessageBox::information(mwin, "Viqo", QStringLiteral("自動延長失敗しました"));
  }

  reply->deleteLater();

  this->deleteLater();
}

