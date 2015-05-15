#include "matchandaddbroadcast.h"
#include "ui_matchandaddbroadcast.h"
#include "mainwindow.h"

MatchAndAddBroadcast::MatchAndAddBroadcast(MainWindow* mwin, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::MatchAndAddBroadcast)
{
  this->mwin = mwin;
  ui->setupUi(this);
}

MatchAndAddBroadcast::~MatchAndAddBroadcast()
{
  delete ui;
}

void MatchAndAddBroadcast::init()
{

}

void MatchAndAddBroadcast::on_addButton_clicked()
{

}

void MatchAndAddBroadcast::on_deleteButton_clicked()
{

}
