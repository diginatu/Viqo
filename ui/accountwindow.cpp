#include "accountwindow.h"
#include "ui_accountwindow.h"
#include "mainwindow.h"

AccountWindow::AccountWindow(MainWindow* mwin, QWidget* parent) :
  QDialog(parent),
  ui(new Ui::AccountWindow),
  nicookie(this)
{
  ui->setupUi(this);
  this->mwin = mwin;

  ui->userdata_mail->setEchoMode(QLineEdit::Password);
  ui->userdata_pass->setEchoMode(QLineEdit::Password);
  ui->usersession->setEchoMode(QLineEdit::Password);

  on_login_way_combo_currentIndexChanged(static_cast<int>(UserSessionWay::Browser));

  ui->browser_combo->addItems(nicookie.getBrowserList());
}

AccountWindow::~AccountWindow()
{
  delete ui;
}

void AccountWindow::init()
{
  ui->userdata_mail->setText(mwin->settings.getUserMail());
  ui->userdata_pass->setText(mwin->settings.getUserPass());
  ui->login_way_combo->setCurrentIndex(static_cast<int>(mwin->settings.getLoginWay()));
  ui->browser_combo->setCurrentText(mwin->settings.getBrowser());
  ui->usersession->setText(mwin->settings.getUserSession());
}

void AccountWindow::getUserSessionFinished()
{
  ui->usersession->setText(mwin->settings.getUserSession());
}

void AccountWindow::updateSessionAndSave()
{
  on_get_session_clicked();
  on_buttonBox_accepted();
}

void AccountWindow::on_login_way_combo_currentIndexChanged(int index)
{
  switch (static_cast<UserSessionWay>(index)) {
  case UserSessionWay::Browser:
    ui->usersession->setEnabled(false);
    ui->get_session->setEnabled(true);
    ui->browser_combo->setEnabled(true);
    ui->userSessionWayDiscription->setText(
          QStringLiteral(u"ブラウザからユーザーセッションを取得します<br>\
                         ブラウザを選択してください"));
    break;
  case UserSessionWay::Direct:
    ui->usersession->setEnabled(true);
    ui->get_session->setEnabled(false);
    ui->browser_combo->setEnabled(false);
    ui->userSessionWayDiscription->setText(
          QStringLiteral(u"ユーザーセッションを直接入力します<br>\
                         選択肢に無いブラウザのセッションidを使う場合などに使用してください。"));
    break;
  case UserSessionWay::Login:
    ui->usersession->setEnabled(false);
    ui->get_session->setEnabled(true);
    ui->browser_combo->setEnabled(false);
    ui->userSessionWayDiscription->setText(
          QStringLiteral(u"Viqoからログインしてユーザーセッションを取得します<br>\
                         ニコ生のログインセッションを一つ消費するので、\
                         他のブラウザなどがログアウトされる可能性があります。<br>\
                         <b>上のメールとパスワードを入力したあと、必ず取得ボタンを押してください。</b>"));
    break;
  }
}

void AccountWindow::on_buttonBox_accepted()
{
  mwin->settings.setUserMail(ui->userdata_mail->text());
  mwin->settings.setUserPass(ui->userdata_pass->text());
  mwin->settings.setLoginWay(UserSessionWay(ui->login_way_combo->currentIndex()));
  mwin->settings.setBrowser(ui->browser_combo->currentText());
  mwin->settings.setUserSession(ui->usersession->text());

  mwin->settings.saveSettings();

  mwin->nicolivemanager->alertReconnect();
}

void AccountWindow::on_get_session_clicked()
{
  switch (static_cast<UserSessionWay>(ui->login_way_combo->currentIndex())) {
  case UserSessionWay::Browser:
    mwin->settings.setUserSession(
          nicookie.getUserSession(ui->browser_combo->currentText()));

    if (nicookie.hasError()) {
      QString error = nicookie.errorString();
      mwin->insLog("Nicookie : " + error);
      QMessageBox::information(this, "Nicookie", error);
      break;
    }

    QMessageBox::information(this, "Nicookie",
                             QStringLiteral(u"正常にセッションが取得されました"));

    // For Chrome in Mac:
    //   The main window is focused when the Keychain window closes.
    this->raise();

    getUserSessionFinished();
    break;
  case UserSessionWay::Login:
    mwin->nicolivemanager->login(ui->userdata_mail->text(), ui->userdata_pass->text());
    break;
  default:
    Q_ASSERT(false);
    break;
  }
}
