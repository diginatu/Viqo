#ifndef FOLLOWCOMMUNITY_H
#define FOLLOWCOMMUNITY_H

#include <QDialog>
#include <QInputDialog>
#include <QTreeWidget>

class MainWindow;
class NicoLiveManager;

namespace Ui {
class FollowCommunity;
}

class FollowCommunity : public QDialog
{
    Q_OBJECT

public:
    explicit FollowCommunity(MainWindow* mwin, QWidget *parent = 0);
    ~FollowCommunity();

    void init();
    void gotCommunityInfo(QString commid, QString title);
private slots:
    void on_addList_clicked();

    void on_deleteList_clicked();

    void on_buttonBox_accepted();

private:
    Ui::FollowCommunity *ui;
    MainWindow* mwin;
};

#endif // FOLLOWCOMMUNITY_H
