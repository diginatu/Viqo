#ifndef WAKUTCP_H
#define WAKUTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QDateTime>
#include <QProcess>
#include <QTimer>

class MainWindow;
class NicoLiveManager;

class WakuTcp : public QObject
{
	Q_OBJECT
public:
	explicit WakuTcp(QString domain, int port, QString thread, MainWindow* mwin, NicoLiveManager* nicolivemanager);
	void doConnect();
	void close();

signals:

public slots:
	void connected();
	void disconnected();
	void bytesWritten(qint64 bytes);
	void readyRead();
	void readOneRawWaku(QByteArray& rawwaku);
	bool isConnected();

private:
	MainWindow* mwin;
	NicoLiveManager* nicolivemanager;
	QTcpSocket* socket;

	QString domain, thread;
	int port;

	QDateTime opentime;
	QByteArray lastRawWaku;
	QTimer* nullDataTimer;

};

#endif // WAKUTCP_H
