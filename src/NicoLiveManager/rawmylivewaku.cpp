#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getRawMyLiveHTML()
{
	QNetworkAccessManager* mManager = new QNetworkAccessManager(this);
	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData(mwin->getUserSession());
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://www.nicovideo.jp/my/live"));

	connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
					SLOT(rawMyLivefinished(QNetworkReply*)));
	mManager->get(rq);
}

void NicoLiveManager::rawMyLivefinished(QNetworkReply* reply)
{
	QByteArray repdata = reply->readAll();
//	qDebug() << repdata;

//	while (!liveWakuList.isEmpty())
//		delete liveWakuList.takeFirst();

	StrAbstractor liveID(repdata);

	QString ID;
	while((ID = liveID.midStr("<a href=\"http://live.nicovideo.jp/watch/lv",
														"?ref=zero_mysubscribe\">")) != "") {
		insertLiveWakuList(new LiveWaku(mwin, ID));
	}

}
