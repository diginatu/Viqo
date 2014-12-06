#ifndef NICOLIVEMANAGER_H
#define NICOLIVEMANAGER_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>

#include "../strabstractor.h"
#include "Alert/wakutcp.h"
#include "LiveWaku/livewaku.h"
#include "LiveWaku/nowlivewaku.h"
#include "../settingswindow.h"

namespace Ui {
class MainWindow;
}

class NicoLiveManager : public QObject
{
	Q_OBJECT
public:
  explicit NicoLiveManager(MainWindow* mwin, SettingsWindow* swin, QObject *parent = 0);
	~NicoLiveManager();

	static QVariant makePostData(QString session_id);

	void insertLiveWakuList(LiveWaku* livewaku);

	void getHeartBeatAPI();
  void loginAlertAPI(const QString& mail, const QString& pass);
  void adminAlertAPI(const QString& ticket);
  void getPostKeyAPI(const QString& thread, int block_no);

  void alertReconnect();

  void login(QString mail, QString pass);

	QStringList mycommunities;

	NowLiveWaku nowWaku;

	QList<LiveWaku*> liveWakuList;

	void broadDisconnect();
	void broadStart();

signals:

public slots:
  void getRawMyLiveHTML();

private:
	MainWindow* mwin;
  SettingsWindow* swin;
  WakuTcp* wakutcp;

	QString watchCount;

	QString waku_addr;
	QString waku_thread;
	int waku_port;

	QTimer* delWakuTimer;

private slots:
	void heartBeatFinished(QNetworkReply* reply);
	void loginAlertFinished(QNetworkReply* reply);
	void adminAlertFinished(QNetworkReply* reply);
	void rawMyLivefinished(QNetworkReply* reply);
  void loginFinished(QNetworkReply* reply);
  void postKeyFinished(QNetworkReply* reply);

  void deleteWakuList();
};


#endif // NICOLIVEMANAGER_H
