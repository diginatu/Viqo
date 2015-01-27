#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getNewWakuAPI(int type)
{
  if (mNewWaku!=nullptr) delete mNewWaku;
  mNewWaku = new QNetworkAccessManager(this);

  QHttpMultiPart *multiPart;
  if (type > 2) {
    multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setParent(mNewWaku);
  }

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

    QMapIterator<QString, QString> i(newWakuData);
    while (i.hasNext()) {
      i.next();

      QHttpPart tpart;
      tpart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + i.key() + "\""));
      tpart.setBody(i.value().toUtf8());
      multiPart->append(tpart);
    }
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
  delete reply;

  nwin->applySettingsPostData();

  getNewWakuAPI(3);
}

void NicoLiveManager::newWakuConfirmFinished(QNetworkReply* reply){
  newWakuAbstractor(reply, 2);
  newWakuData.insert("kiyaku", "true");
  qDebug() << newWakuData;

  delete reply;
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
  if (mainForm == nullptr) {
    mwin->insLog("NicoLiveManager::newWakuAbstractor reading page error");
    return;
  }

  if (mode == 2) {
    newWakuData.clear();
    categoryPair.clear();
    communityPair.clear();
  }
  if (mode <= 1) nwin->listStateSave();

  StrAbstractor* input;
  while ((input = mainForm->mid("<input", ">")) != nullptr) {
    QString type = input->midStr("type=\"", "\"", false);
    if (type == "button") continue;
    if ((type == "checkbox" || type == "radio") &&
        input->toString().indexOf("checked") == -1) continue;
    QString name = input->midStr("name=\"", "\"", false);
    if (name == "") continue;
    QString value = HTMLdecode(input->midStr("value=\"", "\"", false));
    if (mode == 0) nwin->set(name, value);
    if (mode == 2) newWakuData.insert(name, value);
  }

  mainForm->setPosition(0);

  while ((input = mainForm->mid("<select", "</select>")) != nullptr) {
    QString name = input->mid("", ">")->midStr("name=\"", "\"", false);
    if (name == "") continue;
    StrAbstractor* option;
    while ((option = input->mid("<option", "</option>")) != nullptr) {
      StrAbstractor* head = option->mid("", ">");
      QString value = HTMLdecode(head->midStr("value=\"", "\""));
      if (value == "") continue;
      QString disp = option->midStr("", "");
      if (mode <= 1) {
        if (name == "tags[]") name = "tags[]c";
        nwin->set(name, disp);
        if (head->forward("selected") != -1)
          nwin->setIndex(name, disp);
      }
      if (mode == 2) {
        if (name == "tags[]") {
          categoryPair.insert(disp, value);
        } else if (name == "default_community") {
          communityPair.insert(disp, value);
        } else if (head->forward("selected") != -1) {
          newWakuData.insert(name, value);
        }
      }
    }
  }

  mainForm->setPosition(0);

  while ((input = mainForm->mid("<textarea", "</textarea>")) != nullptr) {
    QString name = input->mid("", ">")->midStr("name=\"", "\"", false);
    if (name == "") continue;
    QString value = HTMLdecode(input->midStr("", ""));
    if (mode == 0) nwin->set(name, value);
    if (mode == 2) newWakuData.insert(name, value);
  }

  if (mode == 1) nwin->listStateLoad();

}

void NicoLiveManager::newWakuSetFormData(QString name, QString value)
{
  if (value == "") return;
  if (name == "tags[]c") {
    newWakuData.insert("tags[]", categoryPair[value]);
    return;
  }
  if (name == "tags[]") {
    newWakuData.insert("tags[]", value);
    return;
  }
  if (name == "default_community") {
    newWakuData.insert("default_community", communityPair[value]);
    return;
  }
  newWakuData.replace(name, value);
}
