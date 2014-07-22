#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getRawMyLiveHTML(QString session_id)
{
	QNetworkAccessManager* mManager = new QNetworkAccessManager(this);
	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData(session_id);
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://www.nicovideo.jp/my/live"));

	connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
					SLOT(rawMyLivefinished(QNetworkReply*)));
	mManager->get(rq);
}

void NicoLiveManager::rawMyLivefinished(QNetworkReply* reply)
{
	QByteArray repdata = reply->readAll();

//	while (!liveWakuList.isEmpty())
//		delete liveWakuList.takeFirst();

	//cap(1)はコミュID、cap(2)は配信ID,cap(3)はタイトル
	QRegExp rx("<a href=\"http://com.nicovideo.jp/community/(co\\d+)\">.*<h5><a href=\"http://live.nicovideo.jp/watch/lv(\\d+)\\?ref=zero_mysubscribe\">(.*)</a></h5>");
	rx.setMinimal(true);
	int currentIndex=0;

	while ((currentIndex=rx.indexIn(QString(repdata),currentIndex))!=-1){
		// 見つけた文字列分だけずらす
		currentIndex+=rx.cap(0).length();

		insertLiveWakuList(new LiveWaku(mwin, rx.cap(2),rx.cap(1),rx.cap(3)));
	}

//	qDebug() << broadIDList;

	mwin->refleshLiveWaku();

}
