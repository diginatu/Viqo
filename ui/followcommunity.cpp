#include "followcommunity.h"
#include "ui_followcommunity.h"

FollowCommunity::FollowCommunity(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FollowCommunity)
{
    ui->setupUi(this);
}

FollowCommunity::~FollowCommunity()
{
    delete ui;
}
