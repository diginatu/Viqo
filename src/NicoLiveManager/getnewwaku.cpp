#include "nicolivemanager.h"
#include "../../ui/mainwindow.h"

void NicoLiveManager::getNewWakuAPI(const int type, QString liveNum)
{
  if (type == 2) {
    if (!nwin->isSetNecessary()) {
      mwin->insLog("NicoLiveManager::getNewWakuAPI type" + QString::number(type) + " no necessary item(s)");
      QMessageBox::information(mwin, "Viqo", QStringLiteral("設定されていない必須項目があります"));
      return;
    }
    if (!nwin->isTwitterTagValid()) {
      mwin->insLog("NicoLiveManager::getNewWakuAPI type" + QString::number(type) + " twitter tag must start with \"#\"");
      QMessageBox::information(mwin, "Viqo", QStringLiteral("ツイッタータグは # で始めてください"));
      return;
    }
  }

  if (mNewWaku!=nullptr) mNewWaku->deleteLater();
  mNewWaku = new QNetworkAccessManager(this);

  QHttpMultiPart *multiPart;
  if (type > 2) {
    multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setParent(mNewWaku);
  } else {
    multiPart = nullptr;
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
  } else if (type == 4) {
    connect(mNewWaku, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(newWakuFinished(QNetworkReply*)));
  }

  if (type == 3 || type == 4) {
    QMapIterator<QString, QString> i(newWakuData);
    while (i.hasNext()) {
      i.next();

      QHttpPart tpart;
      tpart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + i.key() + "\""));
      tpart.setBody(i.value().toUtf8());
      multiPart->append(tpart);
    }
  }

  // make request
  QString durl = "http://live.nicovideo.jp/editstream";
  if (liveNum.length() != 0) durl.append(tr("?reuseid=") + liveNum);
  QNetworkRequest rq;
  rq.setUrl(QUrl(durl));

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
  nwin->applySettingsPostData();
  getNewWakuAPI(3);
}

void NicoLiveManager::newWakuConfirmFinished(QNetworkReply* reply){
  newWakuAbstractor(reply, 3);
  reply->deleteLater();

  newWakuData.replace("kiyaku", "true");

  nwin->songRightsApply();

  // qDebug() << newWakuData;
  getNewWakuAPI(4);
}

void NicoLiveManager::newWakuFinished(QNetworkReply* reply){
  QString location = "";

  auto headers = reply->rawHeaderPairs();
  foreach (auto header, headers) {
    if (header.first == "Location") {
      location = header.second;
      break;
    }
  }

  if (!location.isEmpty()) {
    if (mwin->settings.isAutoNewWakuOpenBrowser()) {
      QString url = "http://live.nicovideo.jp/" + location;
      QDesktopServices::openUrl(url);
    }

    if (location.startsWith("watch") &&
        mwin->settings.isAutoNewWakuStart()) {
      QString broadID;
      const QRegExp broadIDrg("^.+lv(\\d+).*$");
      if (broadIDrg.indexIn(location) != -1) {
        broadID = broadIDrg.cap(1);
      }

      LiveWaku* myNewWaku = new LiveWaku(mwin, this, broadID, this);
      myNewWaku->setFlag(1);
      myNewWaku->getPlayerStatusAPI();
    }
  } else {
    QString body = QString(reply->readAll());
    StrAbstractor bodya(body);

    if (bodya.forward("<div id=\"wait\">") != -1) {
      QString waitingUsers = bodya.midStr("<span id=\"waiting_users\">", "</span>");
      mwin->insLog("wating " + waitingUsers + "users");
      QMessageBox::information(mwin, "Viqo", QStringLiteral("順番待ちが発生しているので並びます （") + waitingUsers + QStringLiteral("人）"));

      newWakuAbstractor(reply, 3);
      reply->deleteLater();

      newWakuData.replace("is_wait", "wait");
      nwin->songRightsApply();

      getNewWakuAPI(4);
    } else {
      mwin->insLog("getting waku failed");
      QMessageBox::information(mwin, "Viqo", QStringLiteral("枠取りに失敗しました"));
      auto headers = reply->rawHeaderPairs();
      qDebug() << headers;
      qDebug() << body;
    }

  }

  reply->deleteLater();
}

void NicoLiveManager::newWakuAbstractor(QNetworkReply* reply, int mode) {
  StrAbstractor allTagHtml(QString(reply->readAll()));

  StrAbstractor* mainForm = allTagHtml.mid("<form action=\"editstream\"", "</form>");
  if (mainForm == nullptr) {
    mwin->insLog("NicoLiveManager::newWakuAbstractor reading page error");
    return;
  }

  if (mode == 0) nwin->formInit();
  if (mode <= 1) nwin->listStateSave();
  if (mode >= 2) newWakuData.clear();

  StrAbstractor* input;
  while ((input = mainForm->mid("<input", ">")) != nullptr) {
    QString type = input->midStr("type=\"", "\"", false);
    if (type == "button") continue;
    if ((type == "checkbox" || type == "radio") &&
        input->toString().indexOf("checked") == -1) continue;
    QString name = input->midStr("name=\"", "\"", false);
    // ignore reservation
    if (name == "" || name == "is_charge") continue;
    QString value = HTMLdecode(input->midStr("value=\"", "\"", false));
    if (mode == 0) nwin->set(name, value);
    // not to add tags and song rights to data when init
    if (mode == 2 && (name.startsWith("livetags") ||
                      name.startsWith("taglock") ||
                      name != "tags[]" ||
                      name != "public_status" ||
                      name != "twitter_tag" ||
                      name.startsWith("rights"))) continue;
    if (mode >= 2) newWakuData.insert(name, value);
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
        nwin->set(name, value, disp);
        if (mode == 0 && head->forward("selected") != -1)
          nwin->setIndex(name, disp);
      }
      if (mode >= 2) {
        if (name != "tags[]" && name != "default_community") {
          if (head->forward("selected") != -1)
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
    if (mode >= 2) newWakuData.insert(name, value);
  }

  if (mode <= 1) nwin->listStateLoad();
  if (mode == 0) nwin->clearWakuDataIfNeeded();

}

void NicoLiveManager::newWakuSetFormData(QString name, QString value)
{
  if (name == "tags[]c") {
    newWakuData.insert("tags[]", value);
    return;
  }
  if (name == "tags[]") {
    newWakuData.insert("tags[]", value);
    return;
  }
  newWakuData.replace(name, value);
}
