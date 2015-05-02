#include "nowlivewaku.h"
#include "../../../ui/mainwindow.h"

NowLiveWaku::NowLiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent) :
  LiveWaku(mwin, nlman, parent),
  commtcp(nullptr),
  isConnected(false)
{
}

NowLiveWaku::NowLiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QString community, QObject* parent) :
  LiveWaku(mwin, nlman, broadID, community, parent),
  commtcp(nullptr),
  isConnected(false)
{
}

void NowLiveWaku::broadDisconnect() {
  if (commtcp != nullptr) {
    if (commtcp->isConnected())
      commtcp->close();
    commtcp->deleteLater();
    commtcp = nullptr;
  }
  isConnected = false;
}

void NowLiveWaku::getPostKeyAPI(const QString& thread, int block_num)
{
  nlman->getPostKeyAPI(thread, block_num);
}

void NowLiveWaku::playerStatusFinished(QNetworkReply* reply)
{
  mwin->insLog("LiveWaku::playerStatusFinished :");
  QString repdata = QString(reply->readAll());
  // qDebug() << repdata;

  StrAbstractor commTcpi(repdata);

  QString status = commTcpi.midStr("status=\"","\"");
  if (status == "fail") {
    QString code = commTcpi.midStr("<code>","</code>");
    mwin->insLog(code);
    addr = ""; port = 0; thread = "";
    return;
  }

  setBroadID(commTcpi.midStr("<id>lv", "</id>"));
  setTitle(commTcpi.midStr("<title>", "</title>"));
  setCommunity(commTcpi.midStr("<default_community>", "</default_community>"));
  owner_id = commTcpi.midStr("<owner_id>", "</owner_id>");
  setOwnerName(commTcpi.midStr("<owner_name>", "</owner_name>"));

  setSt(commTcpi.midStr("<start_time>","</start_time>").toUInt());
  setEd(commTcpi.midStr("<end_time>","</end_time>").toUInt());

  broadcastToken = commTcpi.midStr("<broadcast_token>","</broadcast_token>");

  user_id = commTcpi.midStr("<user_id>", "</user_id>");
  is_premium = commTcpi.midStr("<is_premium>", "</is_premium>").toUInt();

  addr = commTcpi.midStr("<addr>", "</addr>");
  port = commTcpi.midStr("<port>", "</port>").toInt();
  thread = commTcpi.midStr("<thread>", "</thread>");

  mwin->insLog("addr: " + addr +
               "\nport: " + QString::number(port) +
               "\nthread:" + thread + "\n");

  if (mwin->settings.getOwnerComment() && owner_id == user_id) {
    nlman->getPublishStatusAPI();
    ownerBroad = true;
  } else {
    ownerBroad = false;
  }

  if (!isConnected) {
    commtcp = new CommTcp(addr, port, thread, mwin, this, this);
    commtcp->doConnect();
  }

  reply->deleteLater();
}
bool NowLiveWaku::getIsConnected() const
{
  return isConnected;
}

void NowLiveWaku::setIsConnected(bool value)
{
  isConnected = value;
}


void NowLiveWaku::init()
{
  didAlermCommand = false;
}

bool NowLiveWaku::isOwnerBroad() const
{
  return ownerBroad;
}

QString NowLiveWaku::getOwnerCommentToken() const
{
  return ownerCommentToken;
}

void NowLiveWaku::setOwnerCommentToken(const QString& value)
{
  ownerCommentToken = value;
}

QString NowLiveWaku::getOwner_id() const
{
  return owner_id;
}

QString NowLiveWaku::getUser_id() const
{
  return user_id;
}

bool NowLiveWaku::getIs_premium() const
{
  return is_premium;
}

QString NowLiveWaku::getPostKey() const
{
  return postKey;
}

void NowLiveWaku::setPostKey(const QString& value)
{
  postKey = value;
}

void NowLiveWaku::sendComment(const QString& text, const QString& name)
{
  if (!commtcp->isConnected()) {
    mwin->insLog("NowLiveWaku::sendComment commTcp is not connected");
    return;
  }

  if (mwin->settings.getOwnerComment() && ownerBroad) {
    nlman->submitOwnerCommentAPI(text, name);
  } else {
    commtcp->sendComment(text);
  }

}
