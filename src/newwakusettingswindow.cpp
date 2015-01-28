#include "newwakusettingswindow.h"
#include "ui_newwakusettingswindow.h"
#include "mainwindow.h"

NewWakuSettingsWindow::NewWakuSettingsWindow(MainWindow* mwin, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NewWakuSettingsWindow)
{
  ui->setupUi(this);

  this->mwin = mwin;
}

NewWakuSettingsWindow::~NewWakuSettingsWindow()
{
  delete ui;
}

void NewWakuSettingsWindow::on_befWakuReuse_clicked()
{
  mwin->getNewWakuAPI(0);
}

void NewWakuSettingsWindow::setSelectedCategory(const QString& value)
{
  selectedCategory = value;
}

void NewWakuSettingsWindow::setSelectedCommunity(const QString& value)
{
  selectedCommunity = value;
}

void NewWakuSettingsWindow::formInit()
{
  ui->communityOnly->setChecked(false);
}

void NewWakuSettingsWindow::listStateSave()
{
  selectedCommunity = ui->community->currentText();
  selectedCategory = ui->category->currentText();

  ui->community->clear();
  ui->category->clear();
  ui->tags_list->clear();
}

void NewWakuSettingsWindow::setIndex(QString name, QString value)
{
  if (name == "default_community") {
    selectedCommunity = value;
    return;
  }
  if (name == "tags[]c") {
    selectedCategory = value;
    return;
  }
}

void NewWakuSettingsWindow::set(QString name, QString value)
{
  if (name == "title") {
    ui->title->setText(value);
    return;
  }
  if (name == "description") {
    ui->description->setPlainText(value);
    return;
  }
  if (name == "default_community") {
    ui->community->addItem(value);
    return;
  }
  if (name == "tags[]c") {
    ui->category->addItem(value);
    return;
  }
  if (name == "tags[]") {
    if (value == "顔出し") {
      ui->additional_unmask->setChecked(true);
    } else if (value == "凸待ち") {
      ui->additional_callMe->setChecked(true);
    } else if (value == "クルーズ待ち") {
      ui->additional_cruise->setChecked(true);
    }
    return;
  }
  if (name.startsWith("livetags")) {
    int index = name.mid(8).toInt() - 1;
    while (index >= ui->tags_list->count()) {
      auto newitem = new QListWidgetItem(ui->tags_list);
      newitem->setFlags(newitem->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      newitem->setCheckState(Qt::Unchecked);
    }
    ui->tags_list->item(index)->setText(value);
    return;
  }
  if (name.startsWith("taglock")) {
    int index = name.mid(7).toInt() - 1;
    ui->tags_list->item(index)->setCheckState(Qt::Checked);
    return;
  }
  if (name == "public_status") {
    if (value == "2")
      ui->communityOnly->setChecked(true);
    else
      ui->communityOnly->setChecked(false);
    return;
  }
  if (name == "timeshift_enabled") {
    if (value == "1")
      ui->timeshift->setChecked(true);
    else
      ui->timeshift->setChecked(false);
    return;
  }
  if (name == "twitter_disabled") {
    if (value == "0")
      ui->twitter->setChecked(true);
    else
      ui->twitter->setChecked(false);
    return;
  }
  if (name == "input_twitter_tag") {
    ui->twitterTag->setText(value);
    return;
  }
  if (name == "ad_enable") {
    if (value == "0")
      ui->advertising->setChecked(true);
    else
      ui->advertising->setChecked(false);
    return;
  }
  if (name == "ichiba_type") {
    if (value == "1")
      ui->ichiba->setChecked(true);
    else
      ui->ichiba->setChecked(false);
    return;
  }
}

void NewWakuSettingsWindow::listStateLoad()
{
  ui->community->setCurrentIndex(ui->community->findText(selectedCommunity));
  ui->category->setCurrentIndex(ui->category->findText(selectedCategory));
}

void NewWakuSettingsWindow::applySettingsPostData()
{
  // necessary
  mwin->nicolivemanager->newWakuSetFormData("title", ui->title->text());
  mwin->nicolivemanager->newWakuSetFormData("description", ui->description->toPlainText());
  mwin->nicolivemanager->newWakuSetFormData("default_community", ui->community->currentText());
  mwin->nicolivemanager->newWakuSetFormData("tags[]c", ui->category->currentText());

  // other
  // tags
  for (int i = 0; i < ui->tags_list->count(); ++i) {
    QListWidgetItem* item = ui->tags_list->item(i);
    mwin->nicolivemanager->newWakuSetFormData("livetags" + QString::number(i+1), item->text());
    if (item->checkState() == Qt::Checked)
      mwin->nicolivemanager->newWakuSetFormData("taglock" + QString::number(i+1), "ロックする");
  }
  if (ui->tag_allLock->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("taglock", "ロックする");
  if (ui->additional_unmask->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("tags[]", "顔出し");
  if (ui->additional_callMe->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("tags[]", "凸待ち");
  if (ui->additional_cruise->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("tags[]", "クルーズ待ち");
  if (ui->communityOnly->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("public_status", "2");
  if (ui->timeshift->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("timeshift_enabled", "1");
  else
    mwin->nicolivemanager->newWakuSetFormData("timeshift_enabled", "0");
  if (ui->twitter->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("twitter_disabled", "0");
  else
    mwin->nicolivemanager->newWakuSetFormData("twitter_disabled", "1");
  mwin->nicolivemanager->newWakuSetFormData("twitter_tag", ui->twitterTag->text());
  if (ui->advertising->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("ad_enable", "0");
  else
    mwin->nicolivemanager->newWakuSetFormData("ad_enable", "1");
  if (ui->ichiba->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("ichiba_type", "1");
  else
    mwin->nicolivemanager->newWakuSetFormData("ichiba_type", "0");

}

bool NewWakuSettingsWindow::isSetNecessary()
{
  return ui->title->text() != "" &&
      ui->description->toPlainText() != "" &&
      ui->community->currentText() != "" &&
      ui->category->currentText() != "";
}

bool NewWakuSettingsWindow::isTwitterTagValid()
{
  return !ui->twitter->isChecked() || ui->twitter->text().startsWith('#');
}

void NewWakuSettingsWindow::on_tag_add_clicked()
{
  QListWidgetItem* item = new QListWidgetItem(ui->tags_list);
  item->setText("タグ名");
  item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
  item->setCheckState(Qt::Unchecked);
}

void NewWakuSettingsWindow::on_tag_delete_clicked()
{
  delete ui->tags_list->currentItem();
}
