#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::onReceiveStarted()
{
	qDebug() << "-----------st-------------";

	QWidget::setWindowTitle(nicolivemanager->nowWaku.getTitle() + " - Viqo");

	// set audiences num timer
	elapsed_time_timer = new QTimer(this);
	elapsed_time_timer->setInterval(1000);
	elapsed_time_timer->start();
	connect(elapsed_time_timer,SIGNAL(timeout()),this,SLOT(updateElapsedTime()));

	// set audiences num timer
	getWatchCount();
	watch_count_timer = new QTimer(this);
	watch_count_timer->setInterval(60000);
	watch_count_timer->start();
	connect(watch_count_timer,SIGNAL(timeout()),this,SLOT(getWatchCount()));
}

void MainWindow::onReceiveEnded()
{
	qDebug() << "-----------ed-------------";

	QWidget::setWindowTitle("Viqo");

	// delete audiences num timer
	elapsed_time_timer->stop();
	elapsed_time_timer->deleteLater();
	ui->elapsed_time->setText("00:00:00");

	// delete audiences num timer
	watch_count_timer->stop();
	watch_count_timer->deleteLater();
}

void MainWindow::getWatchCount()
{
	const QString& broad_id = ui->housouId->text();

	nicolivemanager->getHeartBeatAPI(broad_id);
}

void MainWindow::updateElapsedTime()
{
	QTime el_time(QTime(0,0).addSecs(QDateTime::currentDateTimeUtc().toTime_t() -
			nicolivemanager->nowWaku.getSt().toTime_t()));

	ui->elapsed_time->setText(el_time.toString("hh:mm:ss"));
}

void MainWindow::setHousouID(QString text)
{
	ui->housouId->setText(text);
}

void MainWindow::reflashLiveWaku()
{
	ui->live_waku_list->clear();
	int now_no;
	for(int i = 0; i < nicolivemanager->liveWakuList.size(); ++i) {
		if (nicolivemanager->liveWakuList.at(i)->getBroadID() == nicolivemanager->nowWaku.getBroadID())
			now_no = i;
		ui->live_waku_list->addItem(nicolivemanager->liveWakuList.at(i)->getTitle());
	}

	ui->live_waku_list->setCurrentIndex(now_no);
}

void MainWindow::setWatchCount()
{
	ui->num_audience->setText("来場者数: " + nicolivemanager->getWatchCount());
}

bool MainWindow::isCheckedAutoGettingUserName()
{
	return ui->auto_getting_user_name->isChecked();
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

QTreeWidgetItem* MainWindow::insComment(int num, QString prem, QString user, QString comm, QString date, bool is_184)
{
	QStringList ls;

	ls += QString::number(num);
	ls += prem;
	ls += user;
	ls += comm;
	ls += date;
	ls += user;
	ls += is_184?"@":"";

	QTreeWidgetItem* item = new QTreeWidgetItem(ls);
	ui->commentView->insertTopLevelItem(0, item);
	return item;
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
	setting_commentCommand(""),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->cookiesetting_usersession->setEchoMode(QLineEdit::Password);
	ui->userdata_mail->setEchoMode(QLineEdit::Password);
	ui->userdata_pass->setEchoMode(QLineEdit::Password);


//	if ( ui->cookiesetting_browserCombo->currentIndex() == 0 )
//		getUserSession();

	on_actionLoad_triggered();

	try {
		userManager = new UserManager(this);
		nicolivemanager = new NicoLiveManager(this, this);
	} catch(QString e) {
		qDebug() << e;
	}

	const QString mail = ui->userdata_mail->text();
	const QString pass = ui->userdata_pass->text();
	nicolivemanager->loginAlertAPI(mail, pass);

	on_user_data_OK_clicked();
	QTimer::singleShot(30000, this, SLOT(on_user_data_OK_clicked()));
	QTimer::singleShot(60000, this, SLOT(on_user_data_OK_clicked()));
}

MainWindow::~MainWindow()
{
	on_disconnect_clicked();
	delete ui;
}

QString MainWindow::getUserSession(){
	return ui->cookiesetting_usersession->text();
}

void MainWindow::on_receive_clicked()
{
	if ( getUserSession().isEmpty() ) return;

	on_disconnect_clicked();
	on_clear_clicked();

	nicolivemanager->nowWaku.setBroadID(ui->housouId->text());
	nicolivemanager->broadStart();
}


void MainWindow::on_disconnect_clicked()
{
	nicolivemanager->broadDisconnect();
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
	ui->commentView->clear();
	ui->logtext->clear();
	ui->comment_view->clear();
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

	QJsonObject other;
	other["auto_getting_user_name"] = ui->auto_getting_user_name->isChecked();

	other["comment_command_check"] = ui->setting_commentComand_checkbox->isChecked();
	other["comment_command"] = ui->setting_commentComand->text();

	QJsonObject user_data;
	user_data["mail"] = ui->userdata_mail->text();
	user_data["pass"] = ui->userdata_pass->text();


	QJsonObject root;
	root["cookie"] = cookie;
	root["other"] = other;
	root["user_data"] = user_data;

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
	if ( !file.exists() ) return;

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


	QJsonObject other;
	other = jsd.object()["other"].toObject();
	ui->auto_getting_user_name->setChecked(other["auto_getting_user_name"].toBool());
	ui->setting_commentComand->setText(other["comment_command"].toString());
	ui->setting_commentComand_checkbox->setChecked(other["comment_command_check"].toBool());

	QJsonObject user_data;
	user_data = jsd.object()["user_data"].toObject();
	ui->userdata_mail->setText(user_data["mail"].toString());
	ui->userdata_pass->setText(user_data["pass"].toString());

	file.close();
}

void MainWindow::on_setting_commentComand_checkbox_stateChanged(int st)
{
	bool fl = st==0?false:true;
	ui->setting_commentComand->setEnabled( fl );
	setting_commentCommand = fl?(ui->setting_commentComand->text()):"";
}

void MainWindow::on_setting_apply_clicked()
{
	if (ui->setting_commentComand_checkbox->isChecked()) {
		setting_commentCommand = ui->setting_commentComand->text();
	} else {
		setting_commentCommand = "";
	}
}

void MainWindow::on_commentView_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (column == 2) {
		QString userid = item->text(5);
		userManager->getUserName(item, userid, true, false);
	}
}

void MainWindow::on_cookiesetting_usersession_textChanged()
{
}

void MainWindow::on_commentView_currentItemChanged(QTreeWidgetItem *current)
{
	if (current == NULL) return;


	QString commentView;
	if (current->text(6) == "@") {
		commentView += current->text(2);
	} else {
		commentView += "<a href=\"http://www.nicovideo.jp/user/"+current->text(5)+"\">"+current->text(2)+"</a>";
	}

	commentView += "<pre style=\"white-space:normal;\">"+current->text(3).toHtmlEscaped()+"</pre>";

	ui->comment_view->setHtml( commentView );
}

bool MainWindow::isNextWaku()
{
	return ui->auto_next_waku->isChecked();
}

void MainWindow::on_live_waku_list_activated(int index)
{
	ui->housouId->setText(nicolivemanager->liveWakuList.at(index)->getBroadID());
	on_receive_clicked();
}

void MainWindow::on_user_data_OK_clicked()
{
	nicolivemanager->getRawMyLiveHTML();
}
