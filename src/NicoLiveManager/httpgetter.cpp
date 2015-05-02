#include "httpgetter.h"
#include "../../ui/mainwindow.h"

HttpGetter::HttpGetter(MainWindow* mwin, NicoLiveManager* nlman, QObject *parent) :
  QObject(parent),
  mManager(nullptr)
{
  this->mwin = mwin;
  this->nlman = nlman;
}

void HttpGetter::get()
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

HttpGetter::~HttpGetter()
{
  delete mManager;
}

void HttpGetter::got(QNetworkReply* reply)
{
  StrAbstractor data(QString(reply->readAll()));

  qDebug() << data.toString();

  reply->deleteLater();
}
