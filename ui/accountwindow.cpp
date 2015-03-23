#include "accountwindow.h"
#include "ui_accountwindow.h"
#include "mainwindow.h"

AccountWindow::AccountWindow(MainWindow* mwin, QWidget* parent) :
  QDialog(parent),
  ui(new Ui::AccountWindow)
{
  ui->setupUi(this);
  this->mwin = mwin;

  ui->userdata_mail->setEchoMode(QLineEdit::Password);
  ui->userdata_pass->setEchoMode(QLineEdit::Password);
  ui->usersession->setEchoMode(QLineEdit::Password);
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
  ui->usersession->setText(mwin->settings.getUserSession());
  ui->cookiesetting_filename->setText(mwin->settings.getCookieFile());
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
  case UserSessionWay::Direct:
    ui->cookie_group->setEnabled(false);
    ui->usersession->setEnabled(true);
    ui->get_session->setEnabled(false);
    break;
  case UserSessionWay::Firefox:
    ui->cookie_group->setEnabled(true);
    ui->usersession->setEnabled(false);
    ui->get_session->setEnabled(true);
    break;
  case UserSessionWay::Login:
    ui->cookie_group->setEnabled(false);
    ui->usersession->setEnabled(false);
    ui->get_session->setEnabled(true);
    break;
  }
}

void AccountWindow::on_buttonBox_accepted()
{
  mwin->settings.setUserMail(ui->userdata_mail->text());
  mwin->settings.setUserPass(ui->userdata_pass->text());
  mwin->settings.setLoginWay(UserSessionWay(ui->login_way_combo->currentIndex()));
  mwin->settings.setUserSession(ui->usersession->text());
  mwin->settings.setCookieFile(ui->cookiesetting_filename->text());

  mwin->settings.saveSettings();

  mwin->nicolivemanager->alertReconnect();
}


void AccountWindow::on_cookiesetting_file_open_button_clicked()
{
  const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Cookies File"), QDir::homePath(), tr("sqlite Files (*.sqlite)"));
  if ( filePath == "" ) return;
  ui->cookiesetting_filename->setText(filePath);
}

void AccountWindow::on_get_session_clicked()
{
  switch (static_cast<UserSessionWay>(ui->login_way_combo->currentIndex())) {
  case UserSessionWay::Firefox:
    mwin->getSessionFromCookie(ui->cookiesetting_filename->text());
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
