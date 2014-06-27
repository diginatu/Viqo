#ifndef COOKIEREAD_H
#define COOKIEREAD_H

#include <QObject>
#include <QtSql/QtSql>

class MainWindow;

class CookieRead : public QObject
{
	Q_OBJECT
public:
	explicit CookieRead(MainWindow* mwin);
	~CookieRead();
	QString getUserSession();
	void close();

private:
	QSqlDatabase db;
	MainWindow* mwin;

signals:

public slots:

};

#endif // COOKIEREAD_H
