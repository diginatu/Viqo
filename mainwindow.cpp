#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::insLog(QString log)
{
	ui->logtext->append(log + "\n");
}

void MainWindow::insComment(int num, QString user, QString comm, QString date)
{
	QStringList ls;

	ls += QString::number(num);
	ls += user;
	ls += comm;
	ls += date;

	QTreeWidgetItem * item = new QTreeWidgetItem(ls);
	ui->commentView->insertTopLevelItem(0, item);
}

QVariant makePostData(QString session_id){
	QVariant postData;

	// make cookies
	QList <QNetworkCookie> cookies;
	QNetworkCookie ck;
	ck.toRawForm(QNetworkCookie::NameAndValueOnly);
	ck.setName("user_session");
	QByteArray user_id_ba;
	user_id_ba.append(session_id);
	ck.setValue(user_id_ba);
	cookies.append(ck);

	postData.setValue(cookies);
	return postData;
}

void MainWindow::getHeartBeatAPI(QString session_id, QString broad_id)
{
	mManager = new QNetworkAccessManager(this);

	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData(session_id);
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://live.nicovideo.jp/api/heartbeat?v=" + broad_id));

	reply = mManager->get(rq);
	connect(reply,SIGNAL(finished()),this,SLOT(heartbeatfinished()));
}

void MainWindow::getAPI(QString session_id, QString broad_id)
{
	mManager = new QNetworkAccessManager(this);

	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData(session_id);
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://watch.live.nicovideo.jp/api/getplayerstatus?v=" + broad_id));

	reply = mManager->get(rq);
	connect(reply,SIGNAL(finished()),this,SLOT(finished()));
}

void MainWindow::finished()
{
	QByteArray repdata = reply->readAll();
	QString name[3] = {"addr","port","thread"};
	int adrp  = repdata.indexOf("<"+name[0]+">") + name[0].length() + 2;
	int adrpe = repdata.indexOf("</"+name[0]+">",adrp);
	int prtp  = repdata.indexOf("<"+name[1]+">",adrpe) + name[1].length() + 2;
	int prtpe = repdata.indexOf("</"+name[1]+">",prtp);
	int thrp  = repdata.indexOf("<"+name[2]+">",prtpe) + name[2].length() + 2;
	int thrpe = repdata.indexOf("</"+name[2]+">",thrp);

	addr = QString(repdata.mid(adrp, adrpe-adrp));
	port = repdata.mid(prtp, prtpe-prtp).toInt();
	thread = QString(repdata.mid(thrp, thrpe-thrp));

	qDebug() << addr << "\n" << port << "\n" << thread;

	commtcp = new CommTcp(addr, port, thread, this);
	commtcp->doConnect();
}


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->lineEdit->setEchoMode(QLineEdit::Password);
	ui->statusBar->showMessage("来場者数: 0");
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::heartbeatfinished(){
	QByteArray repdata = reply->readAll();
	QString wachSt = "watchCount";

	int adrp  = repdata.indexOf("<"+wachSt+">") + wachSt.length() + 2;
	int adrpe = repdata.indexOf("</"+wachSt+">",adrp);

	QString  watchCount= repdata.mid(adrp, adrpe-adrp);

	ui->statusBar->showMessage("来場者数: " + watchCount);
	//ui->watch_count_label->setText(watchCount);
}

void MainWindow::on_receive_clicked()
{
	const QString user_id = ui->lineEdit->text();
	const QString broad_id = ui->lineEdit_2->text();
	getAPI(user_id, broad_id);
}

void MainWindow::on_pushButton_2_clicked()
{
	const QString user_id = ui->lineEdit->text();
	const QString broad_id = ui->lineEdit_2->text();
	getHeartBeatAPI(user_id,broad_id);
}

void MainWindow::on_disconnect_clicked()
{
	commtcp->close();
}

