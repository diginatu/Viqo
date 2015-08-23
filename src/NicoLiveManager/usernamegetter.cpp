#include "usernamegetter.h"
#include "../../ui/mainwindow.h"

UserNameGetter::UserNameGetter(MainWindow* mwin, QObject *parent, QString userID) :
  HttpGetter(mwin, parent)
{
  this->userID = userID;
}

UserNameGetter::~UserNameGetter()
{
}

void UserNameGetter::get()
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(gotReply(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  rq.setUrl(QUrl("http://api.ce.nicovideo.jp/api/v1/user.info?user_id=" + userID));

  mManager->get(rq);
}

void UserNameGetter::gotReply(QNetworkReply* reply)
{
  StrAbstractor userinfo(QString(reply->readAll()));

  QString username = userinfo.midStr("<nickname>", "</nickname>");

  emit got(username);

  reply->deleteLater();
  this->deleteLater();
}
