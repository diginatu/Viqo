#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class PreferencesWindow;
}

class PreferencesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesWindow(QWidget *parent = 0);
    ~PreferencesWindow();

private slots:
    void on_PreferencesWindow_accepted();

private:
    Ui::PreferencesWindow *ui;
};

#endif // PREFERENCESWINDOW_H
