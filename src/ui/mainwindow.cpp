#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_preferences_triggered()
{
    auto preferencesWindow = new PreferencesWindow(this);
    preferencesWindow->setAttribute(Qt::WA_DeleteOnClose);
    preferencesWindow->show();
}

void MainWindow::on_accountSettings_triggered()
{
    auto accountSettingsWindow = new AccountSettingsWindow(this);
    accountSettingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    accountSettingsWindow->show();
}

