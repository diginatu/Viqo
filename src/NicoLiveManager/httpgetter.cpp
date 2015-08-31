#include "httpgetter.h"
#include "../../ui/mainwindow.h"

HttpGetter::HttpGetter(MainWindow* mwin, QObject *parent) :
  QObject(parent),
  mManager(nullptr)
{
  this->mwin = mwin;
}

void HttpGetter::requestGet(const QNetworkRequest& rq)
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(gotReply(QNetworkReply*)));

  mManager->get(rq);
}

void HttpGetter::requestPost(const QNetworkRequest& rq, QIODevice* data)
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(gotReply(QNetworkReply*)));

  mManager->post(rq, data);
}

HttpGetter::~HttpGetter()
{
  if(mManager!=nullptr) delete mManager;
}

void HttpGetter::gotReply(QNetworkReply* reply)
{
  reply->deleteLater();
  this->deleteLater();
}
