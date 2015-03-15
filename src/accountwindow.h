#ifndef ACCOUNTWINDOW_H
#define ACCOUNTWINDOW_H

#include <QDialog>

namespace Ui {
class AccountWindow;
}

class MainWindow;

class AccountWindow : public QDialog
{
  Q_OBJECT

public:
  explicit AccountWindow(MainWindow* mwin, QWidget* parent = 0);
  ~AccountWindow();

  void init();
  void getUserSessionFinished();

private slots:
  void on_login_way_combo_currentIndexChanged(int index);
  void on_buttonBox_accepted();
  void on_cookiesetting_file_open_button_clicked();
  void on_get_session_clicked();

private:
  Ui::AccountWindow *ui;
  MainWindow* mwin;
};

#endif // ACCOUNTWINDOW_H
