#ifndef NICOLIVEMANAGER_H
#define NICOLIVEMANAGER_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "../strabstractor.h"
#include "../commtcp.h"

class MainWindow;

class NicoLiveManager : public QObject
{
	Q_OBJECT
public:
	explicit NicoLiveManager(MainWindow* mwin, CommTcp** comtcp, QObject *parent = 0);

	QString getAddr();
	QString getThread();
	int getPort();
	QString getWatchCount();

	void getHeartBeatAPI(QString user_id, QString broad_id);
	void getPlayyerStatusAPI(QString session_id, QString broad_id);


signals:

public slots:

private:
	MainWindow* mwin;
	CommTcp** commtcp;

	QString addr;
	QString thread;
	int port;
	QString watchCount;

	QNetworkAccessManager* mManager;
	QNetworkReply* playyerStatusReply;
	QNetworkReply* heartBeatReply;

	QVariant makePostData(QString session_id);

private slots:
	void playerStatusFinished();
	void heartBeatFinished();

};


#endif // NICOLIVEMANAGER_H
