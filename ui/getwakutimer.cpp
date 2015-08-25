#include "getwakutimer.h"
#include "ui_getwakutimer.h"

#include "mainwindow.h"

GetWakuTimer::GetWakuTimer(MainWindow* mwin, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::GetWakuTimer)
{
  ui->setupUi(this);
  this->mwin = mwin;
}

GetWakuTimer::~GetWakuTimer()
{
  delete ui;
}

void GetWakuTimer::init()
{
  ui->checkBox->setChecked(mwin->startWakuTimerEnabled);
  ui->dateTimeEdit->setDateTime(mwin->startWakuTimerTime);

  ui->dateTimeEdit->setEnabled(mwin->startWakuTimerEnabled);
  ui->getNow->setEnabled(mwin->startWakuTimerEnabled);
}

void GetWakuTimer::on_buttonBox_accepted()
{
  mwin->startWakuTimerEnabled = ui->checkBox->isChecked();
  mwin->startWakuTimerTime = ui->dateTimeEdit->dateTime();
}

void GetWakuTimer::on_getNow_clicked()
{
  ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}
