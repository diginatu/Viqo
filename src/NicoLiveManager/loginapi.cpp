#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::login(QString mail, QString pass)
{
  QNetworkAccessManager* mManager = new QNetworkAccessManager(this);

  QNetworkRequest rq(QUrl("https://secure.nicovideo.jp/secure/login?site=nicolive"));
  rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QUrlQuery params;
  params.addQueryItem("next_url", "");
  params.addQueryItem("show_button_facebook", "0");
  params.addQueryItem("show_button_twitter", "0");
  params.addQueryItem("nolinks", "0");
  params.addQueryItem("_use_valid_error_code", "0");
  params.addQueryItem("mail", QUrl::toPercentEncoding(mail));
  params.addQueryItem("password", QUrl::toPercentEncoding(pass));

  connect(mManager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(loginFinished(QNetworkReply*)));

  mManager->post(rq, params.toString(QUrl::FullyEncoded).toUtf8());
}

void NicoLiveManager::loginFinished(QNetworkReply* reply){
  mwin->insLog("NicoLiveManager::loginFinished");
  auto headers = reply->rawHeaderPairs();

  bool success = false;
  foreach (auto header, headers) {
    if (header.first == "Set-Cookie") {
      auto cookies = QNetworkCookie::parseCookies(header.second);
      foreach (auto cookie, cookies) {
        if (cookie.name() == "user_session" &&
            cookie.value() != "deleted" &&
            cookie.value() != "") {
          mwin->settings.setUserSession(cookie.value());
          mwin->insLog("login succeeded");
          success = true;
          break;
        }
      }
      break;
    }
  }

  if (!success) mwin->insLog("login failed");

  mwin->insLog();
}
