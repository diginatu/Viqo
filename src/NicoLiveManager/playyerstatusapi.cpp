#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getPlayyerStatusAPI(QString session_id, QString broad_id)
{
	mManager = new QNetworkAccessManager(this);
	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData(session_id);
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://live.nicovideo.jp/api/getplayerstatus?v=" + broad_id));

	playyerStatusReply = mManager->get(rq);
	connect(playyerStatusReply,SIGNAL(finished()),this,SLOT(playerStatusFinished()));
}

void NicoLiveManager::playerStatusFinished()
{
	QByteArray repdata = playyerStatusReply->readAll();
	mManager->deleteLater();

	StrAbstractor commTcpi(repdata);

	QString status = commTcpi.midStr("status=\"","\"");
	if (status == "fail") {
		QString code = commTcpi.midStr("<code>","</code>");
		mwin->insLog(code);
		addr = ""; port = 0; thread = "";
		return;
	}

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

QString NicoLiveManager::getAddr() {
	return this->addr;
}

QString NicoLiveManager::getThread() {
	return this->thread;
}

int NicoLiveManager::getPort() {
	return this->port;
}
