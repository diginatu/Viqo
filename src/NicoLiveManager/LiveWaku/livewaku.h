#ifndef LIVEWAKU_H
#define LIVEWAKU_H

#include <QObject>
#include <QDateTime>
#include <QNetworkReply>

class MainWindow;

class LiveWaku : public QObject
{
	Q_OBJECT
public:
	explicit LiveWaku(MainWindow* mwin, QObject *parent = 0);
	explicit LiveWaku(MainWindow* mwin, QString broadID, QObject *parent = 0);
	explicit LiveWaku(MainWindow* mwin, QString broadID, QString community = "", QString title = "", QObject *parent = 0);

	void getPlayyerStatusAPI(QString session_id);

	QString getTitle() const;
	void setTitle(const QString& value);

	QString getBroadID() const;
	void setBroadID(const QString& value);

	QDateTime endTime;

	QString getCommunity() const;
	void setCommunity(const QString& value);

	QDateTime getSt() const;
	void setSt(uint unixt);

	QDateTime getEd() const;
	void setEd(uint unixt);

signals:

public slots:

private slots:
protected slots:
	void playerStatusFinished(QNetworkReply* reply);

private:
protected:
	QString title;
	QString broadID;
	QString community;

	QDateTime st;
	QDateTime ed;

	MainWindow* mwin;

};

#endif // LIVEWAKU_H
