#include "usermanager.h"
#include "../ui/mainwindow.h"

UserManager::UserManager(MainWindow* mwin, QObject *parent) :
  NicoHttp(mwin, parent)
{
  mwin->insLog("UserManager::UserManager");
  db = QSqlDatabase::addDatabase("QSQLITE");

  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not found");
    return;
  }
  db.setDatabaseName(dir[0] + "/user.sqlite");

  if (db.open()) {
    mwin->insLog("db open succeeded");
  } else {
    mwin->insLog("db open error occured");
    QMessageBox::information(mwin, "Viqo", "ユーザのデータベースオープンに失敗しました");
  }

  QSqlQuery query(db);

  query.prepare("create table if not exists user (id integer unique primary key, name varchar(60))");

  if (!query.exec()) {
    mwin->insLog("UserManager::UserManager create table error");
  }

  mwin->insLog();
}

void UserManager::getUserName(QTreeWidgetItem* item, QString userID, bool useHTTP, bool useDB)
{
  // return if empty
  if (userID.isEmpty()) return;
  // return if 184 comment.
  if (item->text(6) == "@") return;

  if (useDB) {
    QSqlQuery query(db);
    query.prepare("select distinct name from user where id=" + userID);

    if (query.exec()) {
      if (query.next()) {
        item->setText(2, query.value(0).toString());
      } else if (useHTTP) {
        UserGetter* ug = new UserGetter(mwin,this);
        ug->getUserName(item, userID, db);
      }
    } else {
      mwin->insLog("UserManager::getUserName user db get error\n");
    }
  } else if (useHTTP) {
    UserGetter* ug = new UserGetter(mwin,this);
    ug->getUserName(item, userID, db);
  }

}

