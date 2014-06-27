#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QTreeWidgetItem>
#include <QtSql/QtSql>

#include "usergetter.h"

class UserManager : public NicoHttp
{
	Q_OBJECT
public:
	UserManager(MainWindow* mwin, QObject* parent = 0);
	void getUserName(QTreeWidgetItem* item, QString userID, bool useHTTP = true, bool useDB = true);

private:
//	MainWindow* mwin;
	QSqlDatabase db;

};

#endif // USERMANAGER_H
