#include "newwakusettingswindow.h"
#include "ui_newwakusettingswindow.h"
#include "mainwindow.h"

NewWakuSettingsWindow::NewWakuSettingsWindow(MainWindow* mwin, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NewWakuSettingsWindow)
{
  ui->setupUi(this);
  setAcceptDrops(true);

  this->mwin = mwin;
}

NewWakuSettingsWindow::~NewWakuSettingsWindow()
{
  delete ui;
}

void NewWakuSettingsWindow::dragEnterEvent(QDragEnterEvent *event)
{
  if (event->mimeData()->hasText())
    event->acceptProposedAction();
}

void NewWakuSettingsWindow::dropEvent(QDropEvent *event)
{
  const QRegExp broadIDrg("^.+lv(\\d+).*$");
  if (broadIDrg.indexIn(event->mimeData()->text()) != -1) {
    mwin->getNewWakuAPI(0, broadIDrg.cap(1));
    event->acceptProposedAction();
  } else {
    event->dropAction();
  }

}

void NewWakuSettingsWindow::on_befWakuReuse_clicked()
{
  mwin->getNewWakuAPI(0);
}

void NewWakuSettingsWindow::on_getTag_clicked()
{
  mwin->getNewWakuAPI(1);
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
  ui->additional_callMe->setChecked(false);
  ui->additional_cruise->setChecked(false);
  ui->additional_unmask->setChecked(false);
}

void NewWakuSettingsWindow::listStateSave()
{
  selectedCommunity = ui->community->currentText();
  selectedCategory = ui->category->currentText();
}

