#include "commtcp.h"

CommTcp::CommTcp(QString domain, int port, QString thread, MainWindow* mwin)
{
	this->domain = domain;
	this->port = port;
	this->thread = thread;
	this->mwin = mwin;
}

void CommTcp::doConnect()
{
	socket = new QTcpSocket(this);

	connect(socket, SIGNAL(connected()),this, SLOT(connected()));
	connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
	connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
	connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

	qDebug() << "connecting...";

	// this is not blocking call
	socket->connectToHost(domain, port);

	// we need to wait...
	if(!socket->waitForConnected(5000))
	{
		qDebug() << "Error: " << socket->errorString();
	}
}

void CommTcp::connected()
{
	qDebug() << "connected...";

	QByteArray send;
	send.append("<thread thread=\"" + thread + "\" version=\"20061206\" res_from=\"-1\"/>");
	send.append('\0');
	socket->write(send);
}

void CommTcp::disconnected()
{
	qDebug() << "disconnected...";
}

void CommTcp::bytesWritten(qint64 bytes)
{
	qDebug() << bytes << " bytes written...";
}

void CommTcp::readyRead()
{
	qDebug() << "reading...";

	rawcomm = socket->readAll();

	if (rawcomm.indexOf("<thread") != -1) {
		return;
	}

	int st = rawcomm.indexOf(">") + 1;
	int ed = rawcomm.indexOf("</chat>");

	QByteArray comment = rawcomm.mid(st,ed-st);

	qDebug() << comment;
}
