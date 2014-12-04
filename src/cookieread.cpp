#include "cookieread.h"
#include "mainwindow.h"

CookieRead::CookieRead(MainWindow* mwin)
{
  this->mwin = mwin;
}

QString CookieRead::getUserSession()
{
  mwin->insLog("CookieRead::getUserSession");

  QString cookie_name = mwin->settings.getCookieFile();
  if (cookie_name == "") {
    mwin->insLog("Cookie Name is not specified");
    return "";
  }
  db = QSqlDatabase::addDatabase("QSQLITE", "firefox_cookie_connection");
  db.setDatabaseName(cookie_name);

  if (db.open()) {
    mwin->insLog("db open succeeded\n");
  } else {
    mwin->insLog("db open error occured\n");
  }

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
    mwin->insLog("query error");
  }

  mwin->insLog();
  return "";
}

CookieRead::~CookieRead()
{
  mwin->insLog("CookieRead::~CookieRead db closed\n");
  db.close();
}
