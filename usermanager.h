#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QTreeWidgetItem>

#include "nicohttp.h"

class UserManager : public NicoHttp
{
	Q_OBJECT
public:
	UserManager(MainWindow* mwin, QObject* parent = 0);
	void getUserName(QTreeWidgetItem* item, QString userID);

private:
	MainWindow* mwin;
	QTreeWidgetItem* item;

private slots:
	void finished();

};

#endif // USERMANAGER_H
