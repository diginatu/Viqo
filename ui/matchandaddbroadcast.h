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
  void on_communityAddButton_clicked();
  void on_userAddButton_clicked();
  void on_deleteButton_clicked();

  void on_MatchAndAddBroadcast_accepted();

  void on_itemUpButton_clicked();
  void on_itemDownButton_clicked();

  void gotCommunityInfo(QString commid, QString title);

private:
  Ui::MatchAndAddBroadcast *ui;
  MainWindow* mwin;

};

#endif // MATCHANDADDBROADCAST_H
