#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>

class MainWindow;

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsWindow(MainWindow* mwin, QWidget *parent = 0);
  ~SettingsWindow();
  void init();

private slots:
  void on_buttonBox_rejected();

  void on_loginWayCombo_currentIndexChanged(int index);

  void on_buttonBox_accepted();

  void on_cookiesetting_file_open_button_clicked();

  void on_get_session_clicked();

private:
  Ui::SettingsWindow *ui;
  MainWindow* mwin;
  void saveSettings();
};

#endif // SETTINGSWINDOW_H
