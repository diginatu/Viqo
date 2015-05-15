#include "nicolivemanager.h"
#include "../../ui/mainwindow.h"

NicoLiveManager::NicoLiveManager(MainWindow* mwin, AccountWindow* awin, NewWakuSettingsWindow* nwin, FollowCommunity* fwin, QObject *parent) :
  QObject(parent),
  nowWaku(mwin, this, this),
  wakutcp(nullptr),
  mPostKeyManager(nullptr),
  mLoginAlertManager(nullptr),
  mAdminAlertManager(nullptr),
  mLoginManager(nullptr),
  mOwnerCommentManager(nullptr),
  mOwnerCommentSManager(nullptr),
  mRawMyLiveManager(nullptr),
  mTags(nullptr),
  mNewWaku(nullptr),
  mConfigure(nullptr),
  mCommunityInfo(nullptr)
{
  this->mwin = mwin;
  this->awin = awin;
  this->nwin = nwin;
  this->fwin = fwin;

  // set timer to delete the ended elements in waku list.
  delWakuTimer = new QTimer(this);

  connect(delWakuTimer, SIGNAL(timeout()), this, SLOT(deleteWakuList()));
  delWakuTimer->start(15000);
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
    if (livewakuBID == alistwaku->getBroadID()) {
      delete livewaku;
      return;
    }
  }

  livewaku->getPlayerStatusAPI();
  liveWakuList << livewaku;
}

void NicoLiveManager::broadStart()
{
  nowWaku.init();
  nowWaku.getPlayerStatusAPI();
}

void NicoLiveManager::broadDisconnect(bool disableFollow)
{
  watchCount = "0";
  mwin->updateWatchCount();
  nowWaku.broadDisconnect();
  if (disableFollow)
    nowWaku.setCommunity("");
}

void NicoLiveManager::deleteWakuList()
{
  QDateTime nowTime = QDateTime::currentDateTimeUtc();
  for (int i = 0; i < liveWakuList.size(); ++i) {
    if ( nowTime > liveWakuList[i]->getEd() ) {
      liveWakuList[i]->getPlayerStatusAPI();
    }
  }
}

void NicoLiveManager::allGotWakuInfo(QString communityID, QString broadID)
{
  if (mwin->settings.isAutoNextWaku()) {
    if ( nowWaku.getCommunity() == communityID &&
         nowWaku.getBroadID() != broadID )
    {
      mwin->setHousouID(broadID);
      mwin->on_receive_clicked();

      if (mwin->settings.isCommandNextWakuChecked()) {
        QProcess pr;
        QString cmd = mwin->settings.getCommandNextWaku();

        cmd.replace("%wakuURL%","http://live.nicovideo.jp/watch/lv" + broadID);

        pr.start(cmd);
        pr.waitForFinished(5000);
      }
    }
  }
}

QString NicoLiveManager::HTMLdecode(QString st)
{
  st.replace("&amp;", "&");
  st.replace("&lt;", "<");
  st.replace("&gt;", ">");
  return st;
}
QString NicoLiveManager::getWatchCount() const
{
    return watchCount;
}

void NicoLiveManager::setWatchCount(const QString& value)
{
    watchCount = value;
}

