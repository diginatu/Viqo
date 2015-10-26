#include "nicolivemanager.h"
#include "../../ui/mainwindow.h"

void NicoLiveManager::getTagsAPI()
{
  if (nowWaku.getBroadID() == "") return;
  if(mTags!=nullptr)  delete mTags;
  mTags = new QNetworkAccessManager(this);

  connect(mTags, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(tagsFinished(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(mwin->settings.getUserSession());
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/livetags.php?v=lv" + nowWaku.getBroadID()));

  mTags->get(rq);
}

void NicoLiveManager::tagsFinished(QNetworkReply* reply){
  QString repdata = QString(reply->readAll());

  nicolive::StrAbstractor allTagHtml(repdata);
  const QString tagsString = allTagHtml.midStr("<table>", "</table>");

  nicolive::StrAbstractor tagArea(tagsString);
  for (;;) {
    const QString tagString = tagArea.midStr("<nobr>", "</nobr>");
    if (tagString == "") break;

    nicolive::StrAbstractor tagabs(tagString);
    const QString tag = tagabs.midStr(">", "</a>");
    qDebug() << tag;
  }

  reply->deleteLater();
}
