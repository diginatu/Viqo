#include "nicolivemanager.h"
#include "../../ui/mainwindow.h"

void NicoLiveManager::login(QString mail, QString pass)
{
  if(mLoginManager!=nullptr)  delete mLoginManager;
  mLoginManager = new QNetworkAccessManager(this);

  connect(mLoginManager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(loginFinished(QNetworkReply*)));


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

  mLoginManager->post(rq, params.toString(QUrl::FullyEncoded).toUtf8());
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
            !cookie.value().isEmpty()) {
          mwin->settings.setUserSession(cookie.value());
          mwin->insLog("login succeeded");
          QMessageBox::information(awin, "Viqo", QStringLiteral("ログインに成功しました"));
          success = true;
          break;
        }
      }
      break;
    }
  }

  if (!success) {
    mwin->insLog("login failed");
    QMessageBox::information(awin, "Viqo", QStringLiteral("ログインに失敗しました\nメールアドレスとパスワードを確認してください"));
  }

  mwin->insLog();
  reply->deleteLater();

  awin->getUserSessionFinished();
}
