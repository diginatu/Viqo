#include "cookieread.h"
#include "mainwindow.h"

CookieRead::CookieRead(MainWindow* mwin)
{
	this->mwin = mwin;
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName( QDir::homePath() + QDir::separator() + "testcookie" + QDir::separator() + "cookies.sqlite");

	if (db.open()) {
		qDebug() << "db open succeeded";
	} else {
		qDebug() << "db open error occured";
	}
}

QString CookieRead::getUserSession()
{
	QString queryS = "select ";
	queryS.append(mwin->getCookieSetting(4));
	queryS.append(" from ").append(mwin->getCookieSetting(1));
	queryS.append(" where ").append(mwin->getCookieSetting(2));
	queryS.append("=:baseDomain and ").append(mwin->getCookieSetting(3));
	queryS.append("=:name");

	qDebug() << queryS;

	QSqlQuery query(db);

	query.prepare(queryS);
	query.bindValue(":baseDomain", "nicovideo.jp");
	query.bindValue(":name", "user_session");

	if (query.exec()) {
		if (query.next()) {
			QString value = query.value(0).toString();
			return value;
		}
	} else {
		qDebug() << "usersession get error" << query.lastError();
	}

	return "";
}

CookieRead::~CookieRead()
{
	db.close();
}
