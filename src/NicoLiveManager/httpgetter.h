#ifndef HTTPGETTER_H
#define HTTPGETTER_H

#include <QObject>
#include <qnetworkaccessmanager.h>
#include "nicolivemanager.h"

class MainWindow;

class HttpGetter : public QObject
{
  Q_OBJECT
public:
  explicit HttpGetter(MainWindow* mwin, QObject *parent = 0);
  ~HttpGetter();
protected:
  void requestGet(const QNetworkRequest& rq);
  void requestPost(const QNetworkRequest& rq, QIODevice* data);
  void get();
  QNetworkAccessManager* mManager;
  MainWindow* mwin;

private:

signals:

public slots:

private slots:

protected slots:
  void got(QNetworkReply* reply);
};

#endif // HTTPGETTER_H
