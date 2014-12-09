#include "nowlivewaku.h"
#include "../../mainwindow.h"

NowLiveWaku::NowLiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent) :
  LiveWaku(mwin, nlman, parent),
  commtcp(nullptr)
{
}

NowLiveWaku::NowLiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QObject* parent) :
  LiveWaku(mwin, nlman, broadID, parent),
  commtcp(nullptr)
{
}

void NowLiveWaku::broadDisconnect() {
  if (commtcp != nullptr) {
    if (commtcp->isConnected())
      commtcp->close();
    commtcp->deleteLater();
    commtcp = nullptr;
  }
}

void NowLiveWaku::getPostKeyAPI(const QString& thread, int block_num)
{
  nlman->getPostKeyAPI(thread, block_num);
}

void NowLiveWaku::playerStatusFinished(QNetworkReply* reply)
{
  mwin->insLog(" LiveWaku::playerStatusFinished :");
  QByteArray repdata = reply->readAll();
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

  setSt(commTcpi.midStr("<start_time>","</start_time>").toUInt());
  setEd(commTcpi.midStr("<end_time>","</end_time>").toUInt());

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

  commtcp = new CommTcp(addr, port, thread, mwin, this, this);
  commtcp->doConnect();

  reply->deleteLater();
}

bool NowLiveWaku::getOwnerBroad() const
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
