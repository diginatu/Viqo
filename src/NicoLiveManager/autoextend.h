#ifndef AUTOEXTEND_H
#define AUTOEXTEND_H

#include <QObject>
#include <QDateTime>
#include <QMessageBox>
#include <nl/httpgetter.h>

class MainWindow;
class NicoLiveManager;

class AutoExtend : public nicolive::HttpGetter
{
  Q_OBJECT
public:
  AutoExtend(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent);
  ~AutoExtend();
  void get();

private:
  QNetworkAccessManager* mExtendManager;
  void getExtend(QString code, QString item, QString num);
  NicoLiveManager* nlman;
  MainWindow* mwin;

private slots:
  void gotReply(QNetworkReply* reply);
  void gotExtend(QNetworkReply* reply);
};

#endif // AUTOEXTEND_H
