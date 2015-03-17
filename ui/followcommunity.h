#ifndef FOLLOWCOMMUNITY_H
#define FOLLOWCOMMUNITY_H

#include <QDialog>

namespace Ui {
class FollowCommunity;
}

class FollowCommunity : public QDialog
{
    Q_OBJECT

public:
    explicit FollowCommunity(QWidget *parent = 0);
    ~FollowCommunity();

private:
    Ui::FollowCommunity *ui;
};

#endif // FOLLOWCOMMUNITY_H
