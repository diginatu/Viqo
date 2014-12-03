#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QtNetwork>
#include <QList>
#include <QVariant>
#include <QIODevice>
#include <QFileDialog>

#include "cookieread.h"
#include "usermanager.h"
#include "livedata.h"
#include "strabstractor.h"
#include "NicoLiveManager/nicolivemanager.h"
#include "settingswindow.h"
#include "settings.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void onReceiveStarted();
	void onReceiveEnded();

  void insComment(int num, bool prem, QString user, QString comm, QString date, bool is_184, bool broadcaster, bool after_open);
  void insLog(QString log = "");

	void setWatchCount(QString num);
	void setHousouID(QString text);
	void refleshLiveWaku();

	bool isNextWaku();

	UserManager* userManager;

  // command setting
  bool isCommandCommentChecked();
  QString getCommandComment();
  bool isCommandNextWakuChecked();
  QString getCommandNextWaku();

  void getSessionFromCookie();

  SettingsWindow* settingsWindow;

  Settings settings;

public slots:
	// ui slots
	void on_receive_clicked();
	void on_disconnect_clicked();

	void getWatchCount();

	void on_clear_clicked();

	void on_actionSave_triggered();
	void on_actionLoad_triggered();

private slots:
  void on_comment_view_itemDoubleClicked(QTreeWidgetItem *item, int column);
  void on_comment_view_currentItemChanged(QTreeWidgetItem *current);
	void on_live_waku_list_activated(int index);
	void updateElapsedTime();

  void on_action_triggered();

private:

	void bodyClear();

	Ui::MainWindow* ui;

	NicoLiveManager* nicolivemanager;

	QTimer* watch_count_timer;
	QTimer* elapsed_time_timer;

};

#endif // MAINWINDOW_H
