#ifndef NICOLIVEMANAGER_H
#define NICOLIVEMANAGER_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "../strabstractor.h"
#include "../commtcp.h"
#include "Alert/wakutcp.h"

class MainWindow;

class NicoLiveManager : public QObject
{
	Q_OBJECT
public:
	explicit NicoLiveManager(MainWindow* mwin, CommTcp** comtcp, QObject *parent = 0);

	QString getAddr() const;
	QString getThread() const;
	int getPort() const;
	QString getWatchCount() const;
	QString getCommunity() const;

	void getHeartBeatAPI(QString user_id, QString broad_id);
	void getPlayyerStatusAPI(QString session_id, QString broad_id);
	void loginAlertAPI(QString mail, QString pass);
	void adminAlertAPI(QString ticket);

	QStringList getMycommunityes() const;

	QStringList getMylivecommunityes() const;

signals:

public slots:

private:
	MainWindow* mwin;
	CommTcp** commtcp;
	WakuTcp* wakutcp;

	QString community;

	QString addr;
	QString thread;
	int port;
	QString watchCount;

	QString waku_addr;
	QString waku_thread;
	int waku_port;

	QStringList mycommunityes;
	QStringList mylivecommunityes;

	QVariant makePostData(QString session_id);

private slots:
	void playerStatusFinished(QNetworkReply* reply);
	void heartBeatFinished(QNetworkReply* reply);
	void loginAlertFinished(QNetworkReply* reply);
	void adminAlertFinished(QNetworkReply* reply);

};


#endif // NICOLIVEMANAGER_H
