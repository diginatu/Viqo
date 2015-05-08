#ifndef GETWAKUTIMER_H
#define GETWAKUTIMER_H

#include <QDialog>

class MainWindow;

namespace Ui {
class GetWakuTimer;
}

class GetWakuTimer : public QDialog
{
  Q_OBJECT

public:
  explicit GetWakuTimer(MainWindow* mwin, QWidget *parent = 0);
  ~GetWakuTimer();

  void init();

private slots:
  void on_buttonBox_accepted();

  void on_getNow_clicked();

private:
  Ui::GetWakuTimer *ui;
  MainWindow* mwin;
};

#endif // GETWAKUTIMER_H
