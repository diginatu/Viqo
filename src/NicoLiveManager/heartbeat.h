#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QObject>
#include "httpgetter.h"
#include "nicolivemanager.h"

class HeartBeat : public HttpGetter
{
  Q_OBJECT
public:
  HeartBeat(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent);
  ~HeartBeat();
  void get();
private:
  NicoLiveManager* nlman;
private slots:
  void got(QNetworkReply* reply);
};

#endif // HEARTBEAT_H
