#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "mainwindow.h"

SettingsWindow::SettingsWindow(MainWindow* mwin, QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsWindow)
{
  ui->setupUi(this);

  this->mwin = mwin;
}

SettingsWindow::~SettingsWindow()
{
  delete ui;
}

void SettingsWindow::init()
{
  ui->comment_ownercomment_chk->setChecked(mwin->settings.getOwnerComment());
  ui->comment_dipspNG_chk->setChecked(mwin->settings.getDispNG());
}

void SettingsWindow::on_buttonBox_accepted()
{
  mwin->settings.setOwnerComment(ui->comment_ownercomment_chk->isChecked());
  mwin->settings.setDispNG(ui->comment_dipspNG_chk->isChecked());

  mwin->settings.saveSettings();
}
