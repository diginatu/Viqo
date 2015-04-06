#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  settingsWindow(new SettingsWindow(this, this)),
  newWakuSettingsWindow(new NewWakuSettingsWindow(this, this)),
  accountWindow(new AccountWindow(this, this)),
  followCommunity(new FollowCommunity(this, this)),
  userSessionDisabledDialogAppeared(false),
  isCursorTop(true),
  settings(this, ui, this)
{
  ui->setupUi(this);
  setAcceptDrops(true);

  this->setWindowIcon(QIcon(":/img/icon.svg"));

  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

  // set the column width for the comment view list
  ui->comment_view->header()->resizeSection(0, 40);
  ui->comment_view->header()->resizeSection(1, 30);
  ui->comment_view->header()->resizeSection(3, 200);
  ui->comment_view->header()->resizeSection(6, 30);
  ui->comment_view->header()->resizeSection(7, 30);

  ui->comment_view->insertAction(0,ui->CommentViewEditKotehan);
  ui->comment_view->insertAction(0,ui->CommentViewGetKotehan);
  ui->one_comment_view->insertAction(0,ui->oneCommentActionCopy);
  ui->one_comment_view->insertAction(0,ui->oneCommentActionSearchByGoogle);

  QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dirs.empty()) {
    insLog("save directory is not found");
    QMessageBox::information(this, "Viqo", QStringLiteral("アプリケーション保存領域がありません"));
  } else {
    QDir dir(dirs[0]);
    if (!dir.exists()) {
      if (!dir.mkpath(dirs[0])) {
        insLog("making save path failed");
        QMessageBox::information(this, "Viqo", QStringLiteral("保存ディレクトリの作成に失敗しました"));
      }
    }
  }

  settings.loadAll();

  userManager = new UserManager(this);
  nicolivemanager = new NicoLiveManager(this, accountWindow, newWakuSettingsWindow, followCommunity, this);

  const QString mail = settings.getUserMail();
  const QString pass = settings.getUserPass();
  if ( mail != "" && pass != "") {
    nicolivemanager->loginAlertAPI(mail, pass);
  }

  nicolivemanager->getRawMyLiveHTML();
  QTimer::singleShot(30000, nicolivemanager, SLOT(getRawMyLiveHTML()));

  newWakuSettingsWindow->loadPresets();
}

MainWindow::~MainWindow()
{
  nicolivemanager->broadDisconnect();
  delete ui;
}

void MainWindow::onReceiveStarted()
{
  qDebug() << "--comment receiving started--";

  ui->submit_button->setEnabled(true);
  ui->disconnect->setEnabled(true);
  ui->openBrowser->setEnabled(true);

  QWidget::setWindowTitle(nicolivemanager->nowWaku.getTitle() + " - Viqo");

  // set audiences num timer
  elapsed_time_timer = new QTimer(this);
  connect(elapsed_time_timer,SIGNAL(timeout()),this,SLOT(updateElapsedTime()));
  elapsed_time_timer->start(1000);

  // set audiences num timer
  getWatchCount();
  watch_count_timer = new QTimer(this);
  connect(watch_count_timer,SIGNAL(timeout()),this,SLOT(getWatchCount()));
  watch_count_timer->start(60000);
}

void MainWindow::onReceiveEnded()
{
  qDebug() << "--comment receiving ended--";

  ui->submit_button->setEnabled(false);
  ui->disconnect->setEnabled(false);
  ui->openBrowser->setEnabled(false);

  QWidget::setWindowTitle("Viqo");

  // delete audiences num timer
  elapsed_time_timer->stop();
  elapsed_time_timer->deleteLater();
  ui->elapsed_time->setText("00:00:00");

  // delete audiences num timer
  watch_count_timer->stop();
  watch_count_timer->deleteLater();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
  if (event->mimeData()->hasText())
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
  event->acceptProposedAction();
  ui->broadID->setText(event->mimeData()->text());
  on_receive_clicked();
}

void MainWindow::getWatchCount()
{
  nicolivemanager->getHeartBeatAPI();
}

void MainWindow::setWatchCount(QString num)
{
  ui->num_audience->setText(QStringLiteral("来場者数: ") + num);
}

