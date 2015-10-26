#include "usermanager.h"
#include "../ui/mainwindow.h"

UserManager::UserManager(MainWindow* mwin, QObject *parent) :
  QObject(parent)
{
  this->mwin = mwin;

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
    QMessageBox::information(mwin, "Viqo",
                             QStringLiteral("ユーザのデータベースオープンに失敗しました"));
  }

  QSqlQuery query(db);

  query.prepare("create table if not exists user "
                "(id integer unique primary key, name varchar(60))");

  if (!query.exec()) {
    mwin->insLog("UserManager::UserManager create table error");
    QMessageBox::information(mwin, "Viqo",
                             QStringLiteral("ユーザのデータベーステーブル作成に失敗しました"));
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
    query.prepare("select distinct name from user where id=:id");
    query.bindValue(":id", userID);

    if (query.exec()) {
      if (query.next()) {
        item->setText(2, query.value(0).toString());
      } else if (useHTTP) {
        nicolive::FetchUserName* ug = new nicolive::FetchUserName(this, userID);
        connect(ug, &nicolive::FetchUserName::error, this, [=](){
          QMessageBox::information(mwin, "Viqo", QStringLiteral("ユーザ取得エラー"));
        });

        connect(ug, &nicolive::FetchUserName::got, this, [=](QString n){
          mwin->userManager->setUserName(item,n);
        });

        ug->get();
      }
    } else {
      mwin->insLog("UserManager::getUserName user db get error\n");
      QMessageBox::information(mwin, "Viqo",
                               QStringLiteral("ユーザのデータベーステーブル取得に失敗しました"));
    }
  } else if (useHTTP) {
    nicolive::FetchUserName* ug = new nicolive::FetchUserName(this, userID);
    connect(ug, &nicolive::FetchUserName::error, this, [=](){
      QMessageBox::information(mwin, "Viqo", QStringLiteral("ユーザ取得エラー"));
    });
    connect(ug, &nicolive::FetchUserName::got, this, [=](QString n){
      mwin->userManager->setUserName(item,n);
    });
    ug->get();
  }
}

void UserManager::setUserName(QTreeWidgetItem* item, QString username)
{
  item->setText(2, username);

  QSqlQuery query(db);
  query.prepare("insert or replace into user (id, name) values (:id, :name)");
  query.bindValue(":id", item->text(5));
  query.bindValue(":name", username);

  if (!query.exec()) {
    mwin->insLog("user db set error");
    QMessageBox::information(mwin, "Viqo",
                             QStringLiteral("ユーザのデータベーステーブル書き込みに失敗しました"));
  }
}

void UserManager::removeUser(QTreeWidgetItem* item)
{
  QSqlQuery query(db);
  query.prepare("delete from user where id = :id");
  query.bindValue(":id", item->text(5));

  if (!query.exec()) {
    mwin->insLog("user db set error");
    QMessageBox::information(mwin, "Viqo",
                             QStringLiteral("ユーザのデータベーステーブル書き込みに失敗しました"));
  }
}
