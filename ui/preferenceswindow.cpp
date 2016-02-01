#include "preferenceswindow.h"
#include "ui_preferenceswindow.h"

PreferencesWindow::PreferencesWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PreferencesWindow)
{
  ui->setupUi(this);
}

PreferencesWindow::~PreferencesWindow()
{
  delete ui;
}
