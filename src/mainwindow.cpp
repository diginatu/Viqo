#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::onReceiveStarted()
{
	qDebug() << "--comment receiving started--";

	QWidget::setWindowTitle(nicolivemanager->nowWaku.getTitle() + " - Viqo");

	// set audiences num timer
	elapsed_time_timer = new QTimer(this);
	elapsed_time_timer->setInterval(1000);
	elapsed_time_timer->start();
	connect(elapsed_time_timer,SIGNAL(timeout()),this,SLOT(updateElapsedTime()));

	// set audiences num timer
	getWatchCount();
	watch_count_timer = new QTimer(this);
  connect(watch_count_timer,SIGNAL(timeout()),this,SLOT(getWatchCount()));
  watch_count_timer->setInterval(60000);
	watch_count_timer->start();
}

void MainWindow::onReceiveEnded()
{
	qDebug() << "--comment receiving ended--";

	QWidget::setWindowTitle("Viqo");

	// delete audiences num timer
	elapsed_time_timer->stop();
	elapsed_time_timer->deleteLater();
	ui->elapsed_time->setText("00:00:00");

	// delete audiences num timer
	watch_count_timer->stop();
	watch_count_timer->deleteLater();
}

QString MainWindow::getCookieName()
{
  return ui->cookiesetting_filename->text();
}

void MainWindow::getWatchCount()
{
	nicolivemanager->getHeartBeatAPI();
}

bool MainWindow::isCommandCommentChecked() {
  return ui->command_comment_chk->isChecked();
}
QString MainWindow::getCommandComment() {
  return ui->command_comment->text();
}

bool MainWindow::isCommandNextWakuChecked() {
  return ui->command_nextWaku_chk->isChecked();
}
QString MainWindow::getCommandNextWaku() {
  return ui->command_nextWaku->text();
}

void MainWindow::setWatchCount(QString num)
{
	ui->num_audience->setText("来場者数: " + num);
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

void MainWindow::refleshLiveWaku()
{
	ui->live_waku_list->clear();
  int now_no = -1;
	for(int i = 0; i < nicolivemanager->liveWakuList.size(); ++i) {
		if (nicolivemanager->liveWakuList.at(i)->getBroadID() == nicolivemanager->nowWaku.getBroadID())
			now_no = i;
		ui->live_waku_list->addItem(nicolivemanager->liveWakuList.at(i)->getTitle());
	}

  if (now_no != -1)
    ui->live_waku_list->setCurrentIndex(now_no);
}

void MainWindow::insLog(QString log)
{
  ui->logtext->append(log);
}

void MainWindow::insComment(int num, QString prem, QString user, QString comm, QString date, bool is_184, bool after_open)
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

	if ( ui->auto_getting_user_name_chk->isChecked() && (user!="放送主") ) {
		// use HTTP connection for only received comment after started.
		userManager->getUserName(item, user, after_open);
	}

	if (after_open && ui->keep_top_chk->isChecked()) {
		ui->commentView->setCurrentItem(item);
	}
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

	ui->cookiesetting_usersession->setEchoMode(QLineEdit::Password);
	ui->userdata_mail->setEchoMode(QLineEdit::Password);
	ui->userdata_pass->setEchoMode(QLineEdit::Password);

  QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dirs.empty()) {
    throw QString("save directory is not found");
  }
  QDir dir(dirs[0]);
  if (!dir.exists()) {
    if (!dir.mkpath(dirs[0])) throw QString("making save path failed");
  }

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

  nicolivemanager->getRawMyLiveHTML();
  QTimer::singleShot(30000, nicolivemanager, SLOT(getRawMyLiveHTML()));
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
	bodyClear();

	nicolivemanager->nowWaku.setBroadID(ui->housouId->text());
	nicolivemanager->broadStart();
}


void MainWindow::on_disconnect_clicked()
{
	setWatchCount("0");
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
	bodyClear();
	ui->logtext->clear();
}

void MainWindow::bodyClear()
{
	ui->commentView->clear();
	ui->comment_view->clear();
}

void MainWindow::on_actionSave_triggered()
{
	QJsonObject cookie;
	cookie["browser"] = ui->cookiesetting_browserCombo->currentIndex();
	cookie["user_session"] = ui->cookiesetting_usersession->text();
	cookie["file_name"] = ui->cookiesetting_filename->text();

	QJsonObject other;
	other["auto_getting_user_name"] = ui->auto_getting_user_name_chk->isChecked();
	other["keep_top_comment"] = ui->keep_top_chk->isChecked();

	QJsonObject command;
	command["comment_check"] = ui->command_comment_chk->isChecked();
	command["comment"] = ui->command_comment->text();
  command["nextWaku_check"] = ui->command_nextWaku_chk->isChecked();
  command["nextWaku"] = ui->command_nextWaku->text();

	QJsonObject user_data;
	user_data["mail"] = ui->userdata_mail->text();
	user_data["pass"] = ui->userdata_pass->text();

	QJsonObject root;
	root["cookie"] = cookie;
	root["other"] = other;
	root["command"] = command;
	root["user_data"] = user_data;

	QJsonDocument jsd;
	jsd.setObject(root);

  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    throw QString("save directory is not found");
  }
  QFile file(dir[0] + "/settings.json");
	file.open(QIODevice::WriteOnly);

	QTextStream out(&file);

	out << jsd.toJson().data();

	file.close();
}

void MainWindow::on_actionLoad_triggered()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    throw QString("save directory is not found");
  }
  QFile file(dir[0] + "/settings.json");

	if ( !file.exists() ) return;

	file.open(QIODevice::ReadOnly | QIODevice::Text);

	QJsonDocument jsd;

	jsd = QJsonDocument::fromJson(file.readAll());

	QJsonObject cookie;
	cookie = jsd.object()["cookie"].toObject();
	ui->cookiesetting_browserCombo->setCurrentIndex(cookie["browser"].toInt());
	ui->cookiesetting_usersession->setText(cookie["user_session"].toString());
	ui->cookiesetting_filename->setText(cookie["file_name"].toString());

	QJsonObject other;
	other = jsd.object()["other"].toObject();
	ui->auto_getting_user_name_chk->setChecked(other["auto_getting_user_name"].toBool());
	ui->keep_top_chk->setChecked(other["keep_top_comment"].toBool());

	QJsonObject command;
	command = jsd.object()["command"].toObject();
	ui->command_comment->setText(command["comment"].toString());
	ui->command_comment_chk->setChecked(command["comment_check"].toBool());
  ui->command_nextWaku->setText(command["nextWaku"].toString());
  ui->command_nextWaku_chk->setChecked(command["nextWaku_check"].toBool());

	QJsonObject user_data;
	user_data = jsd.object()["user_data"].toObject();
	ui->userdata_mail->setText(user_data["mail"].toString());
	ui->userdata_pass->setText(user_data["pass"].toString());

	file.close();
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

	QString commentView("");
	commentView += "<html><head /><body>";

	commentView += "<table width=\"100%\" border=\"0\"><tr>";

	commentView += "<td>";
	if (current->text(6) == "@") {
		commentView += current->text(2);
	} else {
		commentView += "<a href=\"http://www.nicovideo.jp/user/"+current->text(5)+"\">"+current->text(2)+"</a>";
	}
	commentView += "</td>";

	commentView += "<td align=\"right\">";
	commentView += current->text(4);
	commentView += "</td>";

	commentView += "</tr></table>";

	commentView += "<pre>"+current->text(3).toHtmlEscaped()+"</pre>";

	commentView += "</body></html>";

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
