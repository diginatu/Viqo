#include "commtcp.h"
#include "mainwindow.h"

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

	socket->connectToHost(domain, port);

	if(!socket->waitForConnected(5000)) {
		throw QString("Error: ").append(socket->errorString());
	}
}

void CommTcp::connected()
{
	QByteArray send;
	opentime = QDateTime::currentDateTime();

	send.append("<thread thread=\""+thread+"\" res_from=\"-1000\" version=\"20061206\" />");
	send.append('\0');

	if (socket->write(send) == -1) {
		throw QString("Error: ").append(socket->errorString());
	}

	mwin->onReceiveStarted();

	// set timer to send NULL data.
	nullDataTimer = new QTimer(this);
	nullDataTimer->setInterval(60000);

	connect(nullDataTimer, SIGNAL(timeout()), this, SLOT(sendNull()));
	nullDataTimer->start();
}

void CommTcp::disconnected()
{
	mwin->onReceiveEnded();
}

void CommTcp::sendNull()
{
	QByteArray send(1, '\0');

	if (socket->write(send) == -1)
		throw QString("Error: ").append(socket->errorString());

}

void CommTcp::bytesWritten(qint64 bytes)
{
	qDebug() << bytes << " bytes written...";
}

void CommTcp::readyRead()
{
	QList<QByteArray> rawcomms( socket->readAll().split('\0') );
	rawcomms[0].insert(0, lastRawComm);

	for ( int i = 0; i < rawcomms.size()-1; ++i) {
		readOneRawComment(rawcomms[i]);
	}

	lastRawComm = rawcomms.takeLast();
}

void CommTcp::readOneRawComment(QByteArray& rawcomm)
{
	mwin->insLog(QString(rawcomm));

	if (rawcomm.startsWith("<thread")) {
		return;
	}

	int commSt = rawcomm.indexOf(">") + 1;
	int commEd = rawcomm.lastIndexOf("</chat>");

	QString comm = QString(rawcomm.mid(commSt,commEd-commSt));
	rawcomm.truncate(commSt);

	StrAbstractor comminfo(rawcomm);

	int num = comminfo.midStr("no=\"", "\"", false).toInt();

	int udate = comminfo.midStr("date=\"", "\"", false).toInt();
	QDateTime commenttime;
	commenttime.setTime_t(udate);
	QString date = commenttime.toString("yyyy/MM/dd hh:mm:ss");

	QString user = comminfo.midStr("user_id=\"", "\"", false);

	bool premium = false, broadcaster = false;
	QString premS = comminfo.midStr("premium=\"", "\"", false);
	if ( !premS.isEmpty() ) {
		int prem = premS.toInt();
		if (prem % 2) {
			premium = true;
		}
		if ((prem >> 1) % 2 ) {
			broadcaster = true;
		}
	}

	if (comm == "/disconnect" && broadcaster) {
		mwin->on_disconnect_clicked();
	}

	// html code decode
	comm.replace("&amp;", "&");
	comm.replace("&lt;", "<");
	comm.replace("&gt;", ">");

	QTreeWidgetItem* item = mwin->insComment(num,premium?"@":" ",broadcaster?"放送主":user,comm,date);

	if ( mwin->isCheckedAutoGettingUserName() && !broadcaster ) {
		// use HTTP connection for only received comment after started.
		mwin->userManager->getUserName(item, user, commenttime > opentime);
	}

	// comment command
	if ( !mwin->setting_commentCommand.isEmpty() && commenttime > opentime ) {
		QProcess pr;
		QString cmd = mwin->setting_commentCommand;

		cmd.replace("%comment%",comm);
		QString escmd = comm;
		escmd.replace("\"", "\\\"");
		escmd.replace("\\", "\\\\");
		cmd.replace("%comment_escaped%",escmd);

		pr.start(cmd);
		pr.waitForFinished(30000);
	}
}

void CommTcp::close()
{
	socket->close();
}

bool CommTcp::isConnected()
{
	return socket->state() != QAbstractSocket::UnconnectedState;
}
