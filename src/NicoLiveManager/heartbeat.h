#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QObject>
#include "httpgetter.h"
#include "nicolivemanager.h"

class HeartBeat : public HttpGetter
{
  Q_OBJECT
public:
  HeartBeat(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent = 0);
  ~HeartBeat();
  void get();
private:
  NicoLiveManager* nlman;
private slots:
  void gotReply(QNetworkReply* reply);
signals:
  void got(QString watchCount);

};

#endif // HEARTBEAT_H
