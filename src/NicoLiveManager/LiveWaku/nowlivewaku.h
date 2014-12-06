#ifndef NOWLIVEWAKU_H
#define NOWLIVEWAKU_H

#include <QObject>
#include "../../strabstractor.h"
#include "livewaku.h"
#include "commtcp.h"

class NowLiveWaku : public LiveWaku
{
	Q_OBJECT
public:
  NowLiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent);
  NowLiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QObject* parent);
  NowLiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QString community, QString title, QObject* parent);

	void broadDisconnect();

  void getPostKeyAPI(const QString& thread, int block_num);

  QString getPostKey() const;
  void setPostKey(const QString& value);

  void sendComment(const QString& text);

  bool getIs_premium() const;
  QString getUser_id() const;

private slots:
  void playerStatusFinished(QNetworkReply* reply);

private:
	QString addr;
	QString thread;
	int port;

  QString postKey;
  QString user_id;
  bool is_premium;

	CommTcp* commtcp;

};

#endif // NOWLIVEWAKU_H
