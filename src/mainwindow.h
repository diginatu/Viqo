#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QList>
#include <QVariant>
#include <QIODevice>
#include <QFileDialog>
#include <QJsonObject>

#include "commtcp.h"
#include "cookieread.h"
#include "usermanager.h"
#include "livedata.h"
#include "strabstractor.h"
#include "NicoLiveManager/nicolivemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	void onReceiveStarted();
	void onReceiveEnded();

	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	QTreeWidgetItem* insComment(int num, QString prem, QString user, QString comm, QString date, bool is_184);
	void insLog(QString log);

	QString getCookieSetting(int n);
	QString getUserSession();
	QString setting_commentCommand;

	void setWatchCount();
	void setHousouID(QString text);

	bool isCheckedAutoGettingUserName();
	bool is_next_waku();

	UserManager* userManager;

public slots:
	// ui slots
	void on_receive_clicked();
	void on_disconnect_clicked();

	void getWatchCount();

	void on_cookiesetting_file_open_button_clicked();
	void on_cookiesetting_apply_clicked();
	void on_cookiesetting_browserCombo_currentIndexChanged(int index);

	void on_clear_clicked();

	void on_actionSave_triggered();
	void on_actionLoad_triggered();

	void on_setting_commentComand_checkbox_stateChanged(int st);
	void on_setting_apply_clicked();

	void on_broad_list_activated(int index);
	void on_mylive_reflesh_clicked();

private slots:
	// connect slot
	void rawMyLivefinished();

	void on_commentView_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void on_cookiesetting_usersession_textChanged();

	void on_commentView_currentItemChanged(QTreeWidgetItem *current);

private:
	void getRawMyLiveHTML(QString user_id);
	void getComment();
	void getSessionFromCookie();

	CommTcp* commtcp;

	Ui::MainWindow* ui;
	QByteArray m_data;

	QNetworkAccessManager* mManager;
	QNetworkReply* reply;

	NicoLiveManager* nicolivemanager;

	QList<LiveData*> broadIDList;
	LiveData* currentSelectLive;
	QTimer* liveDataReloadtimer;
	QTimer* watch_count_timer;

};

#endif // MAINWINDOW_H
