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
  //	qDebug() << repdata;

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

  commtcp = new CommTcp(addr, port, thread, mwin, this);
  commtcp->doConnect();
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

void NowLiveWaku::sendComment(const QString& text)
{
  commtcp->sendComment(text);
}
