#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QtNetwork>
#include <QList>
#include <QVariant>
#include <QIODevice>
#include <QFileDialog>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDesktopServices>
#include <QClipboard>
#include <nl/strabstractor.h>

#include "../src/usermanager.h"
#include "../src/NicoLiveManager/nicolivemanager.h"
#include "../src/NicoLiveManager/autoextend.h"
#include "../src/NicoLiveManager/heartbeat.h"
#include "../src/settings.h"
#include "settingswindow.h"
#include "newwakusettingswindow.h"
#include "accountwindow.h"
#include "getwakutimer.h"
#include "matchandaddbroadcast.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void onReceiveStarted();
  void onReceiveEnded();

  void insComment(int num, bool prem, QString user, QString comm, QString date, bool is_184, bool broadcaster, bool after_open);
  int lastCommentNum();
  void insLog(QString log = "");

  void updateWatchCount();
  void setHousouID(QString text);
  void refleshLiveWaku();

  UserManager* userManager;

  void getSessionFromCookie(QString cookie_name);


  NicoLiveManager* nicolivemanager;

  void bodyClear();

  void submittedComment();
  void getNewWakuAPI(int type, QString livenum = "");
  void userSessionDisabled();

  void deleteBroadcastFromList(QString broadID);

  bool startWakuTimerEnabled;
  QDateTime startWakuTimerTime;

public slots:
  // ui slots
  void on_receive_clicked();
  void on_disconnect_clicked();

  void getWatchCount();
  void on_AccountSettings_triggered();

private slots:
  void timeUpdate();

  void on_comment_view_currentItemChanged(QTreeWidgetItem *current);
  void on_live_waku_list_activated(int index);
  void updateElapsedTime();

  void on_submit_button_clicked();
  void on_submit_text_returnPressed();
  void dropEvent(QDropEvent* event);
  void dragEnterEvent(QDragEnterEvent* event);
  void on_openBrowser_clicked();
  void on_oneCommentActionSearchByGoogle_triggered();
  void on_oneCommentActionCopy_triggered();

  void on_command_test_button_clicked();
  void on_autoNewWakuSettings_triggered();
  void on_getNewWakuNow_triggered();

  void on_save_1_triggered();
  void on_save_2_triggered();
  void on_save_3_triggered();
  void on_save_4_triggered();
  void on_load_1_triggered();
  void on_load_2_triggered();
  void on_load_3_triggered();
  void on_load_4_triggered();

  void on_quit_triggered();

  void on_AboutViqo_triggered();
  void on_AboutQt_triggered();

  void on_clear_triggered();
  void on_setting_triggered();

  void on_comment_view_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

  void on_CommentViewEditKotehan_triggered();
  void on_CommentViewGetKotehan_triggered();
  void on_CommentViewRemoveKotehan_triggered();

  void on_autoGettingUserName_toggled(bool status);
  void on_action_triggered();
  void on_MatchAndAddBroadcast_triggered();


private:
  Ui::MainWindow* ui;

  SettingsWindow* settingsWindow;
  NewWakuSettingsWindow* newWakuSettingsWindow;
  AccountWindow* accountWindow;
  GetWakuTimer* getWakuTimer;
  MatchAndAddBroadcast* matchAndAddBroadcast;

  QTimer* watch_count_timer;
  QTimer* elapsed_time_timer;

  bool userSessionDisabledDialogAppeared;
  bool isCursorTop;
public:
  Settings settings;

};

#endif // MAINWINDOW_H
