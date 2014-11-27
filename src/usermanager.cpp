#include "usermanager.h"
#include "mainwindow.h"

UserManager::UserManager(MainWindow* mwin, QObject *parent) :
	NicoHttp(mwin, parent)
{
	db = QSqlDatabase::addDatabase("QSQLITE");

  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    throw QString("save directory is not found");
  }
  db.setDatabaseName(dir[0] + "/user.sqlite");

	if (db.open()) {
    mwin->insLog("UserManager::UserManager db open succeeded\n");
	} else {
    mwin->insLog("UserManager::UserManager db open error occured\n");
	}

	QSqlQuery query(db);

	query.prepare("create table if not exists user (id integer unique primary key, name varchar(60))");

	if (!query.exec()) {
		throw QString("user db get error");
	}

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
			throw QString("user db get error");
		}
	} else if (useHTTP) {
		UserGetter* ug = new UserGetter(mwin,this);
		ug->getUserName(item, userID, db);
	}

}

