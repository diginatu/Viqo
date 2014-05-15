#include "usermanager.h"
#include "mainwindow.h"

UserManager::UserManager(MainWindow* mwin, QObject *parent) :
	NicoHttp(mwin, parent)
{
}

void UserManager::getUserName(QTreeWidgetItem* item, QString userID)
{
	this->item = item;
	getBody(QUrl(QString("http://www.nicovideo.jp/user/").append(userID)));
}

void UserManager::finished()
{
	QByteArray repdata = reply->readAll();
	StrAbstractor userinfo(repdata);

	QString username = userinfo.midStr("<title>", "さんのユーザーページ");

	item->setText(2, username);
}
