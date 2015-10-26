#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QObject>
#include "nl/httpgetter.h"

class NicoLiveManager;
class MainWindow;

class HeartBeat : public nicolive::HttpGetter
{
  Q_OBJECT
public:
  HeartBeat(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent = 0);
  ~HeartBeat();
  void get();
private:
  NicoLiveManager* nlman;
  MainWindow* mwin;
private slots:
  void gotReply(QNetworkReply* reply);
signals:
  void got(QString watchCount);

};

#endif // HEARTBEAT_H
