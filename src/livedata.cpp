#include "livedata.h"

LiveData::LiveData(QString liveID, QString title, QString communityID)
{
  this->liveID = liveID;
  this->communityID = communityID;
  this->title = title;
}

QString LiveData::getLiveID(){
  return liveID;
}

QString LiveData::getTitle(){
  return title;
}
QString LiveData::getCommunityID(){
  return communityID;
}
