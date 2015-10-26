#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QTreeWidgetItem>
#include <QtSql/QtSql>
#include <QMessageBox>
#include <nl/fetchusername.h>

class MainWindow;

class UserManager : QObject
{
	Q_OBJECT
public:
	UserManager(MainWindow* mwin, QObject* parent = 0);
  void getUserName(QTreeWidgetItem* item, QString userID, bool useHTTP = true, bool useDB = true);
  void setUserName(QTreeWidgetItem* item, QString username);
  void removeUser(QTreeWidgetItem* item);

private:
	QSqlDatabase db;
  MainWindow* mwin;

};

#endif // USERMANAGER_H
