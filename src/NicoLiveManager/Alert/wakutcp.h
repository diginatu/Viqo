#ifndef WAKUTCP_H
#define WAKUTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
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
  ~WakuTcp();
	void close();

signals:

public slots:
  void doConnect();
  void connected();
	void disconnected();
	void readyRead();
  void readOneRawWaku(const QString& rawwaku);
	bool isConnected();

private:
	MainWindow* mwin;
	NicoLiveManager* nicolivemanager;
	QTcpSocket* socket;

	QString domain, thread;
	int port;

	QDateTime opentime;
  QByteArray lastRawWaku;
  QTimer checkConnectionTimer;
private slots:
  void timeoutConnection();
};

#endif // WAKUTCP_H
