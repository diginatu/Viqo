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
  NowLiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QString community, QObject* parent);

	void broadDisconnect();

  void getPostKeyAPI(const QString& thread, int block_num);

  QString getPostKey() const;
  void setPostKey(const QString& value);

  void sendComment(const QString& text, const QString& name = nullptr);

  bool getIs_premium() const;
  QString getUser_id() const;
  QString getOwner_id() const;

  QString getOwnerCommentToken() const;
  void setOwnerCommentToken(const QString& value);

  bool isOwnerBroad() const;

  void init();
  bool didAlermCommand;
  bool didExtend;
  bool didUpdate;

  bool getIsConnected() const;
  void setIsConnected(bool value);

  void getPlayerStatusAPI();

private slots:
  void playerStatusFinished(QNetworkReply* reply);

private:
  QString addr;
	QString thread;
	int port;

  QString postKey;
  QString user_id;
  QString owner_id;
  bool is_premium;

	CommTcp* commtcp;

  QString ownerCommentToken;
  bool ownerBroad;

  bool isConnected;

};

#endif // NOWLIVEWAKU_H
