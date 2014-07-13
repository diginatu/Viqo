#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getHeartBeatAPI(QString session_id, QString broad_id)
{
	QNetworkAccessManager* mManager = new QNetworkAccessManager(this);
	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData(session_id);
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://live.nicovideo.jp/api/heartbeat?v=" + broad_id));

	connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
					SLOT(heartBeatFinished(QNetworkReply*)));
	mManager->get(rq);
}

void NicoLiveManager::heartBeatFinished(QNetworkReply* reply){
	QByteArray repdata = reply->readAll();
	StrAbstractor heartbeat_data(repdata);

	const QString status = heartbeat_data.midStr("status=\"", "\"");
	if ( status == "fail" ) return;

	watchCount = heartbeat_data.midStr("<watchCount>","</watchCount>");

	mwin->setWatchCount();
}

QString NicoLiveManager::getWatchCount() const {
	return watchCount;
}
