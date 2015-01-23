#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getNewWakuAPI(int type)
{
  if (mNewWaku!=nullptr) delete mNewWaku;
  mNewWaku = new QNetworkAccessManager(this);

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  multiPart->setParent(mNewWaku);

  if (type == 0) {
    connect(mNewWaku, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(newWakuNewReuseFinished(QNetworkReply*)));
  } else if (type == 1) {
    connect(mNewWaku, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(newWakuNewUpdateFinished(QNetworkReply*)));
  } else if (type == 2) {
    connect(mNewWaku, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(newWakuNewInitFinished(QNetworkReply*)));
  } else if (type == 3) {
    connect(mNewWaku, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(newWakuConfirmFinished(QNetworkReply*)));

    QHttpPart is_wait;
    is_wait.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"is_wait\""));
    is_wait.setBody("");
    multiPart->append(is_wait);

    QHttpPart usecoupon;
    usecoupon.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"usecoupon\""));
    usecoupon.setBody("");
    multiPart->append(usecoupon);

    QHttpPart title;
    title.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"title\""));
    title.setBody("タイトル");
    multiPart->append(title);

    QHttpPart auto_charge_confirmed;
    auto_charge_confirmed.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"auto_charge_confirmed\""));
    auto_charge_confirmed.setBody("0");
    multiPart->append(auto_charge_confirmed);
  } else if (type == 4) {
    connect(mNewWaku, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(newWakuFinished(QNetworkReply*)));
  }

  // make request
  QNetworkRequest rq(QUrl("http://live.nicovideo.jp/editstream"));

  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setRawHeader("accept-language", "ja");

  if (type <= 2) {
    mNewWaku->get(rq);
  } else {
    mNewWaku->post(rq, multiPart);
  }

}

void NicoLiveManager::newWakuNewReuseFinished(QNetworkReply* reply){
  newWakuAbstractor(reply, 0);
  reply->deleteLater();
}

void NicoLiveManager::newWakuNewUpdateFinished(QNetworkReply* reply){
  newWakuAbstractor(reply, 1);
  reply->deleteLater();
}

void NicoLiveManager::newWakuNewInitFinished(QNetworkReply* reply){
  newWakuAbstractor(reply, 2);
  reply->deleteLater();
}

void NicoLiveManager::newWakuConfirmFinished(QNetworkReply* reply){
  QString repdata = QString(reply->readAll());

  StrAbstractor allTagHtml(repdata);
  qDebug() << repdata;

  reply->deleteLater();
}

void NicoLiveManager::newWakuFinished(QNetworkReply* reply){
  QString repdata = QString(reply->readAll());

  StrAbstractor allTagHtml(repdata);
  qDebug() << repdata;

  reply->deleteLater();
}

void NicoLiveManager::newWakuAbstractor(QNetworkReply* reply, int mode) {
  QString repdata = QString(reply->readAll());

  StrAbstractor allTagHtml(repdata);
  StrAbstractor* mainForm = allTagHtml.mid("<form action=\"editstream\"", "</form>");

  if (mode == 2) newWakuData.clear();
  if (mode <= 1) swin->newWakuListStateSave();

  StrAbstractor* input;
  while ((input = mainForm->mid("<input", ">")) != nullptr) {
    QString type = input->midStr("type=\"", "\"", false);
    if (type == "button") continue;
    if ((type == "checkbox" || type == "radio") &&
        input->toString().indexOf("checked") == -1) continue;
    QString name = input->midStr("name=\"", "\"", false);
    if (name == "") continue;
    QString value = input->midStr("value=\"", "\"", false);
    if (mode == 0) swin->newWakuSet(name, value);
    if (mode == 2) newWakuData.insert(name, value);
  }

  mainForm->setPosition(0);

  while ((input = mainForm->mid("<select", "</select>")) != nullptr) {
    QString name = input->mid("", ">")->midStr("name=\"", "\"", false);
    if (name == "") continue;
    StrAbstractor* option;
    while ((option = input->mid("<option", "</option>")) != nullptr) {
      StrAbstractor* head = option->mid("", ">");
      QString value = head->midStr("value=\"", "\"");
      if (value == "") continue;
      QString disp = option->midStr("", "");
      if (mode <= 1) {
        if (name == "tags[]") name = "tags[]c";
        swin->newWakuSet(name, disp);
        if (head->forward("selected") != -1)
          swin->newWakuSetIndex(name, disp);
      }
      if (mode == 2 && head->forward("selected") != -1) {
        newWakuData.insert(name, disp);
      }
    }
  }

  mainForm->setPosition(0);

  while ((input = mainForm->mid("<textarea", "</textarea>")) != nullptr) {
    QString name = input->mid("", ">")->midStr("name=\"", "\"", false);
    if (name == "") continue;
    QString value = input->midStr("", "");
    if (mode == 0) swin->newWakuSet(name, HTMLdecode(value));
    if (mode == 2) newWakuData.insert(name, value);
  }

  if (mode == 1) swin->newWakuListStateLoad();

  qDebug() << newWakuData;
}