void NewWakuSettingsWindow::clearListForm(int mode)
{
  ui->community->clear();
  ui->category->clear();
  if (mode != 1) ui->tags_list->clear();
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

void NewWakuSettingsWindow::set(QString name, QString value, QString disp)
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
    ui->community->addItem(disp, value);
    return;
  }
  if (name == "tags[]c") {
    ui->category->addItem(disp, value);
    return;
  }
  if (name == "tags[]") {
    if (value == QStringLiteral("顔出し")) {
      ui->additional_unmask->setChecked(true);
    } else if (value == QStringLiteral("凸待ち")) {
      ui->additional_callMe->setChecked(true);
    } else if (value == QStringLiteral("クルーズ待ち")) {
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

void NewWakuSettingsWindow::songRightsApply()
{
  if (ui->song_csv_file->text() == "") return;

  QFile file(ui->song_csv_file->text());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    file.close();
    mwin->insLog("opening csv file failed");
    QMessageBox::information(this, "Viqo", "CSV ファイルのオープンに失敗しました");
    return;
  }

  QTextStream in(&file);

  const QStringList rightsName =
    {"code", "title", "artist", "lyric", "composer", "time"};

  for (int numRights = 0; !in.atEnd(); ++numRights) {
    QString line = in.readLine();
    int mode = 1;
    int stringSt = 0;
    int column = 0;
    for (int i = 0; i < line.length() && column < rightsName.length(); ++i) {
      if (mode == 0) {
        if (line[i] == QChar('"')) {
          mwin->insLog("Song CSV : syntax error");
          QMessageBox::information(this, "Viqo", "CSV ファイルに文法誤りがあります");
          break;
        } else if (line[i] == QChar(',')) {
          mwin->nicolivemanager->newWakuSetFormData(
                QString("rights[%1][%2]").arg(
                  QString::number(numRights),
                  rightsName[column]),
                line.mid(stringSt, i - stringSt));

          stringSt = i+1;
          ++ column;
          mode = 1;
        } else if (i+1 == line.length()) {
          mwin->nicolivemanager->newWakuSetFormData(
                QString("rights[%1][%2]").arg(
                  QString::number(numRights),
                  rightsName[column]),
                line.mid(stringSt));

          ++ column;
        } else {
          mode = 0;
        }
      } else if (mode == 1) {
        if (line[i] == QChar('"')) {
          mode = 2;
          stringSt = i+1;
        } else if (line[i] == QChar(',')) {
          mwin->nicolivemanager->newWakuSetFormData(
                QString("rights[%1][%2]").arg(
                  QString::number(numRights),
                  rightsName[column]),
                line.mid(stringSt, i - stringSt));

          stringSt = i+1;
          ++ column;
          mode = 1;
        } else if (i+1 == line.length()) {
          mwin->nicolivemanager->newWakuSetFormData(
                QString("rights[%1][%2]").arg(
                  QString::number(numRights),
                  rightsName[column]),
                QString());

          ++ column;
        } else {
          mode = 0;
        }
      } else if (mode == 2) {
        if (line[i] == QChar('"')) {
          if (i+1 == line.length()) {
            mwin->nicolivemanager->newWakuSetFormData(
                  QString("rights[%1][%2]").arg(
                    QString::number(numRights),
                    rightsName[column]),
                  line.mid(stringSt, i - stringSt).replace("\"\"", "\""));

            ++ column;
          } else if (line[i+1] == QChar('"')) {
            ++ i;
          } else if (line[i+1] == QChar(',')) {
            mwin->nicolivemanager->newWakuSetFormData(
                  QString("rights[%1][%2]").arg(
                    QString::number(numRights),
                    rightsName[column]),
                  line.mid(stringSt, i - stringSt).replace("\"\"", "\""));

            ++ i;
            ++ column;
            stringSt = i+1;
            mode = 1;
          } else {
            mwin->insLog("Song CSV has a syntax error");
            QMessageBox::information(this, "Viqo", "CSV ファイルに文法誤りがあります");
            break;
          }
        } else if (line[i] == QChar(',') || i+1 == line.length()) {
          mwin->insLog("Song CSV has a syntax error");
          QMessageBox::information(this, "Viqo", "CSV ファイルに文法誤りがあります");
          break;
        }
      }
    }

    for (; column < 6; ++column) {
      mwin->nicolivemanager->newWakuSetFormData(
            QString("rights[%1][%2]").arg(
              QString::number(numRights),
              rightsName[column]),
            QString());
    }
  }

  file.close();
}

void NewWakuSettingsWindow::applySettingsPostData()
{
  // necessary
  mwin->nicolivemanager->newWakuSetFormData("title", ui->title->text());
  mwin->nicolivemanager->newWakuSetFormData("description", ui->description->toPlainText());
  mwin->nicolivemanager->newWakuSetFormData("default_community", ui->community->currentData().toString());
  mwin->nicolivemanager->newWakuSetFormData("tags[]c", ui->category->currentData().toString());

  // tags
  for (int i = 0; i < ui->tags_list->count(); ++i) {
    QListWidgetItem* item = ui->tags_list->item(i);
    mwin->nicolivemanager->newWakuSetFormData("livetags" + QString::number(i+1), item->text());
    if (item->checkState() == Qt::Checked)
      mwin->nicolivemanager->newWakuSetFormData("taglock" + QString::number(i+1), QStringLiteral("ロックする"));
  }

  // other
  if (ui->tag_allLock->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("taglock", QStringLiteral("ロックする"));
  if (ui->additional_unmask->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("tags[]", QStringLiteral("顔出し"));
  if (ui->additional_callMe->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("tags[]", QStringLiteral("凸待ち"));
  if (ui->additional_cruise->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("tags[]", QStringLiteral("クルーズ待ち"));
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

  songRightsApply();
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
  return
      !ui->twitter->isChecked() ||
      ui->twitterTag->text().isEmpty() ||
      ui->twitterTag->text().startsWith('#');
}

void NewWakuSettingsWindow::on_tag_add_clicked()
{
  QListWidgetItem* item = new QListWidgetItem(ui->tags_list);
  item->setText(QStringLiteral("タグ名"));
  item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
  item->setCheckState(Qt::Unchecked);
}

void NewWakuSettingsWindow::on_tag_delete_clicked()
{
  delete ui->tags_list->currentItem();
}

void NewWakuSettingsWindow::savePresets()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    QMessageBox::information(this, "Viqo", "保存領域がないので保存できません");
    return;
  }

  QJsonArray pages;

  for (int i = 0; i < ui->presetes->count(); ++i) {
    QJsonObject page;
    page["title"] = ui->presetes->itemText(i);
    page["data"] = ui->presetes->itemData(i).toJsonObject();
    pages << page;
  }

  QJsonDocument jsd;
  jsd.setArray(pages);

  QFile file(dir[0] + "/newWakuSettings.json");
  if (!file.open(QIODevice::WriteOnly)) {
    file.close();
    mwin->insLog("opening settings file failed");
    QMessageBox::information(this, "Viqo", "設定ファイルに書き込めません");
    return;
  }
  QTextStream out(&file);
  out << jsd.toJson(QJsonDocument::Compact);
  file.close();
}

void NewWakuSettingsWindow::loadPresets()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }
  QFile file(dir[0] + "/newWakuSettings.json");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    file.close();
    mwin->insLog("opening settings file failed");
    return;
  }

  QJsonDocument jsd = QJsonDocument::fromJson(file.readAll());

  QJsonArray pages = jsd.array();
  for (int i = 0; i < pages.size(); ++i) {
    ui->presetes->addItem(pages[i].toObject()["title"].toString(),
        pages[i].toObject()["data"].toObject());
  }

  file.close();
}

QString NewWakuSettingsWindow::getCommunity()
{
  return ui->community->currentData().toString();
}