void MainWindow::updateElapsedTime()
{
  const QDateTime st = nicolivemanager->nowWaku.getSt();
  const QDateTime ed = nicolivemanager->nowWaku.getEd();
  const QDateTime nw = QDateTime::currentDateTimeUtc();

  if (!nicolivemanager->nowWaku.didAlermCommand &&
      ui->command_beforeEnd_chk->isChecked()) {
    const QTime alerm(0,ui->command_beforeEndMinuts_spn->value());
    if (QTime(0,0).addSecs(ed.toTime_t() - nw.toTime_t()) <= alerm) {

      nicolivemanager->nowWaku.didAlermCommand = true;

      QProcess pr;
      QString cmd = ui->command_beforeEnd->text();

      const QString lastTime = QString::number(ui->command_beforeEndMinuts_spn->value());
      cmd.replace("%lastTime%",'"' + lastTime + '"');

      pr.start(cmd);
      pr.waitForFinished(5000);
    }
  }

  const QTime el_time(QTime(0,0).addSecs(nw.toTime_t() - st.toTime_t()));
  ui->elapsed_time->setText(el_time.toString("hh:mm:ss"));
}

void MainWindow::setHousouID(QString text)
{
  ui->broadID->setText(text);
}

void MainWindow::refleshLiveWaku()
{
  ui->live_waku_list->clear();
  int now_no = -1;
  for(int i = 0; i < nicolivemanager->liveWakuList.size(); ++i) {
    if (nicolivemanager->liveWakuList.at(i)->getBroadID() == nicolivemanager->nowWaku.getBroadID())
      now_no = i;
    ui->live_waku_list->addItem(
                nicolivemanager->liveWakuList.at(i)->getTitle() + " - " +
                nicolivemanager->liveWakuList.at(i)->getOwnerName() + QStringLiteral("さん")
                );
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
  ls += broadcaster?QStringLiteral("放送主"):user;
  ls += comm.replace('\n', QChar(8629));
  ls += date;
  ls += user;
  ls += is_184?"@":"";
  ls += broadcaster?"@":"";

  QTreeWidgetItem* item = new QTreeWidgetItem(ls);
  ui->comment_view->insertTopLevelItem(0, item);

  // get user name
  if (!broadcaster && !is_184) {
    userManager->getUserName(item, user, false);
    if (after_open && settings.isAutoGettingUserName() &&
        (settings.isAutoGetUserNameOverWrite() || item->text(2) == user)) {
      if (settings.isAutoGetUserNameUseAt()) {
        // at-mark Kotehan
        const QRegExp kotehanrg("[@＠]\\s*(\\S+)");
        if (kotehanrg.indexIn(comm) != -1) {
          userManager->setUserName(item, kotehanrg.cap(1));
        } else if (settings.isAutoGetUserNameUsePage() &&
                   item->text(2) == user) {
          // use pgae to get the name
          userManager->getUserName(item, user, true);
        }
      } else if (settings.isAutoGetUserNameUsePage() &&
                 item->text(2) == user) {
        userManager->getUserName(item, user, true);
      }
    }
  }

  // comment command
  if ( after_open && settings.isCommandCommentChecked() ) {
    QProcess pr;

    QString cmd = settings.getCommandComment();
    cmd.replace("%userID%",user);
    cmd.replace("%commentNo%",item->text(0));
    QString esuser = user;
    esuser.replace("\"", "\"\"\"");
    cmd.replace("%userName%",item->text(2) == user?"":item->text(2));
    QString escmd = comm;
    escmd.replace("\"", "\"\"\"");
    cmd.replace("%comment%",escmd);

    pr.start(cmd);
    pr.waitForFinished(5000);
  }

  if (after_open && ui->keepTop->isChecked() && isCursorTop) {
    ui->comment_view->setCurrentItem(item);
  }
}

int MainWindow::lastCommentNum()
{
  QTreeWidgetItem* topitem = ui->comment_view->topLevelItem(0);
  if(topitem == 0) return 0;
  return topitem->text(0).toInt();
}

void MainWindow::getSessionFromCookie(QString cookie_name)
{
  CookieRead cr(this);
  settings.setUserSession(cr.getUserSession(cookie_name));
}

void MainWindow::on_receive_clicked()
{
  if ( settings.getUserSession().isEmpty() ) {
    insLog("MainWindow::on_receive_clicked sessionID is not set yet");
    QMessageBox::information(this, "Viqo", QStringLiteral("セッションIDが設定されていません"));
    return;
  }

  // trim into only broad number
  const QRegExp broadIDrg("lv(\\d+)");
  QString broadID = ui->broadID->text();
  if (broadIDrg.indexIn(broadID) != -1) {
    broadID = broadIDrg.cap(1);
  }
  ui->broadID->setText(broadID);

  nicolivemanager->broadDisconnect();
  bodyClear();

  nicolivemanager->nowWaku.setBroadID(broadID);
  nicolivemanager->broadStart();
}


void MainWindow::on_disconnect_clicked()
{
  nicolivemanager->broadDisconnect(true);
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

void MainWindow::on_comment_view_currentItemChanged(QTreeWidgetItem *current)
{
  if (current == NULL) {
    isCursorTop = true;
    return;
  }

  if (ui->comment_view->currentIndex().row() >= 1) {
    isCursorTop = false;
  } else {
    isCursorTop = true;
  }

  const bool owner = current->text(7)=="@"?true:false;

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

  QString comme = current->text(3);
  const QRegExp urlrg("(https?://[\\w/:%#\\$&\\?\\(\\)~\\.=\\+\\-]+)");

  if (!owner) {
    comme = comme.toHtmlEscaped();
    QString tcomme;
    int bpos = 0;
    int pos = 0;
    while ((pos = urlrg.indexIn(comme, pos)) != -1) {
      tcomme += comme.mid(bpos,pos-bpos).toHtmlEscaped();
      tcomme += "<a href=\"" + urlrg.cap(1) + "\">" + urlrg.cap(1).toHtmlEscaped() + "</a>";
      pos += urlrg.matchedLength();
      bpos = pos;
    }
    tcomme += comme.mid(bpos);
    comme = tcomme;
  }
  comme.replace(QChar(8629), "<br>");

  comment_view += "<p>"+comme+"</p>";

  comment_view += "</body></html>";

  ui->one_comment_view->setHtml( comment_view );
}

void MainWindow::on_live_waku_list_activated(int index)
{
  ui->broadID->setText(nicolivemanager->liveWakuList.at(index)->getBroadID());
  on_receive_clicked();
}

void MainWindow::on_setting_triggered()
{
  settingsWindow->init();
  settingsWindow->show();
  settingsWindow->raise();
  settingsWindow->activateWindow();
}

void MainWindow::on_AccountSettings_triggered()
{
  accountWindow->init();
  accountWindow->show();
  accountWindow->raise();
  accountWindow->activateWindow();
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
  if (ui->submit_button->isEnabled())
    on_submit_button_clicked();
}

void MainWindow::on_openBrowser_clicked()
{
  if (!ui->openBrowser->isEnabled()) return;

  QDesktopServices::openUrl("http://live.nicovideo.jp/watch/lv" + ui->broadID->text());
}

void MainWindow::on_oneCommentActionSearchByGoogle_triggered()
{
  const QString url = "https://www.google.co.jp/search?q=" +
      ui->one_comment_view->textCursor().selectedText();
  QDesktopServices::openUrl(url);
}

void MainWindow::on_oneCommentActionCopy_triggered()
{
  QApplication::clipboard()->setText(ui->one_comment_view->textCursor().selectedText());
}

void MainWindow::on_command_test_button_clicked()
{
  QProcess pr;
  QString cmd = ui->command_test->text();

  cmd.replace("%wakuURL%","http://live.nicovideo.jp/watch/lv" + nicolivemanager->nowWaku.getBroadID());

  pr.start(cmd);
  pr.waitForFinished(5000);
}

void MainWindow::on_save_1_triggered()
{
  settings.saveStatus();
}
void MainWindow::on_save_2_triggered()
{
  settings.saveStatus(2);
}
void MainWindow::on_save_3_triggered()
{
  settings.saveStatus(3);
}
void MainWindow::on_save_4_triggered()
{
  settings.saveStatus(4);
}

void MainWindow::on_load_1_triggered()
{
  settings.loadStatus();
}
void MainWindow::on_load_2_triggered()
{
  settings.loadStatus(2);
}
void MainWindow::on_load_3_triggered()
{
  settings.loadStatus(3);
}
void MainWindow::on_load_4_triggered()
{
  settings.loadStatus(4);
}

void MainWindow::getNewWakuAPI(int type, QString livenum)
{
  nicolivemanager->getNewWakuAPI(type, livenum);
}

void MainWindow::userSessionDisabled()
{
  if (userSessionDisabledDialogAppeared) return;
  userSessionDisabledDialogAppeared = true;

  const UserSessionWay usw = settings.getLoginWay();
  if (usw == UserSessionWay::Direct) {
    QMessageBox msgBox(this);
    msgBox.setText(QStringLiteral("ユーザセッションが無効です\n設定画面を開きますか？"));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if (msgBox.exec() == QMessageBox::Ok) {
      on_AccountSettings_triggered();
    }
  } else if (usw == UserSessionWay::Firefox ||
             usw == UserSessionWay::Login) {
    QMessageBox msgBox(this);
    msgBox.setText(QStringLiteral("ユーザセッションが無効です\n取得しなおしますか？"));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if (msgBox.exec() == QMessageBox::Ok) {
      accountWindow->init();
      accountWindow->updateSessionAndSave();
    }
  }

  userSessionDisabledDialogAppeared = false;
  return;
}

void MainWindow::deleteCommunityFromList(QString communityID)
{
  LiveWaku* listElement(nullptr);

  foreach (LiveWaku* awaku, nicolivemanager->liveWakuList) {
    if (awaku->getCommunity() == communityID) {
      listElement = awaku;
      break;
    }
  }

  if (listElement != nullptr) {
    nicolivemanager->liveWakuList.removeOne(listElement);
    refleshLiveWaku();
    listElement->deleteLater();
  }
}

void MainWindow::on_autoNewWakuSettings_triggered()
{
  newWakuSettingsWindow->show();
  newWakuSettingsWindow->raise();
  newWakuSettingsWindow->activateWindow();
}

void MainWindow::on_getNewWakuNow_triggered()
{
  nicolivemanager->broadDisconnect();
  nicolivemanager->nowWaku.setCommunity(newWakuSettingsWindow->getCommunity());
  getNewWakuAPI(2);
}

void MainWindow::on_quit_triggered()
{
  QApplication::quit();
}

void MainWindow::on_AboutViqo_triggered()
{
  QMessageBox::about(this, QStringLiteral("About Viqo"),
         QCoreApplication::applicationName() + " version " +
         QCoreApplication::applicationVersion() +
         QStringLiteral("<br>\
            Qt で作成されたマルチプラットフォームコメントビューワです<br>\
            <a href=\"https://github.com/diginatu/Viqo\">GitHub Viqo repository</a>"));
}

void MainWindow::on_AboutQt_triggered()
{
  QMessageBox::aboutQt(this);
}

void MainWindow::on_FollowCommunity_triggered()
{
  followCommunity->init();
  followCommunity->show();
  followCommunity->raise();
  followCommunity->activateWindow();


  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    insLog("save directory is not available");
    return;
  }
  QFile file(dir[0] + "/follow_communities.json");
  if (!file.exists()) {
    QMessageBox::information(followCommunity, QStringLiteral("Viqo - フォローコミュニティ"),
                             QStringLiteral("<b>フォローコミュニティ</b><br>\
                             ここにコミュニティを登録することで、\
                             お気に入りに登録しているコミュニティ同様、\
                             放送開始時に通知されます。<br>\
                             ただし、コメビュ起動時にすでに放送開始されているフォローコミュニティは\
                             検知することができません。"));
  }
}

void MainWindow::on_comment_view_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
  if (!current ^ !previous) {
    if (current == 0) {
      ui->CommentViewEditKotehan->setEnabled(false);
      ui->CommentViewGetKotehan->setEnabled(false);
    } else {
      ui->CommentViewEditKotehan->setEnabled(true);
      ui->CommentViewGetKotehan->setEnabled(true);
    }
  }
}

void MainWindow::on_CommentViewEditKotehan_triggered()
{
  QTreeWidgetItem* const citem = ui->comment_view->currentItem();

  QString name = QInputDialog::getText(this, QStringLiteral("コテハン編集"),
                             QStringLiteral("コテハン:"), QLineEdit::Normal,
                             citem->text(2));

  if (name.isEmpty()) return;

  userManager->setUserName(citem, name);
}

void MainWindow::on_CommentViewGetKotehan_triggered()
{
  QTreeWidgetItem* const citem = ui->comment_view->currentItem();
  userManager->getUserName(citem, citem->text(5), true, false);
}

void MainWindow::on_autoGettingUserName_toggled(bool status)
{
  ui->autoGetUserNameOverWrite->setEnabled(status);
  ui->autoGetUserWay->setEnabled(status);
}
