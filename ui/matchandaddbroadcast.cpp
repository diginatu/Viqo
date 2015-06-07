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
  ui->treeWidget->clear();
  foreach (QStringList data, mwin->settings.matchDateList) {
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0, data[0]);
    item->setText(1, data[1]);
    item->setText(2, data[3]);
    item->setFlags(item->flags() | Qt::ItemIsEditable |
                   Qt::ItemIsUserCheckable | Qt::ItemNeverHasChildren);
    item->setCheckState(2, (data[2]=="true")?Qt::Checked:Qt::Unchecked);
  }
}

void MatchAndAddBroadcast::on_addButton_clicked()
{
  QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
  item->setText(0, QStringLiteral("name"));
  item->setText(1, QStringLiteral("TDUI"));
  item->setText(2, QStringLiteral("keyword"));
  item->setFlags(item->flags() | Qt::ItemIsEditable |
                 Qt::ItemIsUserCheckable | Qt::ItemNeverHasChildren);
  item->setCheckState(2, Qt::Unchecked);
}

void MatchAndAddBroadcast::on_deleteButton_clicked()
{
  delete ui->treeWidget->currentItem();
}

void MatchAndAddBroadcast::on_MatchAndAddBroadcast_accepted()
{
  const QTreeWidget* matchli = ui->treeWidget;
  mwin->settings.matchDateList.clear();
  for( int i = 0; i < matchli->topLevelItemCount(); ++i ) {
    QTreeWidgetItem *keyword = matchli->topLevelItem(i);
    mwin->settings.matchDateList.append(
          QStringList() << keyword->text(0) << keyword->text(1)
          << (keyword->checkState(2)==Qt::Unchecked?"false":"true") << keyword->text(2)
          );
  }

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
