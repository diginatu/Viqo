#include "../nicolivemanager.h"
#include "../../mainwindow.h"

void LiveWaku::getPlayyerStatusAPI(QString session_id)
{
	QNetworkAccessManager* mManager = new QNetworkAccessManager(this);
	// make request
	QNetworkRequest rq;
	QVariant postData = NicoLiveManager::makePostData(session_id);
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://live.nicovideo.jp/api/getplayerstatus?v=" + broadID));

	connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
					SLOT(playerStatusFinished(QNetworkReply*)));
	mManager->get(rq);
}

void LiveWaku::playerStatusFinished(QNetworkReply* reply)
{
	QByteArray repdata = reply->readAll();

	StrAbstractor commTcpi(repdata);

	QString status = commTcpi.midStr("status=\"","\"");
	if (status == "fail") {
		QString code = commTcpi.midStr("<code>","</code>");
		mwin->insLog(code);
		return;
	}

	setBroadID(commTcpi.midStr("<id>lv", "</id>"));

	setTitle(commTcpi.midStr("<title>", "</title>"));

	setCommunity(commTcpi.midStr("<default_community>", "</default_community>"));

	setSt(commTcpi.midStr("<start_time>","</start_time>").toUInt());
	setEd(commTcpi.midStr("<end_time>","</end_time>").toUInt());

}
