#include "wakutcp.h"
#include "../../mainwindow.h"
#include "../nicolivemanager.h"

WakuTcp::WakuTcp(QString domain, int port, QString thread, MainWindow* mwin, NicoLiveManager* nicolivemanager)
{
	this->domain = domain;
	this->port = port;
	this->thread = thread;
	this->mwin = mwin;
	this->nicolivemanager = nicolivemanager;
}

void WakuTcp::doConnect()
{
	socket = new QTcpSocket(this);

	connect(socket, SIGNAL(connected()),this, SLOT(connected()));
	connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
	connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
	connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

	socket->connectToHost(domain, port);

	if(!socket->waitForConnected(5000)) {
		throw QString("Error: ").append(socket->errorString());
	}
}

void WakuTcp::connected()
{
	QByteArray send;
	opentime = QDateTime::currentDateTime();

	send.append("<thread thread=\""+thread+"\" res_from=\"-1\" version=\"20061206\" />");
	send.append('\0');

	if (socket->write(send) == -1) {
		throw QString("Error: ").append(socket->errorString());
	}

}

void WakuTcp::disconnected()
{
}

void WakuTcp::bytesWritten(qint64 bytes)
{
	qDebug() << bytes << " bytes written...";
}

void WakuTcp::readyRead()
{
	QList<QByteArray> rawwakus( socket->readAll().split('\0') );
	rawwakus[0].insert(0, lastRawWaku);

	for ( int i = 0; i < rawwakus.size()-1; ++i) {
		readOneRawWaku(rawwakus[i]);
	}

	lastRawWaku = rawwakus.takeLast();
}

void WakuTcp::readOneRawWaku(QByteArray& rawwaku)
{
	if (rawwaku.startsWith("<thread")) {
		return;
	}

	StrAbstractor awaku(rawwaku);
	QString wakuS = awaku.midStr(">", "</chat>");

	QStringList wakur = wakuS.split(",");
	const QString& broadID = wakur.at(0);
	const QString& CommunityID = wakur.at(1);
//	const QString& nushiID = wakur.at(2);

//	qDebug() << CommunityID << "," << broadID;

	if(wakur.size() != 3) {
		return;
	}


	if ( mwin->isNextWaku() ){
		if ( nicolivemanager->nowWaku.getCommunity() == CommunityID &&
				 nicolivemanager->nowWaku.getBroadID() != broadID){
			mwin->setHousouID(broadID);
			mwin->on_receive_clicked();
		}
	}

	foreach (QString commu, nicolivemanager->mycommunities) {
		if (commu == wakur.at(1)) {
			nicolivemanager->insertLiveWakuList(new LiveWaku(mwin, nicolivemanager, broadID));
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
