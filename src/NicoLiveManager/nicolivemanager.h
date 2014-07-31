#ifndef NICOLIVEMANAGER_H
#define NICOLIVEMANAGER_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "../strabstractor.h"
#include "Alert/wakutcp.h"
#include "LiveWaku/livewaku.h"
#include "LiveWaku/nowlivewaku.h"


class MainWindow;

class NicoLiveManager : public QObject
{
	Q_OBJECT
public:
	explicit NicoLiveManager(MainWindow* mwin, QObject *parent = 0);

	QString getWatchCount() const;

	static QVariant makePostData(QString session_id);

	void insertLiveWakuList(LiveWaku* livewaku);

	void getHeartBeatAPI(QString broad_id);
	void loginAlertAPI(QString mail, QString pass);
	void adminAlertAPI(QString ticket);
	void getRawMyLiveHTML();

	QStringList mycommunities;

	NowLiveWaku nowWaku;

	QList<LiveWaku*> liveWakuList;

	void broadDisconnect();
	void broadStart();
signals:

public slots:

private:
	MainWindow* mwin;
	WakuTcp* wakutcp;

	QString watchCount;

	QString waku_addr;
	QString waku_thread;
	int waku_port;


private slots:
	void heartBeatFinished(QNetworkReply* reply);
	void loginAlertFinished(QNetworkReply* reply);
	void adminAlertFinished(QNetworkReply* reply);
	void rawMyLivefinished(QNetworkReply* reply);

};


#endif // NICOLIVEMANAGER_H
