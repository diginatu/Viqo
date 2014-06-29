#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getHeartBeatAPI(QString session_id, QString broad_id)
{
	mManager = new QNetworkAccessManager(this);
	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData(session_id);
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://live.nicovideo.jp/api/heartbeat?v=" + broad_id));

	heartBeatReply = mManager->get(rq);
	connect(heartBeatReply,SIGNAL(finished()),this,SLOT(heartBeatFinished()));
}

void NicoLiveManager::heartBeatFinished(){
	QByteArray repdata = heartBeatReply->readAll();
	StrAbstractor heartbeat_data(repdata);
	mManager->deleteLater();

	const QString status = heartbeat_data.midStr("status=\"", "\"");
	if ( status == "fail" ) return;

	watchCount = heartbeat_data.midStr("<watchCount>","</watchCount>");

	mwin->setWatchCount();
}

QString NicoLiveManager::getWatchCount() {
	return watchCount;
}
