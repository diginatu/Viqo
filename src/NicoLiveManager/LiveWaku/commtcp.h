#ifndef COMMTCP_H
#define COMMTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDateTime>
#include <QProcess>
#include <QTimer>


class MainWindow;
class NowLiveWaku;

class CommTcp : public QObject
{
  Q_OBJECT
public:
  explicit CommTcp(QString domain, int port, QString thread, QString user_id, MainWindow* mwin, NowLiveWaku* nlwaku);
  void doConnect();
  void close();
  void sendComment(const QString& text);
  QPair<QString, QString> postKeyInfo();

signals:

public slots:
  void connected();
  void disconnected();
  void readyRead();
  void readOneRawComment(QByteArray& rawcomm);
  bool isConnected();
  void sendNull();

private:
  QTcpSocket* socket;
  QString domain, thread, ticket, user_id;
  uint port, server_time;
  QDateTime open_time;
  QByteArray lastRawComm;
  QTimer nullDataTimer;

  MainWindow* mwin;
  NowLiveWaku* nlwaku;

  int lastBlockNum;
};

#endif // COMMTCP_H
