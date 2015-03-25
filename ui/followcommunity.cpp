#include "followcommunity.h"
#include "ui_followcommunity.h"

#include "mainwindow.h"
#include "../src/NicoLiveManager/nicolivemanager.h"

FollowCommunity::FollowCommunity(MainWindow *mwin, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::FollowCommunity)
{
  this->mwin = mwin;
  ui->setupUi(this);
}

FollowCommunity::~FollowCommunity()
{
  delete ui;
}

void FollowCommunity::init()
{
  typedef QPair<QString,QString> StringPair;
  ui->communityList->clear();
  foreach (StringPair community, mwin->settings.followCommunities) {
    new QTreeWidgetItem(ui->communityList,
                        QStringList() << community.first << community.second);
  }
}

void FollowCommunity::gotCommunityInfo(QString commid, QString title)
{
  if (title.isEmpty()) {
    QMessageBox::information(this, "Viqo", QStringLiteral("コミュニティが見つかりませんでした"));
    return;
  }

  const QStringList ls = QStringList() << commid << title;
  new QTreeWidgetItem(ui->communityList, ls);
}

void FollowCommunity::on_addList_clicked()
{
  QString commid =
      QInputDialog::getText(this, QStringLiteral("コミュニティ追加"),
                            QStringLiteral("コミュニティIDを入力してください"),
                            QLineEdit::Normal, "co");

  if (commid.isNull()) return;

  mwin->nicolivemanager->communityInfoAPI(commid);
}

void FollowCommunity::on_deleteList_clicked()
{
  delete ui->communityList->currentItem();
}

void FollowCommunity::on_buttonBox_accepted()
{
  const QTreeWidget* commli = ui->communityList;
  mwin->settings.followCommunities.clear();
  for( int i = 0; i < commli->topLevelItemCount(); ++i ) {
    QTreeWidgetItem *community = commli->topLevelItem(i);
    mwin->settings.followCommunities.append(qMakePair(community->text(0), community->text(1)));
  }

  mwin->nicolivemanager->updateMyCommunities();

  mwin->settings.saveFollowCommunities();
}
