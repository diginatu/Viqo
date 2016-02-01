#include "accountsettingswindow.h"
#include "ui_accountsettingswindow.h"

AccountSettingsWindow::AccountSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountSettingsWindow)
{
    ui->setupUi(this);
}

AccountSettingsWindow::~AccountSettingsWindow()
{
    delete ui;
}
