#ifndef HTTPGETTER_H
#define HTTPGETTER_H

#include <QObject>
#include <qnetworkaccessmanager.h>
#include "nicolivemanager.h"
#include "../../ui/mainwindow.h"

class httpGetter : public QObject
{
  Q_OBJECT
public:
  explicit httpGetter(MainWindow* mwin, NicoLiveManager* nlman, QObject *parent = 0);
protected:
  void get();
  ~httpGetter();
  QNetworkAccessManager* mManager;
  MainWindow* mwin;
  NicoLiveManager* nlman;

private:

signals:

public slots:

private slots:

protected slots:
  void got(QNetworkReply* reply);
};

#endif // HTTPGETTER_H
