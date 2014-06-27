#include "cookieread.h"
#include "mainwindow.h"

CookieRead::CookieRead(MainWindow* mwin)
{
	this->mwin = mwin;
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(mwin->getCookieSetting(0));

	if (db.open()) {
		mwin->insLog("db open succeeded");
	} else {
		throw QString("db open error occured");
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

	QSqlQuery query(db);

	query.prepare(queryS);
	query.bindValue(":baseDomain", "nicovideo.jp");
	query.bindValue(":name", "user_session");

	if (query.exec()) {
		if (query.next()) {
			return query.value(0).toString();
		}
	} else {
		throw QString("usersession get error");
	}

	return "";
}

CookieRead::~CookieRead()
{
	db.close();
}
