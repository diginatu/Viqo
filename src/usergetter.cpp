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

	userinfo.midStr("class=\"userDetail\"","class=\"avatar\""); //set position
	QString username = userinfo.midStr("alt=\"", "\"");
	mwin->insLog("user name (http) :" + username);

	item->setText(2, username);


	QSqlQuery query(*db);
	query.prepare("insert or replace into user (id, name) values ("
								+ userID + ", '" + username + "')");

	if (!query.exec()) {
    throw QString("user db got error");
	}

	this->deleteLater();
}
