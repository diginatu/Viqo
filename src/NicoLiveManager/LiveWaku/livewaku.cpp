#include "livewaku.h"
#include "../../mainwindow.h"
#include "../nicolivemanager.h"

LiveWaku::LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent) :
  QObject(parent),
  mManager(nullptr)
{
  this->mwin = mwin;
  this->nlman = nlman;
  flag = 0;
}

LiveWaku::LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QObject* parent) :
  LiveWaku(mwin, nlman, parent)
{
  this->broadID = broadID;
}

LiveWaku::LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QString community, QObject *parent) :
  LiveWaku(mwin, nlman, broadID, parent)
{
  this->community = community;
}

void LiveWaku::init()
{
  didAlermCommand = false;
}

QString LiveWaku::getTitle() const
{
  return title;
}

void LiveWaku::setTitle(QString value)
{
  title = value;
}

QString LiveWaku::getBroadID() const
{
  return broadID;
}

void LiveWaku::setBroadID(QString value)
{
  broadID = value;
}

QString LiveWaku::getCommunity() const
{
  return community;
}

void LiveWaku::setCommunity(QString value)
{
  community = value;
}

QDateTime LiveWaku::getSt() const
{
  return st;
}

void LiveWaku::setSt(uint unixt)
{
  st.setTime_t(unixt);
}

QDateTime LiveWaku::getEd() const
{
  return ed;
}

void LiveWaku::setEd(uint unixt)
{
  ed.setTime_t(unixt);
}

void LiveWaku::setFlag(int flag)
{
  this->flag = flag;
}

QString LiveWaku::getBroadcastToken() const
{
  return broadcastToken;
}

void LiveWaku::getPlayerStatusAPI()
{
  if(mManager!=nullptr)  delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(playerStatusFinished(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = NicoLiveManager::makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getplayerstatus?v=lv" + broadID));
  mManager->get(rq);
}

void LiveWaku::playerStatusFinished(QNetworkReply* reply)
{
  mwin->insLog(" LiveWaku::playerStatusFinished :");
  QString repdata = QString(reply->readAll());

  StrAbstractor commTcpi(repdata);

  QString status = commTcpi.midStr("status=\"","\"");
  if (status == "fail") {
    QString code = commTcpi.midStr("<code>","</code>");
    mwin->insLog(code);

    if (code == "closed" || code == "deletedbyuser") {
      QList<LiveWaku*>& wlist = nlman->liveWakuList;

      const int this_index = wlist.indexOf(this);

      if (this_index != -1) {
        wlist.removeAt(this_index);
        mwin->refleshLiveWaku();
        this->deleteLater();
      }
    }
    mwin->insLog();
    return;
  }

  QString befTitle = title;

  broadID = commTcpi.midStr("<id>lv", "</id>");
  title = commTcpi.midStr("<title>", "</title>");
  community = commTcpi.midStr("<default_community>", "</default_community>");
  setSt(commTcpi.midStr("<start_time>","</start_time>").toUInt());
  setEd(commTcpi.midStr("<end_time>","</end_time>").toUInt());
  broadcastToken = commTcpi.midStr("<broadcast_token>","</broadcast_token>");

  mwin->refleshLiveWaku();
  mwin->insLog("got a broad info : " + title + "\n");

  reply->deleteLater();

  if (flag == 0 && mwin->settings.isCommandNewWakuChecked() && befTitle != title) {
    QProcess pr;
    QString cmd = mwin->settings.getCommandNewWaku();

    QString estitle = title;
    estitle.replace("\"", "\"\"\"");
    cmd.replace("%newTitle%", estitle);

    pr.start(cmd);
    pr.waitForFinished(5000);
  } else if (flag == 1) {
    nlman->configureStreamAPI("hq", "1", this);
  }
}
