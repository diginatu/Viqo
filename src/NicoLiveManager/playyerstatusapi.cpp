#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getPlayyerStatusAPI(QString session_id, QString broad_id)
{
	QNetworkAccessManager* mManager = new QNetworkAccessManager(this);
	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData(session_id);
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://live.nicovideo.jp/api/getplayerstatus?v=" + broad_id));

	connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
					SLOT(playerStatusFinished(QNetworkReply*)));
	mManager->get(rq);
}

void NicoLiveManager::playerStatusFinished(QNetworkReply* reply)
{
	QByteArray repdata = reply->readAll();

	StrAbstractor commTcpi(repdata);

	QString status = commTcpi.midStr("status=\"","\"");
	if (status == "fail") {
		QString code = commTcpi.midStr("<code>","</code>");
		mwin->insLog(code);
		addr = ""; port = 0; thread = "";
		return;
	}

	community = commTcpi.midStr("<default_community>", "</default_community>");

	addr = commTcpi.midStr("<addr>", "</addr>");
	port = commTcpi.midStr("<port>", "</port>").toInt();
	thread = commTcpi.midStr("<thread>", "</thread>");

	mwin->insLog("addr: "+addr+"\nport: "+QString::number(port)+"\nthread:"+thread);

	try {
		*commtcp = new CommTcp(addr, port, thread, mwin);
		(*commtcp)->doConnect();
	} catch(QString e) {
		qDebug() << e;
	}

}

QString NicoLiveManager::getAddr() const {
	return this->addr;
}

QString NicoLiveManager::getThread() const {
	return this->thread;
}

int NicoLiveManager::getPort() const {
	return this->port;
}
