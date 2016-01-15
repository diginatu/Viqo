#ifndef NICOLIVEMANAGER_H
#define NICOLIVEMANAGER_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>
#include <nl/strabstractor.h>
#include <nl/livewaku.h>
#include "alertmanager.h"
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

  void getPublishStatusAPI();
  void submitOwnerCommentAPI(const QString& text, const QString& name);
  void getNewWakuAPI(const int type, QString liveNum = "");
  void configureStreamAPI(QString key, QString value, LiveWaku* nowWaku = nullptr);

  void allGotWakuInfo(QString communityID, QString broadID);

  NowLiveWaku nowWaku;
  AlertManager alertmanager;

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
  void newWakuAbstractor(QNetworkReply* reply, int mode);

  MainWindow* mwin;
  AccountWindow* awin;
  NewWakuSettingsWindow* nwin;
  MatchAndAddBroadcast* bwin;

  QString watchCount;

  QString waku_addr;
  QString waku_thread;
  int waku_port;

  QTimer* delWakuTimer;

  QMultiMap<QString, QString> newWakuData;

  LiveWaku* gotNewWaku;

  QNetworkAccessManager* mOwnerCommentManager;
  QNetworkAccessManager* mOwnerCommentSManager;
  QNetworkAccessManager* mNewWaku;
  QNetworkAccessManager* mConfigure;
private slots:
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
