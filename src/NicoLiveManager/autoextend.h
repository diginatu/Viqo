#ifndef AUTOEXTEND_H
#define AUTOEXTEND_H

#include "httpgetter.h"
#include "../../ui/mainwindow.h"
#include "nicolivemanager.h"

class autoExtend : public httpGetter
{
public:
  autoExtend(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent);
  ~autoExtend();

private:
  void get();
  QNetworkAccessManager* mExtendManager;

private slots:
  void got(QNetworkReply* reply);
  void getExtend(QString code, QString item, QString num);
  void gotExtend(QNetworkReply* reply);
};

#endif // AUTOEXTEND_H
