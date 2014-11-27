#include "livewaku.h"
#include "../../mainwindow.h"
#include "../nicolivemanager.h"

LiveWaku::LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent) :
	QObject(parent)
{
	this->mwin = mwin;
	this->nlman = nlman;
}

LiveWaku::LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QObject *parent) :
	QObject(parent)
{
	this->mwin = mwin;
	this->nlman = nlman;
	this->broadID = broadID;
}

QString LiveWaku::getTitle() const
{
	return title;
}

void LiveWaku::setTitle(const QString& value)
{
	title = value;
}

QString LiveWaku::getBroadID() const
{
	return broadID;
}

void LiveWaku::setBroadID(const QString& value)
{
	broadID = value;
}

QString LiveWaku::getCommunity() const
{
	return community;
}

void LiveWaku::setCommunity(const QString& value)
{
	community = value;
}

QDateTime LiveWaku::getSt() const
{
	return st;
}

void LiveWaku::setSt(uint unixt)
{
	st.setTime_t(unixt);
}

QDateTime LiveWaku::getEd() const
{
	return ed;
}

void LiveWaku::setEd(uint unixt)
{
	ed.setTime_t(unixt);
}


void LiveWaku::getPlayyerStatusAPI()
{
	QNetworkAccessManager* mManager = new QNetworkAccessManager(this);
	// make request
	QNetworkRequest rq;
	QVariant postData = NicoLiveManager::makePostData(mwin->getUserSession());
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://live.nicovideo.jp/api/getplayerstatus?v=lv" + broadID));
	connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
					SLOT(playerStatusFinished(QNetworkReply*)));
	mManager->get(rq);
}

void LiveWaku::playerStatusFinished(QNetworkReply* reply)
{
  mwin->insLog(" LiveWaku::playerStatusFinished :");
  QByteArray repdata = reply->readAll();

	StrAbstractor commTcpi(repdata);

	QString status = commTcpi.midStr("status=\"","\"");
	if (status == "fail") {
		QString code = commTcpi.midStr("<code>","</code>");
    mwin->insLog(code);

		if (code == "closed" || code == "deletedbyuser") {
			QList<LiveWaku*>& wlist = nlman->liveWakuList;

			const int this_index = wlist.indexOf(this);

			if (this_index != -1) {
				wlist.removeAt(this_index);
				mwin->refleshLiveWaku();
        this->deleteLater();
			}
		}
		return;
	}

	setBroadID(commTcpi.midStr("<id>lv", "</id>"));
	setTitle(commTcpi.midStr("<title>", "</title>"));
	setCommunity(commTcpi.midStr("<default_community>", "</default_community>"));
	setSt(commTcpi.midStr("<start_time>","</start_time>").toUInt());
	setEd(commTcpi.midStr("<end_time>","</end_time>").toUInt());

	mwin->refleshLiveWaku();
  mwin->insLog("got a broad info : " + title + "\n");

	return;
}

