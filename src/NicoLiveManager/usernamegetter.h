#ifndef USERNAMEGETTER_H
#define USERNAMEGETTER_H

#include <QObject>
#include <QSqlDatabase>
#include "httpgetter.h"


class MainWindow;

class UserNameGetter : public HttpGetter
{
  Q_OBJECT
public:
  UserNameGetter(MainWindow* mwin, QObject* parent, QString userID);
  ~UserNameGetter();

  void get();

private:
  QString userID;
private slots:
  void gotReply(QNetworkReply* reply);

signals:
  void got(QString username);

};

#endif // USERNAMEGETTER_H
