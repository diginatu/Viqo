#ifndef GETCOMMUNITYINFO_H
#define GETCOMMUNITYINFO_H

#include <QObject>
#include "httpgetter.h"

class GetCommunityInfo : public HttpGetter
{
  Q_OBJECT
private:
  QString commID;
public:
  GetCommunityInfo(MainWindow* mwin, QString commID, QObject* parent = 0);
  void get();

signals:
  void got(QString commID, QString title);

public slots:
  void gotReply(QNetworkReply* reply);
};

#endif // GETCOMMUNITYINFO_H
