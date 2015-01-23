#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "mainwindow.h"

SettingsWindow::SettingsWindow(MainWindow* mwin, QWidget* parent) :
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
  ui->login_way_combo->setCurrentIndex(mwin->settings.getLoginWay());
  ui->usersession->setText(mwin->settings.getUserSession());
  ui->cookiesetting_filename->setText(mwin->settings.getCookieFile());

  ui->comment_ownercomment_chk->setChecked(mwin->settings.getOwnerComment());
  ui->comment_dipspNG_chk->setChecked(mwin->settings.getDispNG());
}

void SettingsWindow::getUserSessionFinished()
{
  ui->usersession->setText(mwin->settings.getUserSession());
  ui->buttonBox->setEnabled(true);
}

void SettingsWindow::on_login_way_combo_currentIndexChanged(int index)
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

void SettingsWindow::on_buttonBox_accepted()
{
  mwin->settings.setUserMail(ui->userdata_mail->text());
  mwin->settings.setUserPass(ui->userdata_pass->text());
  mwin->settings.setLoginWay(ui->login_way_combo->currentIndex());
  mwin->settings.setUserSession(ui->usersession->text());
  mwin->settings.setCookieFile(ui->cookiesetting_filename->text());

  mwin->settings.setOwnerComment(ui->comment_ownercomment_chk->isChecked());
  mwin->settings.setDispNG(ui->comment_dipspNG_chk->isChecked());

  mwin->settings.saveSettings();

  mwin->nicolivemanager->alertReconnect();
}


void SettingsWindow::on_cookiesetting_file_open_button_clicked()
{
  const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Cookies File"), QDir::homePath(), tr("sqlite Files (*.sqlite)"));
  if ( filePath == "" ) return;
  ui->cookiesetting_filename->setText(filePath);
}

void SettingsWindow::on_get_session_clicked()
{
  switch (ui->login_way_combo->currentIndex()) {
  case 1:
    ui->buttonBox->setEnabled(false);
    mwin->getSessionFromCookie(ui->cookiesetting_filename->text());
    getUserSessionFinished();
    break;
  case 2:
    ui->buttonBox->setEnabled(false);
    mwin->nicolivemanager->login(ui->userdata_mail->text(), ui->userdata_pass->text());
    break;
  default:
    Q_ASSERT(false);
    break;
  }
}

void SettingsWindow::on_newWaku_befWakuReuse_clicked()
{
  mwin->getNewWakuAPI(0);
}

void SettingsWindow::setSelectedCategory(const QString& value)
{
  selectedCategory = value;
}

void SettingsWindow::setSelectedCommunity(const QString& value)
{
  selectedCommunity = value;
}


void SettingsWindow::newWakuListStateSave()
{
  selectedCommunity = ui->newWaku_community->currentText();
  selectedCategory = ui->newWaku_category->currentText();

  ui->newWaku_community->clear();
  ui->newWaku_category->clear();
}

void SettingsWindow::newWakuSetIndex(QString name, QString value)
{
  if (name == "default_community") {
    selectedCommunity = value;
    return;
  }
  if (name == "tags[]") {
    selectedCategory = value;
    return;
  }
}

void SettingsWindow::newWakuSet(QString name, QString value)
{
  if (name == "title") {
    ui->newWaku_title->setText(value);
    return;
  }
  if (name == "description") {
    ui->newWaku_description->setPlainText(value);
    return;
  }
  if (name == "default_community") {
    ui->newWaku_community->addItem(value);
    return;
  }
  if (name == "tags[]c") {
    ui->newWaku_category->addItem(value);
    return;
  }
}

void SettingsWindow::newWakuListStateLoad()
{
  ui->newWaku_community->setCurrentIndex(ui->newWaku_community->findText(selectedCommunity));
  ui->newWaku_category->setCurrentIndex(ui->newWaku_category->findText(selectedCategory));
}
