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

	StrAbstractor liveID(repdata);

	// seek to Programs from the channels/communities joined if exist
	if (liveID.forwardStr("<div class=\"articleBody \" id=\"ch\">") == -1) {
		return;
	}

	QString bfID;
	QString ID;
	while((ID = liveID.midStr("<a href=\"http://live.nicovideo.jp/watch/lv",
														"?ref=")) != "") {
		if (bfID == ID) continue;
		bool isID = true;

    for(int i = 0; i < ID.size(); ++i) {
			if( ID[i] > '9' || ID[i] < '0' ) {
				isID = false;
				break;
			}
		}

		if ( !isID ) continue;

		insertLiveWakuList(new LiveWaku(mwin, this, ID));
		bfID = ID;
	}

}
