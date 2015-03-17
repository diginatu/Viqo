#include "wakutcp.h"
#include "../../../ui/mainwindow.h"
#include "../nicolivemanager.h"

WakuTcp::WakuTcp(QString domain, int port, QString thread, MainWindow* mwin, NicoLiveManager* nicolivemanager)
{
  this->domain = domain;
  this->port = port;
  this->thread = thread;
  this->mwin = mwin;
  this->nicolivemanager = nicolivemanager;

  socket = new QTcpSocket(this);

  connect(socket, SIGNAL(connected()),this, SLOT(connected()));
  connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
  connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

  connect(&checkConnectionTimer,SIGNAL(timeout()),this,SLOT(timeoutConnection()));
}

WakuTcp::~WakuTcp()
{
}

void WakuTcp::doConnect()
{
  mwin->insLog("WakuTcp::doConnect");
  socket->connectToHost(domain, port);

  if(!socket->waitForConnected(5000)) {
    mwin->insLog("Error: " + socket->errorString() + "\n");
    QMessageBox msgBox;
    msgBox.setText(QStringLiteral("アラート受信開始に失敗しました再接続しますか？"));
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() == QMessageBox::Yes)
      doConnect();
  }

  mwin->insLog();
}

void WakuTcp::connected()
{
  mwin->insLog("WakuTcp::connected");
  QByteArray send;
  opentime = QDateTime::currentDateTime();

  send.append("<thread thread=\""+thread+"\" res_from=\"-1\" version=\"20061206\" />");
  send.append('\0');

  if (socket->write(send) == -1) {
    mwin->insLog("Error: " + socket->errorString() + "\n");
    socket->close();
    doConnect();
    return;
  }

  checkConnectionTimer.start(61000);
  mwin->insLog();
}

void WakuTcp::disconnected()
{
  checkConnectionTimer.stop();
}

void WakuTcp::readyRead()
{
  QList<QByteArray> rawwakus( socket->readAll().split('\0') );
  rawwakus[0].insert(0, lastRawWaku);

  for ( int i = 0; i < rawwakus.size()-1; ++i) {
    const QString tst(rawwakus[i]);
    readOneRawWaku(tst);
  }

  lastRawWaku = rawwakus.takeLast();
}

void WakuTcp::readOneRawWaku(const QString& rawwaku)
{
  checkConnectionTimer.start();
  if (rawwaku.startsWith("<thread")) {
    return;
  }

  StrAbstractor awaku(rawwaku);
  QString wakuS = awaku.midStr(">", "</chat>");

  QStringList wakur = wakuS.split(",");
  const QString broadID = wakur.at(0);
  const QString CommunityID = wakur.at(1);
  //	const QString& nushiID = wakur.at(2);

  if(wakur.size() != 3) {
    return;
  }

  nicolivemanager->allGotWakuInfo(CommunityID, broadID);

  foreach (QString commu, nicolivemanager->mycommunities) {
    if (commu == wakur.at(1)) {
      nicolivemanager->insertLiveWakuList(new LiveWaku(mwin, nicolivemanager, broadID, CommunityID));
      mwin->refleshLiveWaku();
    }
  }

}

void WakuTcp::close()
{
  socket->close();
}

bool WakuTcp::isConnected()
{
  return socket->state() != QAbstractSocket::UnconnectedState;
}

void WakuTcp::timeoutConnection()
{
  mwin->insLog("alert is disconnected\n");
  socket->close();
  nicolivemanager->getRawMyLiveHTML();
  QTimer::singleShot(30000, nicolivemanager, SLOT(getRawMyLiveHTML()));
  doConnect();
}
