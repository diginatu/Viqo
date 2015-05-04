#include "usernamegetter.h"
#include "../../ui/mainwindow.h"

UserNameGetter::UserNameGetter(MainWindow* mwin, QObject *parent,
                               QTreeWidgetItem* item, QString userID,
                               QSqlDatabase& db) :
  HttpGetter(mwin, parent)
{
  this->db = &db;
  this->item = item;
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
          SLOT(got(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  rq.setUrl(QUrl("http://api.ce.nicovideo.jp/api/v1/user.info?user_id=" + userID));

  mManager->get(rq);
}

void UserNameGetter::got(QNetworkReply* reply)
{
  StrAbstractor userinfo(QString(reply->readAll()));

  QString username = userinfo.midStr("<nickname>", "</nickname>");

  mwin->userManager->setUserName(item, username);

  reply->deleteLater();
  this->deleteLater();
}
