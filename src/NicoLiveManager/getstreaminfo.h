#ifndef GETSTREAMINFO_H
#define GETSTREAMINFO_H

#include <QObject>
#include <nl/httpgetter.h>
#include "nicolivemanager.h"

class GetStreamInfo : public nicolive::HttpGetter
{
  Q_OBJECT
public:
  GetStreamInfo(MainWindow* mwin, const QString& broadID, QObject *parent = 0);
  ~GetStreamInfo();

  void get();

private:
  QString broadID;
  MainWindow* mwin;
private slots:
  void gotReply(QNetworkReply* reply);
signals:
  void got(QString broadID, QString title, QString description, QString communityID);

public slots:
};

#endif // GETSTREAMINFO_H
