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


void NewWakuSettingsWindow::listStateSave()
{
  selectedCommunity = ui->community->currentText();
  selectedCategory = ui->category->currentText();

  ui->community->clear();
  ui->category->clear();
}

void NewWakuSettingsWindow::setIndex(QString name, QString value)
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
}

void NewWakuSettingsWindow::listStateLoad()
{
  ui->community->setCurrentIndex(ui->community->findText(selectedCommunity));
  ui->category->setCurrentIndex(ui->category->findText(selectedCategory));
}

void NewWakuSettingsWindow::applySettingsPostData()
{
  mwin->nicolivemanager->newWakuSetFormData("title", ui->title->text());
  mwin->nicolivemanager->newWakuSetFormData("description", ui->description->toPlainText());
  mwin->nicolivemanager->newWakuSetFormData("default_community", ui->community->currentText());
  mwin->nicolivemanager->newWakuSetFormData("tags[]c", ui->category->currentText());
}

void NewWakuSettingsWindow::on_tag_add_clicked()
{
  QTreeWidgetItem* item = new QTreeWidgetItem();
  item->setText(0,"タグ");
  item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
  item->setCheckState(1, Qt::Unchecked);

  ui->tags_list->addTopLevelItem(item);
}
