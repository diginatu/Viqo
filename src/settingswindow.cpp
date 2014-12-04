#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "src/mainwindow.h"

SettingsWindow::SettingsWindow(MainWindow* mwin, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsWindow)
{
  ui->setupUi(this);

  this->mwin = mwin;

  ui->userdata_mail->setEchoMode(QLineEdit::Password);
  ui->userdata_pass->setEchoMode(QLineEdit::Password);
  ui->usersession->setEchoMode(QLineEdit::Password);
}

SettingsWindow::~SettingsWindow()
{
  delete ui;
}

void SettingsWindow::init()
{
  ui->userdata_mail->setText(mwin->settings.getUserMail());
  ui->userdata_pass->setText(mwin->settings.getUserPass());
  ui->loginWayCombo->setCurrentIndex(mwin->settings.getLoginWay());
  ui->usersession->setText(mwin->settings.getUserSession());
  ui->cookiesetting_filename->setText(mwin->settings.getCookieFile());
}

void SettingsWindow::on_loginWayCombo_currentIndexChanged(int index)
{
  switch (index) {
  case 0:
    ui->cookie_group->setEnabled(false);
    ui->usersession_groupe->setEnabled(true);
    ui->get_session->setEnabled(false);
    break;
  case 1:
    ui->cookie_group->setEnabled(true);
    ui->usersession_groupe->setEnabled(false);
    ui->get_session->setEnabled(true);
    break;
  case 2:
    ui->cookie_group->setEnabled(false);
    ui->usersession_groupe->setEnabled(false);
    ui->get_session->setEnabled(true);
    break;
  }
}

void SettingsWindow::on_buttonBox_rejected()
{
}

void SettingsWindow::on_buttonBox_accepted()
{
  mwin->settings.setUserMail(ui->userdata_mail->text());
  mwin->settings.setUserPass(ui->userdata_pass->text());
  mwin->settings.setLoginWay(ui->loginWayCombo->currentIndex());
  mwin->settings.setUserSession(ui->usersession->text());
  mwin->settings.setCookieFile(ui->cookiesetting_filename->text());

  mwin->settings.saveSettings();
}


void SettingsWindow::on_cookiesetting_file_open_button_clicked()
{
  QString filePath = QFileDialog::getOpenFileName(this, tr("Open Cookies File"), QDir::homePath(), tr("sqlite Files (*.sqlite)"));
  if ( filePath == "" ) return;
  ui->cookiesetting_filename->setText(filePath);
}

void SettingsWindow::on_get_session_clicked()
{
  switch (ui->loginWayCombo->currentIndex()) {
  case 1:
    mwin->getSessionFromCookie();
    ui->usersession->setText(mwin->settings.getUserSession());
    break;
  case 2:
    mwin->nicolivemanager->login(ui->userdata_mail->text(), ui->userdata_pass->text());
    ui->usersession->setText(mwin->settings.getUserSession());
    break;
  default:
    Q_ASSERT(false);
    break;
  }
}
