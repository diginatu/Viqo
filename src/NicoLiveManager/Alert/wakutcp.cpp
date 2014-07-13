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

	mwin->onReceiveStarted();
}

void WakuTcp::disconnected()
{
	mwin->onReceiveEnded();
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
//	qDebug() << rawwaku << "\n";

	if (rawwaku.startsWith("<thread")) {
		return;
	}

	StrAbstractor awaku(rawwaku);
	QString wakuS = awaku.midStr(">", "</chat>");

	QStringList wakur = wakuS.split(",");

	if(wakur.size() != 3) {
		return;
	}

	qDebug() << wakur.at(0) << "," << wakur.at(1) << "," << wakur.at(2);


	if ( mwin->is_next_waku() ){
		if ( nicolivemanager->getCommunity() == wakur.at(1) ){
			mwin->setHousouID("lv" + wakur.at(0));
			mwin->on_receive_clicked();
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
