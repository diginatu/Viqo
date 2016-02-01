#ifndef HTTPGETTER_H
#define HTTPGETTER_H

#include <QObject>
#include <QNetworkAccessManager>

class MainWindow;

class HttpGetter : public QObject
{
  Q_OBJECT
public:
  explicit HttpGetter(MainWindow* mwin, QObject *parent = 0);
  ~HttpGetter();

  void get();
protected:
  void requestGet(const QNetworkRequest& rq);
  void requestPost(const QNetworkRequest& rq, QIODevice* data);
  QNetworkAccessManager* mManager;
  MainWindow* mwin;

private:

signals:
  void got(QNetworkReply* reply);

public slots:

private slots:

protected slots:
  virtual void gotReply(QNetworkReply* reply);
};

#endif // HTTPGETTER_H
