#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::insComment(int num, QString user, QString comm, QString date)
{
	ui->treeWidget
}

void MainWindow::getAPI(QString user_id, QString broad_id)
{
	mManager = new QNetworkAccessManager(this);
	// make request
	QNetworkRequest rq;

	// make cookies
	QVariant postData;
	QList <QNetworkCookie> cookies;
	QNetworkCookie ck;
	ck.toRawForm(QNetworkCookie::NameAndValueOnly);
	ck.setName("user_session");
	QByteArray user_id_ba;
	user_id_ba.append(user_id);
	ck.setValue(user_id_ba);
	cookies.append(ck);
	postData.setValue(cookies);

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

	commtcp = new CommTcp(addr, port, thread);
	commtcp->doConnect();
}


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->lineEdit->setEchoMode(QLineEdit::Password);

	QList<QTreeWidgetItem *> items;
	QStringList ls;
	for (int i = 0; i < 10; ++i) {
		QStringList ls;
		ls += QString::number(i+1);
		ls += "vivi";
		ls += "com";
		ls += "date";

		items.append(new QTreeWidgetItem((QTreeWidget*)0, ls));
	}

	ui->treeWidget->insertTopLevelItems(0, items);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	const QString user_id = ui->lineEdit->text();
	const QString broad_id = ui->lineEdit_2->text();
	getAPI(user_id, broad_id);
}


