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
	QString getCookieSetting(int n);

private slots:
	void on_receive_clicked();
	void on_disconnect_clicked();
	void finished();
	void heartbeatfinished();
	void on_pushButton_2_clicked();
	void on_cookiesetting_file_open_button_clicked();
	void on_cookiesetting_apply_clicked();
	void on_cookiesetting_browserCombo_currentIndexChanged(int index);
	void on_clear_clicked();

	void on_actionSave_triggered();

	void on_actionLoad_triggered();

private:
	void getAPI(QString user_id, QString broad_id);
	void getComment();
	void getHeartBeatAPI(QString user_id, QString broad_id);
	void getSessionFromCookie();
	void clearView();
	CommTcp* commtcp;

	Ui::MainWindow* ui;
	QNetworkReply* reply;
	QByteArray m_data;
	QNetworkAccessManager* mManager;
	QString addr, thread;
	int port;
};

#endif // MAINWINDOW_H
