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
  explicit SettingsWindow(MainWindow* mwin, QWidget* parent = 0);
  ~SettingsWindow();
  void init();
  void getUserSessionFinished();


private slots:
  void on_buttonBox_accepted();

private:
  Ui::SettingsWindow *ui;
  MainWindow* mwin;
  void saveSettings();
};

#endif // SETTINGSWINDOW_H
