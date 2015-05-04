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
  UserNameGetter(MainWindow* mwin, QObject* parent,
                 QTreeWidgetItem* item, QString userID, QSqlDatabase& db);
  ~UserNameGetter();

  void get();

private:
  QTreeWidgetItem* item;
  QSqlDatabase* db;
  QString userID;
private slots:
  void got(QNetworkReply* reply);

};

#endif // USERNAMEGETTER_H
