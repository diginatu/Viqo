#include "nicolivemanager.h"
#include "../mainwindow.h"

void NicoLiveManager::getNewWakuAPI(int type)
{
  if (mNewWaku!=nullptr) delete mTags;
  mNewWaku = new QNetworkAccessManager(this);

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  multiPart->setParent(mNewWaku);

  if (type == 0) {
    connect(mNewWaku, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(newWakuNewFinished(QNetworkReply*)));

  } else if (type == 1) {
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

    QHttpPart description;
    description.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"description\""));
    description.setBody("けぺけぺ");
    multiPart->append(description);

    QHttpPart default_community;
    default_community.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"default_community\""));
    default_community.setBody("co2345471");
    multiPart->append(default_community);

    QHttpPart tags;
    tags.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"tags[]\""));
    tags.setBody("一般(その他)");
    multiPart->append(tags);

    QHttpPart livetags1;
    livetags1.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"livetags1\""));
    livetags1.setBody("Ubuntu");
    multiPart->append(livetags1);

    QHttpPart livetags2;
    livetags2.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"livetags2\""));
    livetags2.setBody("Viqo");
    multiPart->append(livetags2);

    QHttpPart livetags3;
    livetags3.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"livetags3\""));
    livetags3.setBody("Qt");
    multiPart->append(livetags3);

    QHttpPart all_remain_point;
    all_remain_point.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"all_remain_point\""));
    all_remain_point.setBody("0");
    multiPart->append(all_remain_point);

    QHttpPart reserve_start_ymd;
    reserve_start_ymd.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"reserve_start_ymd\""));
    reserve_start_ymd.setBody("1421679600");
    multiPart->append(reserve_start_ymd);

    QHttpPart reserve_start_h;
    reserve_start_h.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"reserve_start_h\""));
    reserve_start_h.setBody("22");
    multiPart->append(reserve_start_h);

    QHttpPart reserve_start_i;
    reserve_start_i.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"reserve_start_i\""));
    reserve_start_i.setBody("0");
    multiPart->append(reserve_start_i);

    QHttpPart reserve_stream_time;
    reserve_stream_time.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"reserve_stream_time\""));
    reserve_stream_time.setBody("30");
    multiPart->append(reserve_stream_time);

    QHttpPart timeshift_enabled;
    timeshift_enabled.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"timeshift_enabled\""));
    timeshift_enabled.setBody("1");
    multiPart->append(timeshift_enabled);

    QHttpPart twitter_disabled;
    twitter_disabled.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"twitter_disabled\""));
    twitter_disabled.setBody("");
    multiPart->append(twitter_disabled);

    QHttpPart twitter_tag;
    twitter_tag.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"twitter_tag\""));
    twitter_tag.setBody("");
    multiPart->append(twitter_tag);

    QHttpPart ad_enable;
    ad_enable.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"ad_enable\""));
    ad_enable.setBody("1");
    multiPart->append(ad_enable);

    QHttpPart ichiba_type;
    ichiba_type.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"ichiba_type\""));
    ichiba_type.setBody("1");
    multiPart->append(ichiba_type);

    QHttpPart auto_charge_confirmed;
    auto_charge_confirmed.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"auto_charge_confirmed\""));
    auto_charge_confirmed.setBody("0");
    multiPart->append(auto_charge_confirmed);
  } else if (type == 2) {
    connect(mNewWaku, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(newWakuFinished(QNetworkReply*)));

  }

  // make request
  QNetworkRequest rq(QUrl("http://live.nicovideo.jp/editstream"));

  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);

  if (type == 0) {
    mNewWaku->get(rq);
  } else {
    mNewWaku->post(rq, multiPart);
  }

}

void NicoLiveManager::newWakuNewFinished(QNetworkReply* reply){
  QString repdata = QString(reply->readAll());

  StrAbstractor allTagHtml(repdata);
  StrAbstractor* mainForm = allTagHtml.mid("<form action=\"editstream\"", "</form>");

  StrAbstractor* input;
  while ((input = mainForm->mid("<input", ">")) != nullptr) {
    QString type = input->midStr("type=\"", "\"", false);
    if (type == "button") continue;
    if (type == "checkbox" &&
        input->toString().indexOf("checked") == -1) continue;
    QString name = input->midStr("name=\"", "\"", false);
    if (name == "") continue;
    QString value = input->midStr("value=\"", "\"", false);
    qDebug() << name << " : " << value;
  }

  mainForm->setPosition(0);

  while ((input = mainForm->mid("<select", "</select>")) != nullptr) {
    QString name = input->mid("", ">")->midStr("name=\"", "\"", false);
    if (name == "") continue;
    qDebug() << "Select ----- " << name;
    StrAbstractor* option;
    while ((option = input->mid("<option", "</option>")) != nullptr) {
      QString value = option->midStr("value=\"", "\"");
      QString disp = option->midStr(">", "");
      qDebug() << value << " <= " << disp;
    }
  }

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
