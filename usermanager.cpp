#include "usermanager.h"
#include "mainwindow.h"

UserManager::UserManager(MainWindow* mwin, QObject *parent) :
	NicoHttp(mwin, parent)
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("user.sqlite");

	if (db.open()) {
		mwin->insLog("user name db open succeeded");
	} else {
		throw QString("user name db open error occured");
	}

	QSqlQuery query(db);

	query.prepare("create table if not exists user \
								(id integer unique primary key,\
								 name varchar(60))");

	if (!query.exec()) {
		throw QString("user db get error");
	}
}

void UserManager::getUserName(QTreeWidgetItem* item, QString userID)
{
	UserGetter* ug = new UserGetter(mwin,this);
	ug->getUserName(item, userID, db);

//	ug->deleteLater();
}
