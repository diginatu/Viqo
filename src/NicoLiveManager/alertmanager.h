#ifndef ALERTMANAGER_H
#define ALERTMANAGER_H

#include <QObject>
#include <QMessageBox>
#include <QSharedPointer>
#include <nl/alertlogin.h>
#include <nl/alertadmin.h>
#include <nl/alertconnection.h>

class MainWindow;
class NicoLiveManager;

class AlertManager : public QObject
{
  Q_OBJECT
public:
  explicit AlertManager(MainWindow* mwin, NicoLiveManager* nlman, QObject *parent = 0);
  void login(const QString& mail, const QString& pass, bool willConnect = true);
  void admin(bool willConnect = true);
  void doConnect();
  void disconnect();

  QStringList getMycommunities() const;

private:
  MainWindow* mwin;
  NicoLiveManager* nlman;

  QSharedPointer<nicolive::AlertConnection> alertConnection;

  QString ticket;
  QString addr; qint16 port; QString thread;

  QStringList mycommunities;
signals:
  void gotLogin();
  void gotAdmin();

private slots:
  void newWakuCatched(QString broadID, QString communityID, QString nushiID);
};

#endif // ALERTMANAGER_H
