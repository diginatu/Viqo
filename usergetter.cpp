#include "usergetter.h"
#include "mainwindow.h"

UserGetter::UserGetter(MainWindow* mwin, QObject *parent) :
	NicoHttp(mwin, parent)
{
}

void UserGetter::getUserName(QTreeWidgetItem* item, QString userID, QSqlDatabase& db)
{
	this->db = &db;
	this->item = item;
	this->userID = userID;
	getBody(QUrl(QString("http://www.nicovideo.jp/user/").append(userID)));
}

void UserGetter::finished()
{
	QByteArray repdata = reply->readAll();
	StrAbstractor userinfo(repdata);

	QString username = userinfo.midStr("<title>", "さんのユーザーページ");

	item->setText(2, username);


	QSqlQuery query(*db);
	query.prepare("insert or replace into user (id, name)\
								values (" + userID + ", '" + username + "')");

	if (!query.exec()) {
		throw QString("user db get error");
	}
}
