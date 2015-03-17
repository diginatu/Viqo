#include "cookieread.h"
#include "../ui/mainwindow.h"

CookieRead::CookieRead(MainWindow* mwin)
{
  this->mwin = mwin;
}

QString CookieRead::getUserSession(QString cookie_name)
{
  mwin->insLog("CookieRead::getUserSession");

  if (cookie_name == "") {
    mwin->insLog("Cookie File Name is not specified");
    QMessageBox::information(mwin, "Viqo", "クッキーのファイル名が指定されていません");
    return "";
  }
  db = QSqlDatabase::addDatabase("QSQLITE", "firefox_cookie_connection");
  db.setDatabaseName(cookie_name);

  if (db.open()) {
    mwin->insLog("db open succeeded\n");
  } else {
    mwin->insLog("db open error occured\n");
    QMessageBox::information(mwin, "Viqo", "クッキーファイルのオープンに失敗しました");
  }

  QString queryS = "select value from moz_cookies where baseDomain=:baseDomain and name=:name";

  QSqlQuery query(db);

  query.prepare(queryS);
  query.bindValue(":baseDomain", "nicovideo.jp");
  query.bindValue(":name", "user_session");

  if (query.exec()) {
    if (query.next()) {
      QMessageBox::information(mwin, "Viqo", "クッキーファイルから user_session を取得しました");
      return query.value(0).toString();
    }
  } else {
    mwin->insLog("query error");
    QMessageBox::information(mwin, "Viqo", "クッキーファイル読み込みに失敗しました");
  }

  mwin->insLog();
  return "";
}

CookieRead::~CookieRead()
{
  mwin->insLog("CookieRead::~CookieRead db closed\n");
  db.close();
}
