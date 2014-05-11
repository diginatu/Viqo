#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookie>
#include <QList>
#include <QVariant>
#include <QNetworkReply>
#include <QIODevice>
#include <QtNetwork>
#include <QFileDialog>
#include <QJsonObject>

#include "commtcp.h"
#include "cookieread.h"
#include "usermanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void insComment(int num, QString user, QString comm, QString date);
	void insLog(QString log);
//	void setUsername(int num, QString string);
	QString getCookieSetting(int n);
	QString getUserSession();
	QString setting_commentCommand;

private slots:
	// ui slots
	void on_receive_clicked();
	void on_disconnect_clicked();

	void on_pushButton_2_clicked();

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

	// connect slot
	void finished();
	void heartbeatfinished();
	void rawMyLivefinished();

private:
	void getAPI(QString user_id, QString broad_id);
	void getComment();
	void getHeartBeatAPI(QString user_id, QString broad_id);
	void getSessionFromCookie();
	void getRawMyLiveHTML(QString user_id);

	CommTcp* commtcp;

	Ui::MainWindow* ui;
	QNetworkReply* reply;
	QByteArray m_data;
	QNetworkAccessManager* mManager;
	QString addr, thread; int port;

	QList<QString> broadIDList;
};

#endif // MAINWINDOW_H