QJsonObject NewWakuSettingsWindow::makeJsonFromForm()
{
  QJsonObject necessary;
  {
    necessary["title"] = ui->title->text();
    necessary["description"] = ui->description->toPlainText();

    QJsonArray community;
    community << ui->community->currentText();
    community << ui->community->currentData().toString();
    necessary["community"] = community;

    QJsonArray category;
    category << ui->category->currentText();
    category << ui->category->currentData().toString();
    necessary["category"] = category;
  }

  QJsonObject other;
  {
    QJsonArray tags;
    for (int i = 0; i < ui->tags_list->count(); ++i) {
      QListWidgetItem* item = ui->tags_list->item(i);
      QJsonArray jitem;
      jitem << (item->checkState() == Qt::Checked);
      jitem << item->text();
      tags << jitem;
    }
    other["tags"] = tags;

    other["tag_all_lock"] = ui->tag_allLock->isChecked();

    other["add_unmask"] = ui->additional_unmask->isChecked();
    other["add_call_me"] = ui->additional_callMe->isChecked();
    other["add_cruise"] = ui->additional_cruise->isChecked();

    other["community_only"] = ui->communityOnly->isChecked();
    other["timeshift"] = ui->timeshift->isChecked();
    other["twitter"] = ui->twitter->isChecked();
    other["twitter_tag"] = ui->twitterTag->text();
    other["advertising"] = ui->advertising->isChecked();
    other["ichiba"] = ui->ichiba->isChecked();

    other["song_csv"] = ui->song_csv_file->text();
  }

  QJsonObject root;
  root["necessary"] = necessary;
  root["other"] = other;

  return root;
}

void NewWakuSettingsWindow::setPresetsFromJson(const QJsonObject& jsn)
{
  clearListForm();

  {
    const QJsonObject necessary = jsn["necessary"].toObject();

    ui->title->setText(necessary["title"].toString());
    ui->description->setPlainText(necessary["description"].toString());

    const QJsonArray community = necessary["community"].toArray();
    ui->community->addItem(community[0].toString(), community[1].toString());

    const QJsonArray category = necessary["category"].toArray();
    ui->category->addItem(category[0].toString(), category[1].toString());
  }

  {
    const QJsonObject other = jsn["other"].toObject();

    const QJsonArray tags = other["tags"].toArray();
    for (int i = 0; i < tags.size(); ++i) {
      QListWidgetItem* item = new QListWidgetItem(ui->tags_list);
      const QJsonArray jitem = tags[i].toArray();
      item->setText(jitem[1].toString());
      item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      item->setCheckState(jitem[0].toBool()?Qt::Checked:Qt::Unchecked);
    }

    ui->tag_allLock->setChecked(other["tag_all_lock"].toBool());

    ui->additional_unmask->setChecked(other["add_unmask"].toBool());
    ui->additional_callMe->setChecked(other["add_call_me"].toBool());
    ui->additional_cruise->setChecked(other["add_cruise"].toBool());

    ui->communityOnly->setChecked(other["community_only"].toBool());
    ui->timeshift->setChecked(other["timeshift"].toBool());
    ui->twitter->setChecked(other["twitter"].toBool());
    ui->twitterTag->setText(other["twitter_tag"].toString());
    ui->advertising->setChecked(other["advertising"].toBool());
    ui->ichiba->setChecked(other["ichiba"].toBool());

    ui->song_csv_file->setText(other["song_csv"].toString());
  }

}

void NewWakuSettingsWindow::on_presets_regist_clicked()
{
  QString text = QInputDialog::getText(this, QStringLiteral("プリセット登録"),
                             QStringLiteral("プリセット名:"), QLineEdit::Normal,
                             ui->presetes->currentText());
  if (!text.isEmpty()) {
    int indexNew = ui->presetes->findText(text);
    if (indexNew == -1) {
      ui->presetes->addItem(text, makeJsonFromForm());
      ui->presetes->setCurrentText(text);
    } else {
      QMessageBox msgBox(this);
      msgBox.setText(QStringLiteral("上書きしますか？"));
      msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      if (msgBox.exec() == QMessageBox::Ok) {
        ui->presetes->setItemData(indexNew, makeJsonFromForm());
      }
    }
  }
}

void NewWakuSettingsWindow::on_presets_delete_clicked()
{
  ui->presetes->removeItem(ui->presetes->currentIndex());
}

void NewWakuSettingsWindow::on_presetes_activated(int index)
{
  setPresetsFromJson(ui->presetes->itemData(index).toJsonObject());
}

void NewWakuSettingsWindow::on_clear_clicked()
{
  ui->title->clear();
  ui->description->clear();
  ui->community->unsetCursor();
  ui->category->unsetCursor();
  ui->tags_list->clear();
  ui->additional_unmask->setChecked(false);
  ui->additional_callMe->setChecked(false);
  ui->additional_cruise->setChecked(false);
  ui->communityOnly->setChecked(false);
  ui->timeshift->setChecked(false);
  ui->twitter->setChecked(false);
  ui->twitterTag->clear();
  ui->advertising->setChecked(false);
  ui->ichiba->setChecked(false);
}

void NewWakuSettingsWindow::on_okButton_clicked()
{
  savePresets();
}

void NewWakuSettingsWindow::on_song_csv_open_clicked()
{
  const QString filePath = QFileDialog::getOpenFileName(this, tr("Open song CSV File"), QDir::homePath(), tr("CSV Files (*.csv);;All Files (*)"));
  if ( filePath == "" ) return;
  ui->song_csv_file->setText(filePath);
}

void NewWakuSettingsWindow::on_song_csv_clear_clicked()
{
  ui->song_csv_file->clear();
}
