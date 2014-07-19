#include "nicolivemanager.h"
#include "../mainwindow.h"

NicoLiveManager::NicoLiveManager(MainWindow* mwin, QObject *parent) :
	QObject(parent),
	commtcp(NULL),
	wakutcp(NULL)
{
	this->mwin = mwin;
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

void NicoLiveManager::insertLiveWakuList(LiveWaku* livewaku)
{
	const QString livewakuBID = livewaku->getBroadID();
	foreach (LiveWaku* alistwaku, liveWakuList) {
		if (livewakuBID == alistwaku->getBroadID())
			return;
	}

	liveWakuList << livewaku;
}

void NicoLiveManager::broadDisconnect(){
	if (commtcp != NULL && commtcp->isConnected() ) {
		commtcp->close();
		commtcp->deleteLater();
		commtcp = NULL;
	}
}

