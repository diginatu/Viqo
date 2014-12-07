#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  settingsWindow(new SettingsWindow(this, this)),
  settings(this, this),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dirs.empty()) {
    insLog("save directory is not found");
  } else {
    QDir dir(dirs[0]);
    if (!dir.exists()) {
      if (!dir.mkpath(dirs[0]))
        insLog("making save path failed");
    }
  }

  userManager = new UserManager(this);
  nicolivemanager = new NicoLiveManager(this, settingsWindow, this);

  settings.loadSettings();
  settings.loadStatus(ui);

  const QString mail = settings.getUserMail();
  const QString pass = settings.getUserPass();
  if ( mail != "" && pass != "") {
    nicolivemanager->loginAlertAPI(mail, pass);
  }

  nicolivemanager->getRawMyLiveHTML();
  QTimer::singleShot(30000, nicolivemanager, SLOT(getRawMyLiveHTML()));
}

MainWindow::~MainWindow()
{
  on_disconnect_clicked();
  delete ui;
}
void MainWindow::onReceiveStarted()
{
  qDebug() << "--comment receiving started--";

  ui->submit_button->setEnabled(true);
  ui->disconnect->setEnabled(true);

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

  ui->submit_button->setEnabled(false);
  ui->disconnect->setEnabled(false);

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

void MainWindow::insComment(int num, bool prem, QString user,
     QString comm, QString date, bool is_184,
     bool broadcaster, bool after_open)
{
  QStringList ls;

  ls += QString::number(num);
  ls += prem?"@":"";
  ls += user;
  ls += comm.replace("\n", "\\n");
  ls += date;
  ls += user;
  ls += is_184?"@":"";
  ls += broadcaster?"@":"";

  QTreeWidgetItem* item = new QTreeWidgetItem(ls);
  ui->comment_view->insertTopLevelItem(0, item);

  if ( !broadcaster && !is_184 ) {
    // use HTTP connection for only received comment after started.
    userManager->getUserName(item, user, ui->auto_getting_user_name_chk->isChecked() && after_open);
  }

  if (after_open && ui->keep_top_chk->isChecked()) {
    ui->comment_view->setCurrentItem(item);
  }
}

void MainWindow::getSessionFromCookie()
{
  CookieRead cr(this);
  settings.setUserSession(cr.getUserSession());

}

void MainWindow::on_receive_clicked()
{
  if ( settings.getUserSession().isEmpty() ) return;

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

void MainWindow::bodyClear()
{
  ui->comment_view->clear();
  ui->one_comment_view->clear();
}

void MainWindow::submittedComment()
{
  ui->submit_button->setEnabled(true);
}

void MainWindow::on_actionSave_triggered()
{
  settings.saveStatus(ui);
}

void MainWindow::on_actionLoad_triggered()
{
  settings.loadStatus(ui);
}

void MainWindow::on_comment_view_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
  if (column == 2) {
    QString userid = item->text(5);
    userManager->getUserName(item, userid, true, false);
  }
}

void MainWindow::on_comment_view_currentItemChanged(QTreeWidgetItem *current)
{
  if (current == NULL) return;

  QString comment_view("");
  comment_view += "<html><head /><body>";

  comment_view += "<table width=\"100%\" border=\"0\"><tr>";

  comment_view += "<td>";
  if (current->text(6) == "@") {
    comment_view += current->text(2);
  } else {
    comment_view += "<a href=\"http://www.nicovideo.jp/user/"+current->text(5)+"\">"+current->text(2)+"</a>";
  }
  comment_view += "</td>";

  comment_view += "<td align=\"right\">";
  comment_view += current->text(4);
  comment_view += "</td>";

  comment_view += "</tr></table>";

  QString comme = current->text(3).toHtmlEscaped();
  comme.replace("\n", "\n<br>");
  comment_view += "<p>"+comme.replace("\\n", "<br>")+"</p>";

  comment_view += "</body></html>";


  ui->one_comment_view->setHtml( comment_view );
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

void MainWindow::on_setting_triggered()
{
  settingsWindow->init();
  settingsWindow->show();
  settingsWindow->raise();
  settingsWindow->activateWindow();
}

void MainWindow::on_clear_triggered()
{
  bodyClear();
}

void MainWindow::on_submit_button_clicked()
{
  const QString& text = ui->submit_text->text();
  nicolivemanager->nowWaku.sendComment(text);
  QString bl = "";
  ui->submit_text->setText(bl);
  ui->submit_button->setEnabled(false);
}

void MainWindow::on_submit_text_returnPressed()
{
  on_submit_button_clicked();
}
