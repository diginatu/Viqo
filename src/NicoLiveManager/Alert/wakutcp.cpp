#include "wakutcp.h"
#include "../../../ui/mainwindow.h"
#include "../nicolivemanager.h"
#include "../getstreaminfo.h"

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
  const QString& nushiID = wakur.at(2);

  if(wakur.size() != 3) {
    return;
  }

  nicolivemanager->allGotWakuInfo(CommunityID, broadID);

  auto insertNewWaku = [&](){
    nicolivemanager->insertLiveWakuList(new LiveWaku(mwin, nicolivemanager, broadID, CommunityID));
    mwin->refleshLiveWaku();
  };

  foreach (QString commu, nicolivemanager->mycommunities) {
    if (commu == wakur.at(1)) {
      insertNewWaku();
    }
  }

  if (mwin->settings.getMatchDataEnabled()) {
    foreach (QStringList match, mwin->settings.matchDataList) {
      if (match[1].indexOf('B')!=-1 && match[2] == broadID) {
        insertNewWaku();
        break;
      }
      if (match[1].indexOf('C')!=-1 && match[2] == CommunityID) {
        insertNewWaku();
        break;
      }
      if (match[1].indexOf('U')!=-1 && match[2] == nushiID) {
        insertNewWaku();
        break;
      }
    }

    if (mwin->settings.getMatchDataNeedDetailInfo()) {
      GetStreamInfo* getter = new GetStreamInfo(mwin, broadID, this);
      connect(getter, &GetStreamInfo::got, this,
              [=](QString broadID, QString title,
              QString description, QString communityID)
      {
        auto insertNewWaku = [&](){
          nicolivemanager->insertLiveWakuList(new LiveWaku(mwin, nicolivemanager, broadID, communityID));
          mwin->refleshLiveWaku();
        };

        foreach (QStringList match, mwin->settings.matchDataList) {
          if (match[1].indexOf('T')!=-1 && title.indexOf(match[2])!=-1) {
            insertNewWaku();
            break;
          }
          if (match[1].indexOf('D')!=-1 && description.indexOf(match[2])!=-1) {
            insertNewWaku();
            break;
          }
        }
      });

      getter->get();
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
