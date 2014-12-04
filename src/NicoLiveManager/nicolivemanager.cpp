#include "nicolivemanager.h"
#include "../mainwindow.h"

NicoLiveManager::NicoLiveManager(MainWindow* mwin, SettingsWindow* swin, QObject *parent) :
  QObject(parent),
  nowWaku(mwin, this),
  wakutcp(NULL)
{
  this->mwin = mwin;
  this->swin = swin;

  // set timer to delete the ended elements in waku list.
  delWakuTimer = new QTimer(this);
  delWakuTimer->setInterval(30000);

  connect(delWakuTimer, SIGNAL(timeout()), this, SLOT(deleteWakuList()));
  delWakuTimer->start();
}

NicoLiveManager::~NicoLiveManager()
{
  delWakuTimer->stop();
  delWakuTimer->deleteLater();
}

QVariant NicoLiveManager::makePostData(QString session_id)
{
  QVariant postData;

  // make cookies
  QList <QNetworkCookie> cookies;
  QNetworkCookie ck;
  ck.toRawForm(QNetworkCookie::NameAndValueOnly);
  ck.setName("user_session");

  QByteArray user_id_ba;
  user_id_ba.append(session_id);

  ck.setValue(user_id_ba);
  cookies.append(ck);

  postData.setValue(cookies);
  return postData;
}

void NicoLiveManager::insertLiveWakuList(LiveWaku* livewaku)
{
  const QString livewakuBID = livewaku->getBroadID();
  foreach (LiveWaku* alistwaku, liveWakuList) {
    if (livewakuBID == alistwaku->getBroadID())
      return;
  }

  livewaku->getPlayyerStatusAPI();
  liveWakuList << livewaku;
}

void NicoLiveManager::broadStart()
{
  nowWaku.getPlayyerStatusAPI();
}

void NicoLiveManager::broadDisconnect()
{
  nowWaku.broadDisconnect();
}

void NicoLiveManager::deleteWakuList()
{
  QDateTime nowTime = QDateTime::currentDateTimeUtc();
  for (int i = 0; i < liveWakuList.size(); ++i) {
    if ( nowTime > liveWakuList[i]->getEd() ) {
      liveWakuList[i]->getPlayyerStatusAPI();
    }
  }
}
