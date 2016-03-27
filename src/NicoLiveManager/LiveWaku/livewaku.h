#ifndef LIVEWAKU_H
#define LIVEWAKU_H

#include <QObject>
#include <QDateTime>
#include <QNetworkReply>

class MainWindow;
class NicoLiveManager;

class LiveWaku : public QObject
{
  Q_OBJECT
public:
  explicit LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QObject *parent = 0);
  explicit LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QObject *parent = 0);
  explicit LiveWaku(MainWindow* mwin, NicoLiveManager* nlman, QString broadID, QString community, QObject *parent = 0);


  void getPlayerStatusAPI();

  QString getTitle() const;
  void setTitle(QString value);

  QString getBroadID() const;
  void setBroadID(QString value);

  QString getCommunity() const;
  void setCommunity(QString value);

  QString getOwnerName() const;
  void setOwnerName(const QString &value);

  QDateTime getOp() const;
  void setOp(uint unixt);

  QDateTime getSt() const;
  void setSt(uint unixt);

  QDateTime getEd() const;
  void setEd(uint unixt);

  void setFlag(int flag);

  QString getBroadcastToken() const;


signals:

public slots:

private slots:
protected slots:
  void playerStatusFinished(QNetworkReply* reply);

protected:
  QNetworkAccessManager* mManager;

  QString title;
  QString broadID;
  QString community;
  QString ownerName;
  QString broadcastToken;

  QDateTime op;
  QDateTime st;
  QDateTime ed;

  int flag;

  MainWindow* mwin;
  NicoLiveManager* nlman;

};

#endif // LIVEWAKU_H
