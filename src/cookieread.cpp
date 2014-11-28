#include "cookieread.h"
#include "mainwindow.h"

CookieRead::CookieRead(MainWindow* mwin)
{
  this->mwin = mwin;
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(mwin->getCookieName());

  if (db.open()) {
    mwin->insLog("CookieRead::CookieRead db open succeeded\n");
  } else {
    mwin->insLog("CookieRead::CookieRead db open error occured\n");
  }
}

QString CookieRead::getUserSession()
{
  mwin->insLog("CookieRead::getUserSession");

  QString queryS = "select value from moz_cookies where baseDomain=:baseDomain and name=:name";

  QSqlQuery query(db);

  query.prepare(queryS);
  query.bindValue(":baseDomain", "nicovideo.jp");
  query.bindValue(":name", "user_session");

  if (query.exec()) {
    if (query.next()) {
      return query.value(0).toString();
    }
  } else {
    mwin->insLog("Cookie db usersession getting error");
  }

  mwin->insLog();
  return "";
}

CookieRead::~CookieRead()
{
  qDebug() << "cookie db closed";
  db.close();
}
