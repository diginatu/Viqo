#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QTreeWidgetItem>

class MainWindow;

class UserManager : public QObject
{
	Q_OBJECT
public:
	UserManager(MainWindow* mwin);
	void getUserName(QTreeWidgetItem* item, QString userID);

private:
	MainWindow* mwin;
	QNetworkReply* reply;
	QNetworkAccessManager* mManager;
	QTreeWidgetItem* item;

	QVariant makePostData();

public slots:

private slots:
	void finished();

signals:

};

#endif // USERMANAGER_H
