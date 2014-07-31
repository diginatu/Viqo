#include "../nicolivemanager.h"
#include "../../mainwindow.h"

void NicoLiveManager::loginAlertAPI(QString mail, QString pass)
{
	QNetworkAccessManager* mManager = new QNetworkAccessManager(this);

	connect(mManager, SIGNAL(finished(QNetworkReply*)),
					this, SLOT(loginAlertFinished(QNetworkReply*)));

	QByteArray data;
	data.append("mail=" + mail + "&password=" + pass);

	QNetworkRequest rq(QUrl("https://secure.nicovideo.jp/secure/login?site=nicolive_antenna"));
	rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	mManager->post(rq, data);
}

void NicoLiveManager::loginAlertFinished(QNetworkReply* reply)
{
	QByteArray repdata = reply->readAll();

	StrAbstractor commTcpi(repdata);

	QString status = commTcpi.midStr("status=\"","\"");
	if (status == "fail") {
		QString code = commTcpi.midStr("<code>","</code>");
		mwin->insLog(code);
		return;
	}

	QString ticket = commTcpi.midStr("<ticket>","</ticket>");

//	mwin->insLog("got ticket: "+ticket);

	adminAlertAPI(ticket);
}


void NicoLiveManager::adminAlertAPI(QString ticket)
{
	QNetworkAccessManager* mManager = new QNetworkAccessManager(this);

	connect(mManager, SIGNAL(finished(QNetworkReply*)),
					this, SLOT(adminAlertFinished(QNetworkReply*)));

	QByteArray data;
	data.append("ticket=" + ticket);

	QNetworkRequest rq(QUrl("http://live.nicovideo.jp/api/getalertstatus"));
	rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	mManager->post(rq, data);
}

void NicoLiveManager::adminAlertFinished(QNetworkReply* reply)
{
	QByteArray repdata = reply->readAll();

	StrAbstractor wakuTcpi(repdata);

	QString status = wakuTcpi.midStr("status=\"","\"");
	if (status == "fail") {
		QString code = wakuTcpi.midStr("<code>","</code>");
		mwin->insLog(code);
		return;
	}

	QByteArray mycommunities;
	mycommunities.append(wakuTcpi.midStr("<communities>","</communities>"));

	StrAbstractor communityi(mycommunities);
	QString mycommunity;
	while ((mycommunity = communityi.midStr("<community_id>","</community_id>")) != "") {
		this->mycommunities << mycommunity;
	}

//	qDebug() << this->mycommunities;

	waku_addr = wakuTcpi.midStr("<addr>", "</addr>");
	waku_port = wakuTcpi.midStr("<port>", "</port>").toInt();
	waku_thread = wakuTcpi.midStr("<thread>", "</thread>");

	mwin->insLog("waku addr: "+waku_addr+"\nport: "+QString::number(waku_port)+"\nthread:"+waku_thread);

	try {
		if ( wakutcp != NULL ) wakutcp->deleteLater();

		wakutcp = new WakuTcp(waku_addr, waku_port, waku_thread, mwin, this);
		wakutcp->doConnect();
	} catch(QString e) {
		qDebug() << e;
	}

}
