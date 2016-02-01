#ifndef ACCOUNTSETTINGSWINDOW_H
#define ACCOUNTSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class AccountSettingsWindow;
}

class AccountSettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AccountSettingsWindow(QWidget *parent = 0);
    ~AccountSettingsWindow();

private:
    Ui::AccountSettingsWindow *ui;
};

#endif // ACCOUNTSETTINGSWINDOW_H
