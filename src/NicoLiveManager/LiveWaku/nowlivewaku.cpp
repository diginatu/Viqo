#include "nowlivewaku.h"
#include "../../mainwindow.h"

NowLiveWaku::NowLiveWaku(MainWindow* mwin, QObject* parent) :
	LiveWaku(mwin, parent),
	commtcp(NULL)
{
}

NowLiveWaku::NowLiveWaku(MainWindow* mwin, QString broadID, QObject* parent) :
	LiveWaku(mwin, broadID, parent),
	commtcp(NULL)
{
}

NowLiveWaku::NowLiveWaku(MainWindow* mwin, QString broadID, QString community, QString title, QObject* parent) :
	LiveWaku(mwin, broadID, community, title, parent),
	commtcp(NULL)
{
}

void NowLiveWaku::broadDisconnect() {
	if (commtcp != NULL && commtcp->isConnected() ) {
		commtcp->close();
		commtcp->deleteLater();
		commtcp = NULL;
	}
}

void NowLiveWaku::getPlayyerStatusAPI(QString session_id)
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

void NowLiveWaku::playerStatusFinished(QNetworkReply* reply)
{
	QByteArray repdata = reply->readAll();
	qDebug() << repdata;

	StrAbstractor commTcpi(repdata);

	QString status = commTcpi.midStr("status=\"","\"");
	if (status == "fail") {
		QString code = commTcpi.midStr("<code>","</code>");
		mwin->insLog(code);
		addr = ""; port = 0; thread = "";
		return;
	}

	setBroadID(commTcpi.midStr("<id>lv", "</id>"));

	setTitle(commTcpi.midStr("<title>", "</title>"));

	setCommunity(commTcpi.midStr("<default_community>", "</default_community>"));

	setSt(commTcpi.midStr("<start_time>","</start_time>").toUInt());
	setEd(commTcpi.midStr("<end_time>","</end_time>").toUInt());

	addr = commTcpi.midStr("<addr>", "</addr>");
	port = commTcpi.midStr("<port>", "</port>").toInt();
	thread = commTcpi.midStr("<thread>", "</thread>");

	mwin->insLog("addr: "+addr+"\nport: "+QString::number(port)+"\nthread:"+thread);

	try {
		commtcp = new CommTcp(addr, port, thread, mwin);
		commtcp->doConnect();
	} catch(QString e) {
		qDebug() << e;
	}

}
