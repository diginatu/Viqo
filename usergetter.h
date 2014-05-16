#ifndef USERGETTER_H
#define USERGETTER_H

#include <QObject>
#include <QTreeWidgetItem>
#include <QtSql/QtSql>

#include "nicohttp.h"

class UserGetter : public NicoHttp
{
	Q_OBJECT
public:
	UserGetter(MainWindow* mwin, QObject* parent = 0);
	void getUserName(QTreeWidgetItem* item, QString userID, QSqlDatabase& db);

private:
	MainWindow* mwin;
	QTreeWidgetItem* item;
	QSqlDatabase* db;
	QString userID;

private slots:
	void finished();

};

#endif // USERGETTER_H
