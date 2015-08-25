#ifndef GETSTREAMINFO_H
#define GETSTREAMINFO_H

#include <QObject>
#include "httpgetter.h"
#include "nicolivemanager.h"

class GetStreamInfo : public HttpGetter
{
  Q_OBJECT
public:
  GetStreamInfo(MainWindow* mwin, NicoLiveManager* nlman, const QString& broadID, QObject *parent = 0);
  ~GetStreamInfo();

  void get();

private:
  QString broadID;
  NicoLiveManager* nlman;
private slots:
  void got(QNetworkReply* reply);
signals:

public slots:
};

#endif // GETSTREAMINFO_H
