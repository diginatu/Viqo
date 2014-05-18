#include "mainwindow.h"
#include "ui_mainwindow.h"

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

QTreeWidgetItem* MainWindow::insComment(int num, QString prem, QString user, QString comm, QString date)
{
	QStringList ls;

	ls += QString::number(num);
	ls += prem;
	ls += user;
	ls += comm;
	ls += date;

	QTreeWidgetItem* item = new QTreeWidgetItem(ls);
	ui->commentView->insertTopLevelItem(0, item);
	return item;
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

void MainWindow::getRawMyLiveHTML(QString session_id)
{
	mManager = new QNetworkAccessManager(this);

	// make request
	QNetworkRequest rq;
	QVariant postData = makePostData(session_id);
	rq.setHeader(QNetworkRequest::CookieHeader, postData);
	rq.setUrl(QUrl("http://www.nicovideo.jp/my/live"));

	reply = mManager->get(rq);
	connect(reply,SIGNAL(finished()),this,SLOT(rawMyLivefinished()));
}
/**
 * マイページのHTMLを解析して、自分が参加しているコミュリスト一覧を作成します。
 * @brief MainWindow::rawMyLivefinished
 */
void MainWindow::rawMyLivefinished(){
	QByteArray repdata = reply->readAll();

	ui->broad_list->clear();
	broadIDList.clear();

    //cap(1)はコミュID、cap(2)は配信ID,cap(3)はタイトル
    QRegExp rx("<a href=\"http://com.nicovideo.jp/community/(co\\d+)\">.*<h5><a href=\"http://live.nicovideo.jp/watch/(lv\\d+)\\?ref=zero_mysubscribe\">(.*)</a></h5>");
    rx.setMinimal(true);
    int currentIndex=0;
    insLog("rawMyLivefinished");
    QList<LiveData*> tmpDataList;

    while ((currentIndex=rx.indexIn(QString(repdata),currentIndex))!=-1){
        // 見つけた文字列分だけずらす
        currentIndex+=rx.cap(0).length();
        //内部のデータ構造に格納　メモリ解放をしないといけないかも・・？
        //３つ目のデータは将来的に利用予定
        LiveData *data=new LiveData(rx.cap(2),rx.cap(3),rx.cap(1));
        tmpDataList.append(data);
    }
    //現在見ている番組を一番上にしたい
    if (NULL!=currentSelectLive){
        for (int i=0;i<tmpDataList.length();i++){
            //文字列が一緒がどうか・・Java勢でごめんなさい
            //同じコミュニティなら選択する
            //配信IDじゃなくていいのかな・・？
            LiveData *data=tmpDataList.at(i);
            if (currentSelectLive->getCommunityID().compare(data->getCommunityID())==0){
                ui->broad_list->addItem(data->getTitle());
                broadIDList.append(data);
                //コミュIDが同じで配信IDが異なる場合、新しい配信があると判断
                if (currentSelectLive->getLiveID().compare(data->getLiveID())!=0){
                    //このIDで自動的に再接続
                    on_broad_list_activated(0);
                }

                break;
            }
        }
    }

    for (int i=0;i<tmpDataList.length();i++){
        LiveData *data=tmpDataList.at(i);
        //文字列が一緒がどうか・・Java勢でごめんなさい

        if (NULL!=currentSelectLive){
            if (currentSelectLive->getCommunityID().compare(data->getCommunityID())==0){
                //すでに上で挿入済み
                continue;
            }
        }
        ui->broad_list->addItem(data->getTitle());
        broadIDList.append(data);
    }
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

	StrAbstractor commTcpi(repdata);

	addr = commTcpi.midStr("<addr>", "</addr>");
	port = commTcpi.midStr("<port>", "</port>").toInt();
	thread = commTcpi.midStr("<thread>", "</thread>");

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
	setting_commentCommand(""),
	commtcp(NULL),
	ui(new Ui::MainWindow),
	currentSelectLive(NULL)
{
	ui->setupUi(this);
	ui->statusBar->showMessage("来場者数: 0");

	ui->cookiesetting_usersession->setEchoMode(QLineEdit::Password);
	ui->commentView->setWordWrap(true);

	if ( ui->cookiesetting_browserCombo->currentIndex() == 0 )
		getUserSession();

	on_actionLoad_triggered();

	try {
		userManager = new UserManager(this);
	} catch(QString e) {
		qDebug() << e;
	}
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

QString MainWindow::getUserSession(){
	return ui->cookiesetting_usersession->text();
}

void MainWindow::on_receive_clicked()
{
	const QString userSession = getUserSession();
	const QString broad_id = ui->housouId->text();

	if ( userSession.isEmpty() ) return;

	on_disconnect_clicked();
	on_clear_clicked();

	getAPI(userSession, broad_id);
}

void MainWindow::on_pushButton_2_clicked()
{
	const QString userSession = getUserSession();
	const QString broad_id = ui->housouId->text();
	getHeartBeatAPI(userSession,broad_id);
}

void MainWindow::on_disconnect_clicked()
{
	if (commtcp != NULL && commtcp->isConnected() ) {
		commtcp->close();
		commtcp->deleteLater();
		commtcp = NULL;
	}
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

	QJsonObject root;
	root["cookie"] = cookie;
	root["other"] = other;

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

	file.close();

	afterLoad();
}

void MainWindow::afterLoad()
{
	on_mylive_reflesh_clicked();

	liveDataReloadtimer = new QTimer(this); //タイマー
	liveDataReloadtimer->setInterval(60000);
	liveDataReloadtimer->start();

	connect(liveDataReloadtimer,SIGNAL(timeout()),this,SLOT(on_mylive_reflesh_clicked()));
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

void MainWindow::on_broad_list_activated(int index)
{
    currentSelectLive=broadIDList.at(index);
    ui->housouId->setText(currentSelectLive->getLiveID());

	on_receive_clicked();
}

void MainWindow::on_mylive_reflesh_clicked()
{
	QString session = getUserSession();

	if (!session.isEmpty()){
		getRawMyLiveHTML(session);
	}
}

void MainWindow::on_commentView_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (column == 2) {
		QString userid = item->text(2);
		getUserNameAndSet(item, userid);
	}
}

void MainWindow::getUserNameAndSet(QTreeWidgetItem* item, QString userid)
{
	bool isAllNum = true;
	for ( int i = 0; i < userid.length(); ++i)
		if ( !userid.at(i).isNumber() )
			isAllNum = false;

	if ( isAllNum )
		userManager->getUserName(item, userid);
}
