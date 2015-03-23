#include "nicohttp.h"
#include "../ui/mainwindow.h"

NicoHttp::NicoHttp(MainWindow* mwin, QObject *parent) :
  QObject(parent)
{
  this->mwin = mwin;
}

NicoHttp::~NicoHttp()
{
  reply->deleteLater();
}

// Return PostData with only user_session cookie
QVariant NicoHttp::makePostData(){
  QVariant postData;

  // make cookies
  QList <QNetworkCookie> cookies;
  QNetworkCookie ck;
  ck.toRawForm(QNetworkCookie::NameAndValueOnly);
  ck.setName("user_session");

  QByteArray user_id_ba;
  user_id_ba.append(mwin->settings.getUserSession());

  ck.setValue(user_id_ba);
  cookies.append(ck);

  postData.setValue(cookies);
  return postData;
}

void NicoHttp::getBody(QUrl url)
{
  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData();
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(url);

  reply = mManager.get(rq);
  connect(reply,SIGNAL(finished()),this,SLOT(finished()));
}

void NicoHttp::finished() {}
