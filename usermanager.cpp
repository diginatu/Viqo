#include "usermanager.h"
#include "mainwindow.h"

UserManager::UserManager(MainWindow* mwin) :
	QObject()
{
	this->mwin = mwin;
}

QVariant UserManager::makePostData(){
	QVariant postData;

	// make cookies
	QList <QNetworkCookie> cookies;
	QNetworkCookie ck;
	ck.toRawForm(QNetworkCookie::NameAndValueOnly);
	ck.setName("user_session");

	QByteArray user_id_ba;
	user_id_ba.append(mwin->getUserSession());

	ck.setValue(user_id_ba);
	cookies.append(ck);

	postData.setValue(cookies);
	return postData;
}

void UserManager::getUserName(QTreeWidgetItem* item, QString userID)
{
	mManager = new QNetworkAccessManager(this);
	this->item = item;

	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData();
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl(QString("http://www.nicovideo.jp/user/").append(userID)));

	reply = mManager->get(rq);
	connect(reply,SIGNAL(finished()),this,SLOT(finished()));
}

void UserManager::finished(){
	QByteArray repdata = reply->readAll();
	qDebug() << "2";
	qDebug() << repdata;

	int st = repdata.indexOf("<title>") + 7;
	int ed = repdata.indexOf("さんのユーザーページ", st);

//	item->setText(2, QString(repdata.mid(st, ed-st)));
	qDebug() << QString(repdata.mid(st, ed-st));
}
