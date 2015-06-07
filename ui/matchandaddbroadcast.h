#ifndef MATCHANDADDBROADCAST_H
#define MATCHANDADDBROADCAST_H

#include <QDialog>

class MainWindow;

namespace Ui {
class MatchAndAddBroadcast;
}

class MatchAndAddBroadcast : public QDialog
{
  Q_OBJECT

public:
  explicit MatchAndAddBroadcast(MainWindow* mwin, QWidget *parent = 0);
  ~MatchAndAddBroadcast();

  void init();

private slots:
  void on_addButton_clicked();
  void on_deleteButton_clicked();

  void on_MatchAndAddBroadcast_accepted();

private:
  Ui::MatchAndAddBroadcast *ui;
  MainWindow* mwin;

};

#endif // MATCHANDADDBROADCAST_H
