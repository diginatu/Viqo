#ifndef NICOLIVEMANAGER_H
#define NICOLIVEMANAGER_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "../strabstractor.h"
#include "commtcp.h"
#include "Alert/wakutcp.h"
#include "Alert/livewaku.h"

// temp
#include "../livedata.h"

class MainWindow;

class NicoLiveManager : public QObject
{
	Q_OBJECT
public:
	explicit NicoLiveManager(MainWindow* mwin, QObject *parent = 0);

	QString getWatchCount() const;

	void insertLiveWakuList(LiveWaku* livewaku);

	void getHeartBeatAPI(QString user_id, QString broad_id);
	void getPlayyerStatusAPI(QString session_id, QString broad_id);
	void loginAlertAPI(QString mail, QString pass);
	void adminAlertAPI(QString ticket);
	void getRawMyLiveHTML(QString session_id);

//	QList<LiveWaku> myliveWakus;

	QStringList mycommunities;

	LiveWaku nowWaku;

	QList<LiveWaku*> liveWakuList;

	void broadDisconnect();
signals:

public slots:

private:
	MainWindow* mwin;
	CommTcp* commtcp;
	WakuTcp* wakutcp;

	QString addr;
	QString thread;
	int port;
	QString watchCount;

	QString waku_addr;
	QString waku_thread;
	int waku_port;

	QVariant makePostData(QString session_id);

private slots:
	void playerStatusFinished(QNetworkReply* reply);
	void heartBeatFinished(QNetworkReply* reply);
	void loginAlertFinished(QNetworkReply* reply);
	void adminAlertFinished(QNetworkReply* reply);
	void rawMyLivefinished(QNetworkReply* reply);

};


#endif // NICOLIVEMANAGER_H
