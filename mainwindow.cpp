#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::clearView()
{
	ui->commentView->clear();
	ui->logtext->clear();
}

QString MainWindow::getCookieSetting(int n)
{
	switch (n) {
		case 0:
			return ui->cookiesetting_filename->text();
		case 1:
			return ui->cookiesetting_tablename->text();
		case 2:
			return ui->cookiesetting_column_basedomain->text();
		case 3:
			return ui->cookiesetting_column_name->text();
		case 4:
			return ui->cookiesetting_column_value->text();
		default:
			return "";
	}
}

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
	rq.setUrl(QUrl("http://live.nicovideo.jp/api/getplayerstatus?v=" + broad_id));

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

	insLog("addr: "+addr+"\nport: "+QString::number(port)+"\nthread:"+thread);

	commtcp = new CommTcp(addr, port, thread, this);
	commtcp->doConnect();
}

void MainWindow::getSessionFromCookie()
{
	try {
		CookieRead cr(this);
		ui->cookiesetting_usersession->setText(cr.getUserSession());
	} catch (QString e) {
		qDebug() << e;
	}
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->statusBar->showMessage("来場者数: 0");

	ui->cookiesetting_usersession->setEchoMode(QLineEdit::Password);

	on_actionLoad_triggered();

	getSessionFromCookie();
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
}

void MainWindow::on_receive_clicked()
{
	on_disconnect_clicked();
	clearView();

	const QString userSession = ui->cookiesetting_usersession->text();
	const QString broad_id = ui->lineEdit_2->text();
	getAPI(userSession, broad_id);
}

void MainWindow::on_pushButton_2_clicked()
{
	const QString userSession = ui->cookiesetting_usersession->text();
	const QString broad_id = ui->lineEdit_2->text();
	getHeartBeatAPI(userSession,broad_id);
}

void MainWindow::on_disconnect_clicked()
{
	if (commtcp != NULL)
		commtcp->close();
}

void MainWindow::on_cookiesetting_file_open_button_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open Cookies File"), QDir::homePath(), tr("sqlite Files (*.sqlite)"));
	if ( filePath == "" ) return;
	ui->cookiesetting_filename->setText(filePath);
}

void MainWindow::on_cookiesetting_apply_clicked()
{
	getSessionFromCookie();
}

void MainWindow::on_cookiesetting_browserCombo_currentIndexChanged(int index)
{
	switch (index) {
		case 0:
			ui->cookiesetting_browserSettings_group->setEnabled(true);
			ui->cookiesetting_usersession_groupe->setEnabled(false);
			break;
		case 1:
			ui->cookiesetting_browserSettings_group->setEnabled(false);
			ui->cookiesetting_usersession_groupe->setEnabled(true);
			break;
	}
}

void MainWindow::on_clear_clicked()
{
	clearView();
}

void MainWindow::on_actionSave_triggered()
{
	QJsonObject cookie;
	cookie["browser"] = ui->cookiesetting_browserCombo->currentIndex();
	cookie["user_session"] = ui->cookiesetting_usersession->text();
	cookie["file_name"] = ui->cookiesetting_filename->text();
	cookie["table_name"] = ui->cookiesetting_tablename->text();
	cookie["basedomain"] = ui->cookiesetting_column_basedomain->text();
	cookie["name"] = ui->cookiesetting_column_name->text();
	cookie["value"] = ui->cookiesetting_column_value->text();

	QJsonObject root;
	root["cookie"] = cookie;

	QJsonDocument jsd;
	jsd.setObject(root);

	QFile file("settings.json");
	file.open(QIODevice::WriteOnly);

	QTextStream out(&file);

	out << jsd.toJson().data();

	file.close();
}

void MainWindow::on_actionLoad_triggered()
{
	QFile file("settings.json");
	file.open(QIODevice::ReadOnly | QIODevice::Text);

	QJsonDocument jsd;

	jsd = QJsonDocument::fromJson(file.readAll());

	QJsonObject cookie;
	cookie = jsd.object()["cookie"].toObject();

	ui->cookiesetting_browserCombo->setCurrentIndex(cookie["browser"].toInt());
	ui->cookiesetting_usersession->setText(cookie["user_session"].toString());
	ui->cookiesetting_filename->setText(cookie["file_name"].toString());
	ui->cookiesetting_tablename->setText(cookie["table_name"].toString());
	ui->cookiesetting_column_basedomain->setText(cookie["basedomain"].toString());
	ui->cookiesetting_column_name->setText(cookie["name"].toString());
	ui->cookiesetting_column_value->setText(cookie["value"].toString());

	file.close();
}
