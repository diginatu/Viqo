#ifndef NICOHTTP_H
#define NICOHTTP_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QTreeWidgetItem>

class MainWindow;

class NicoHttp : public QObject
{
	Q_OBJECT
public:
	explicit NicoHttp(MainWindow* mwin, QObject *parent = 0);
  ~NicoHttp();
	void getBody(QUrl url);

protected:
	MainWindow* mwin;
	QNetworkReply* reply;
  QNetworkAccessManager mManager;

	QVariant makePostData();

protected slots:
	virtual void finished();

signals:

};

#endif // NICOHTTP_H
