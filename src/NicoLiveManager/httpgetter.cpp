#include "httpgetter.h"

httpGetter::httpGetter(MainWindow* mwin, NicoLiveManager* nlman, QObject *parent) : QObject(parent)
{
  this->nlman = nlman;
}

void httpGetter::get()
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(got(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = nlman->makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getpublishstatus?v=lv" + nlman->nowWaku.getBroadID()));

  mManager->get(rq);
}

httpGetter::~httpGetter()
{
  delete mManager;
}

void httpGetter::got(QNetworkReply* reply)
{
  StrAbstractor data(QString(reply->readAll()));

  qDebug() << data.toString();

  reply->deleteLater();
}

