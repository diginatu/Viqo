#ifndef NICOLIVEMANAGER_H
#define NICOLIVEMANAGER_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>

#include <nl/strabstractor.h>
#include "Alert/wakutcp.h"
#include "LiveWaku/livewaku.h"
#include "LiveWaku/nowlivewaku.h"
#include "../../ui/settingswindow.h"
#include "../../ui/newwakusettingswindow.h"
#include "../../ui/accountwindow.h"
#include "../../ui/matchandaddbroadcast.h"

namespace Ui {
class MainWindow;
}

class NicoLiveManager : public QObject
{
  Q_OBJECT
public:
  explicit NicoLiveManager(MainWindow* mwin, AccountWindow* awin, NewWakuSettingsWindow* nwin, MatchAndAddBroadcast* bwin, QObject *parent = 0);
  ~NicoLiveManager();

  static QVariant makePostData(QString session_id);

  void insertLiveWakuList(LiveWaku* livewaku);

  void loginAlertAPI(const QString& mail, const QString& pass);
  void getPostKeyAPI(const QString& thread, int block_no);
  void getPublishStatusAPI();
  void submitOwnerCommentAPI(const QString& text, const QString& name);
  void getTagsAPI();
  void getNewWakuAPI(const int type, QString liveNum = "");
  void configureStreamAPI(QString key, QString value, LiveWaku* nowWaku = nullptr);

  void alertReconnect();

  void allGotWakuInfo(QString communityID, QString broadID);
  void login(QString mail, QString pass);

  QStringList officialMyCommunities;
  QStringList mycommunities;

  NowLiveWaku nowWaku;

  QList<LiveWaku*> liveWakuList;

  void broadDisconnect(bool disableFollow = false);
  void broadStart();

  void newWakuSetFormData(QString name, QString value);

  static QString HTMLdecode(QString st);

  void updateMyCommunities();
  QString getWatchCount() const;
  void setWatchCount(const QString& value);

signals:

public slots:
  void getRawMyLiveHTML();

private:
  void adminAlertAPI(const QString& ticket);
  void newWakuAbstractor(QNetworkReply* reply, int mode);

  MainWindow* mwin;
  AccountWindow* awin;
  NewWakuSettingsWindow* nwin;
  MatchAndAddBroadcast* bwin;
  WakuTcp* wakutcp;

  QString watchCount;

  QString waku_addr;
  QString waku_thread;
  int waku_port;

  QTimer* delWakuTimer;

  QMultiMap<QString, QString> newWakuData;

  LiveWaku* gotNewWaku;

  QNetworkAccessManager* mPostKeyManager;
  QNetworkAccessManager* mLoginAlertManager;
  QNetworkAccessManager* mAdminAlertManager;
  QNetworkAccessManager* mLoginManager;
  QNetworkAccessManager* mOwnerCommentManager;
  QNetworkAccessManager* mOwnerCommentSManager;
  QNetworkAccessManager* mRawMyLiveManager;
  QNetworkAccessManager* mTags;
  QNetworkAccessManager* mNewWaku;
  QNetworkAccessManager* mConfigure;
private slots:
  void loginAlertFinished(QNetworkReply* reply);
  void adminAlertFinished(QNetworkReply* reply);
  void rawMyLivefinished(QNetworkReply* reply);
  void loginFinished(QNetworkReply* reply);
  void postKeyFinished(QNetworkReply* reply);
  void tagsFinished(QNetworkReply* reply);
  void newWakuNewReuseFinished(QNetworkReply* reply);
  void newWakuNewUpdateFinished(QNetworkReply* reply);
  void newWakuNewInitFinished(QNetworkReply* reply);
  void newWakuConfirmFinished(QNetworkReply* reply);
  void newWakuFinished(QNetworkReply* reply);

  void publishStatusFinished(QNetworkReply* reply);
  void submitOwnerCommentFinished(QNetworkReply* reply);

  void deleteWakuList();
  void configureStreamFinished(QNetworkReply* reply);
};


#endif // NICOLIVEMANAGER_H
