#include "commtcp.h"
#include "../../../ui/mainwindow.h"
#include "nowlivewaku.h"

CommTcp::CommTcp(QString domain, int port, QString thread, MainWindow* mwin, NowLiveWaku* nlwaku, QObject* parent) :
  QObject(parent),
  socket(NULL)
{
  this->domain = domain;
  this->port = port;
  this->thread = thread;
  this->mwin = mwin;
  this->nlwaku = nlwaku;

  socket = new QTcpSocket(this);

  connect(socket, SIGNAL(connected()),this, SLOT(connected()));
  connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
  connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

  connect(&postkey_timer, SIGNAL(timeout()), this, SLOT(getPostKey()));
  connect(&nullData_timer, SIGNAL(timeout()), this, SLOT(sendNull()));
}

void CommTcp::doConnect()
{
  socket->connectToHost(domain, port);

  if(!socket->waitForConnected(5000)) {
    mwin->insLog("CommTcp::doConnect Error: " + socket->errorString());
    QMessageBox msgBox(mwin);
    msgBox.setText(QStringLiteral("コメント受信が切断されました。再接続しますか？"));
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() == QMessageBox::Yes)
      doConnect();
  }
}

void CommTcp::connected()
{
  QByteArray send;

  send.append("<thread thread=\""+thread+"\" res_from=\"-1000\" version=\"20061206\" />");
  send.append('\0');

  if (socket->write(send) == -1) {
    mwin->insLog("CommTcp::connected Error: " + socket->errorString());
    doConnect();
  }

  // set timer to send NULL data.
  nullData_timer.start(60000);
}

void CommTcp::disconnected()
{
  mwin->onReceiveEnded();
}

void CommTcp::sendNull()
{
  QByteArray send(1, '\0');

  if (socket->write(send) == -1) {
    mwin->insLog("CommTcp::sendNull Error: " + socket->errorString() + "\n");
    mwin->bodyClear();
    socket->close();
    doConnect();
  }
}

void CommTcp::getPostKey()
{
  nlwaku->getPostKeyAPI(thread, lastBlockNum);
}

void CommTcp::readyRead()
{
  QList<QByteArray> rawcomms( socket->readAll().split('\0') );
  rawcomms[0].insert(0, lastRawComm);

  for ( int i = 0; i < rawcomms.size()-1; ++i) {
    const QString tst(rawcomms[i]);
    readOneRawComment(tst);
  }

  lastRawComm = rawcomms.takeLast();
}

void CommTcp::readOneRawComment(const QString rawcomm)
{
  StrAbstractor rawcommabs(rawcomm);

  if (rawcomm.startsWith("<thread")) {
    open_time = QDateTime::currentDateTime();
    mwin->onReceiveStarted();

    lastBlockNum = rawcommabs.midStr("last_res=\"", "\"", false).toInt() / 10;
    ticket = rawcommabs.midStr("ticket=\"", "\"", false);
    server_time = rawcommabs.midStr("server_time=\"", "\"", false).toUInt();

    nlwaku->getPostKeyAPI(thread, lastBlockNum);
    // set timer to get post_key
    postkey_timer.start(10000);

    return;

  } else if (rawcomm.startsWith("<chat_result")) {
    const auto status = rawcommabs.midStr("status=\"", "\"", false);
    if (status != "0") {
      mwin->insLog("CommTcp::readOneRawComment sendComment error with statue " + status);
    }
    mwin->submittedComment();
    return;
  }

  const QString comminfostr = rawcommabs.midStr("<chat", ">");
  StrAbstractor comminfo(comminfostr);
  rawcommabs.setRelativePosition(-2);

  QString comm = rawcommabs.midStr(">", "</chat>");

  int num = comminfo.midStr("no=\"", "\"", false).toInt();
  const int block = num/10;
  if (block > lastBlockNum) {
    lastBlockNum = block;
    nlwaku->getPostKeyAPI(thread, block);
  }

  long long udate = comminfo.midStr("date=\"", "\"", false).toLongLong();
  QDateTime commenttime;
  commenttime.setTime_t(udate);
  QString date = commenttime.toString("hh:mm:ss");

  QString mail = comminfo.midStr("mail=\"", "\"", false);
  bool is_184 = (mail.indexOf("184")!=-1) ? true : false;

  QString user = comminfo.midStr("user_id=\"", "\"", false);

  bool premium = false, broadcaster = false;
  QString premS = comminfo.midStr("premium=\"", "\"", false);
  if ( !premS.isEmpty() ) {
    int prem = premS.toInt();
    if (prem % 2) premium = true;
    if ((prem >> 1) % 2 ) broadcaster = true;
  }

  comm = NicoLiveManager::HTMLdecode(comm);

  int nextnum = mwin->lastCommentNum() + 1;
  if (mwin->settings.getDispNG() && nextnum != num && nextnum != 1) {
    for (int i = nextnum; i < num; ++i) {
      mwin->insComment( i, false, "NG", QStringLiteral("NGコメント"), date,
                        "NGcomment", false, false);
    }
  }

  mwin->insComment( num, premium,
    broadcaster?QStringLiteral("broadcaster"):user, comm, date, is_184,
    broadcaster, commenttime > open_time);

  if (comm == "/disconnect" && broadcaster) {
    if (mwin->settings.isAutoNewWaku() && nlwaku->isOwnerBroad())
      mwin->getNewWakuAPI(2);

    mwin->deleteBroadcastFromList(nlwaku->getBroadID());

    nlwaku->broadDisconnect();
  }
}

void CommTcp::close()
{
  nullData_timer.stop();
  postkey_timer.stop();
  socket->close();
}

void CommTcp::sendComment(const QString& text)
{
  const auto start_time = nlwaku->getSt().toTime_t();
  const auto now_time = QDateTime::currentDateTime();

  QString postkey = nlwaku->getPostKey();
  if (postkey == "") {
    mwin->insLog("CommTcp::sendComment no postKey\n");
    return;
  }

  QByteArray send;

  const auto vpos = 100 * (server_time - start_time +
                     now_time.toTime_t() - open_time.toTime_t());

  send.append(QString("<chat thread=\"%1\" ticket=\"%2\" vpos=\"%3\" "
          "postkey=\"%4\"%5 user_id=\"%6\"%7>%8</chat>")
      .arg(thread)
      .arg(ticket)
      .arg(vpos)
      .arg(postkey)
      .arg((mwin->settings.getIs184())?" mail=\"184\"":"")
      .arg(nlwaku->getUser_id())
      .arg((nlwaku->getIs_premium())?" premium=\"1\"":"")
      .arg(text.toHtmlEscaped()));

  send.append('\0');

  if (socket->write(send) == -1) {
    mwin->insLog("CommTcp::sendComment Error: " + socket->errorString());
    mwin->submittedComment();
  }
}

bool CommTcp::isConnected()
{
  if (socket == nullptr) return false;
  return socket->state() != QAbstractSocket::UnconnectedState;
}
