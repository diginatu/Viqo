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
  ui->enableChk->setChecked(mwin->settings.getMatchDataEnabled());
  ui->treeWidget->clear();
  foreach (QStringList data, mwin->settings.matchDataList) {
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0, data[0]);
    item->setText(1, data[1]);
    item->setText(2, data[2]);
    item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemNeverHasChildren);
  }
}

void MatchAndAddBroadcast::on_addButton_clicked()
{
  QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
  item->setText(0, QStringLiteral("name"));
  item->setText(1, QStringLiteral("BCU"));
  item->setText(2, QStringLiteral("keyword"));
  item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemNeverHasChildren);
}

void MatchAndAddBroadcast::on_deleteButton_clicked()
{
  delete ui->treeWidget->currentItem();
}

void MatchAndAddBroadcast::on_MatchAndAddBroadcast_accepted()
{
  mwin->settings.setMatchDataEnabled(ui->enableChk->isChecked());

  const QTreeWidget* matchli = ui->treeWidget;
  mwin->settings.matchDataList.clear();

  bool includeBroadInfo = false;
  QRegExp infoNeededRg("[TD]");

  for( int i = 0; i < matchli->topLevelItemCount(); ++i ) {
    QTreeWidgetItem *keyword = matchli->topLevelItem(i);
    if (!includeBroadInfo && infoNeededRg.indexIn(keyword->text(1))!=-1) {
      includeBroadInfo = true;
    }
    mwin->settings.matchDataList.append(
          QStringList() << keyword->text(0) << keyword->text(1) << keyword->text(2)
          );
  }

  mwin->settings.setMatchDataNeedDetailInfo(includeBroadInfo);

  mwin->settings.saveMatchDataList();
}

void MatchAndAddBroadcast::on_itemUpButton_clicked()
{
  QTreeWidgetItem* citem = ui->treeWidget->currentItem();

  int row = ui->treeWidget->currentIndex().row();
  if (citem && row > 0)
  {
    int index = ui->treeWidget->indexOfTopLevelItem(citem);
    QTreeWidgetItem* child = ui->treeWidget->takeTopLevelItem(index);
    ui->treeWidget->insertTopLevelItem(index-1, child);
    ui->treeWidget->setCurrentItem(citem);
  }
}

void MatchAndAddBroadcast::on_itemDownButton_clicked()
{
  QTreeWidgetItem* citem = ui->treeWidget->currentItem();

  int row = ui->treeWidget->currentIndex().row();
  if (citem && row < ui->treeWidget->topLevelItemCount()-1)
  {
    int index = ui->treeWidget->indexOfTopLevelItem(citem);
    QTreeWidgetItem* child = ui->treeWidget->takeTopLevelItem(index);
    ui->treeWidget->insertTopLevelItem(index+1, child);
    ui->treeWidget->setCurrentItem(citem);
  }
}
