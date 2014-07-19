#ifndef LIVEWAKU_H
#define LIVEWAKU_H

#include <QObject>
#include <QDateTime>

class LiveWaku : public QObject
{
	Q_OBJECT
public:
	explicit LiveWaku(QObject *parent = 0);
	explicit LiveWaku(QString broadID, QString community = "", QString title = "", QObject *parent = 0);


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

private:
	QString title;
	QString broadID;
	QString community;
	QDateTime st;
	QDateTime ed;

};

#endif // LIVEWAKU_H
