#ifndef LIVEWAKU_H
#define LIVEWAKU_H

#include <QObject>
#include <QDateTime>
#include <QNetworkReply>

class MainWindow;
class NicoLiveManager;

class LiveWaku : public QObject
{
	Q_OBJECT
public:
  explicit LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QObject *parent = 0);
  explicit LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QObject *parent = 0);
  explicit LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QString community, QObject *parent = 0);

  void init();
  bool didAlermCommand;

  void getPlayerStatusAPI();

	QString getTitle() const;
  void setTitle(QString value);

	QString getBroadID() const;
  void setBroadID(QString value);

	QDateTime endTime;

	QString getCommunity() const;
  void setCommunity(QString value);

	QDateTime getSt() const;
	void setSt(uint unixt);

	QDateTime getEd() const;
	void setEd(uint unixt);

  void setFlag(int flag);

  QString getBroadcastToken() const;

signals:

public slots:

private slots:
protected slots:
  void playerStatusFinished(QNetworkReply* reply);

private:
  QNetworkAccessManager* mManager;

  QString title;
	QString broadID;
	QString community;
  QString broadcastToken;

	QDateTime st;
	QDateTime ed;

  int flag;

protected:
  MainWindow* mwin;
	NicoLiveManager* nlman;

};

#endif // LIVEWAKU_H
