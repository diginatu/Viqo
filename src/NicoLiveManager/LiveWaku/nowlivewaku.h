#ifndef NOWLIVEWAKU_H
#define NOWLIVEWAKU_H

#include <QObject>
#include "../../strabstractor.h"
#include "livewaku.h"
#include "commtcp.h"

class NowLiveWaku : public LiveWaku
{
	Q_OBJECT
public:
	NowLiveWaku(MainWindow* mwin, QObject* parent);
	NowLiveWaku(MainWindow* mwin, QString broadID, QObject* parent);
	NowLiveWaku(MainWindow* mwin, QString broadID, QString community, QString title, QObject* parent);

	void broadDisconnect();
	void getPlayyerStatusAPI(QString session_id);

private slots:
	void playerStatusFinished(QNetworkReply* reply);

private:
	QString addr;
	QString thread;
	int port;

	CommTcp* commtcp;

};

#endif // NOWLIVEWAKU_H
