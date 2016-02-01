#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "preferenceswindow.h"
#include "accountsettingswindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // menu
    // Edit
    void on_preferences_triggered();
    void on_accountSettings_triggered();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
