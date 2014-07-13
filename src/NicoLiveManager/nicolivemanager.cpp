#include "nicolivemanager.h"
#include "../mainwindow.h"

NicoLiveManager::NicoLiveManager(MainWindow* mwin, CommTcp** commtcp, QObject *parent) :
	QObject(parent),
	wakutcp(NULL)
{
	this->mwin = mwin;
	this->commtcp = commtcp;
}

QVariant NicoLiveManager::makePostData(QString session_id)
{
	QVariant postData;

	// make cookies
	QList <QNetworkCookie> cookies;
	QNetworkCookie ck;
	ck.toRawForm(QNetworkCookie::NameAndValueOnly);
	ck.setName("user_session");

	QByteArray user_id_ba;
	user_id_ba.append(session_id);

	ck.setValue(user_id_ba);
	cookies.append(ck);

	postData.setValue(cookies);
	return postData;
}

QString NicoLiveManager::getCommunity() const
{
	return community;
}

QStringList NicoLiveManager::getMycommunityes() const
{
	return mycommunityes;
}

QStringList NicoLiveManager::getMylivecommunityes() const
{
	return mylivecommunityes;
}

