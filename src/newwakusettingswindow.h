#ifndef NEWWAKUSETTINGSWINDOW_H
#define NEWWAKUSETTINGSWINDOW_H

#include <QDialog>

class MainWindow;

namespace Ui {
class NewWakuSettingsWindow;
}

class NewWakuSettingsWindow : public QDialog
{
  Q_OBJECT

public:
  explicit NewWakuSettingsWindow(MainWindow* mwin, QWidget *parent = 0);
  ~NewWakuSettingsWindow();

  void listStateSave();
  void setIndex(QString name, QString value);
  void set(QString name, QString value);
  void listStateLoad();

  void setSelectedCommunity(const QString& value);
  void setSelectedCategory(const QString& value);

  void applySettingsPostData();
private:
  Ui::NewWakuSettingsWindow *ui;
  MainWindow* mwin;
  QString selectedCommunity, selectedCategory;

private slots:
  void on_befWakuReuse_clicked();
  void on_tag_add_clicked();
  void on_tag_delete_clicked();
};

#endif // NEWWAKUSETTINGSWINDOW_H
