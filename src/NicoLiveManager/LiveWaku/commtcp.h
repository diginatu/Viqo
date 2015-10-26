#ifndef COMMTCP_H
#define COMMTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDateTime>
#include <QProcess>
#include <QTimer>
#include <nl/strabstractor.h>


class MainWindow;
class NowLiveWaku;

class CommTcp : public QObject
{
  Q_OBJECT
public:
  explicit CommTcp(QString domain, int port, QString thread, MainWindow* mwin, NowLiveWaku* nlwaku, QObject* parent = 0);
  void doConnect();
  void close();
  void sendComment(const QString& text);
  QPair<QString, QString> postKeyInfo();

signals:

public slots:
  void connected();
  void disconnected();
  void readyRead();
  void readOneRawComment(const QString rawcomm);
  bool isConnected();
  void sendNull();

private:
  QTcpSocket* socket;
  QString domain, thread, ticket, user_id;
  uint port, server_time;
  QDateTime open_time;
  QByteArray lastRawComm;
  QTimer nullData_timer, postkey_timer;

  MainWindow* mwin;
  NowLiveWaku* nlwaku;

  int lastBlockNum;

private slots:
  void getPostKey();
};

#endif // COMMTCP_H
