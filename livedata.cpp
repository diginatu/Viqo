#include "livedata.h"

LiveData:: LiveData(QString liveID,  QString title,QString communityID)
{
   this->liveID=liveID;
   this->communityID=communityID;
   this->title=title;

}

QString LiveData::getLiveID(){
    return this->liveID;
}

QString LiveData::getTitle(){
    return this->title;
}
QString LiveData::getCommunityID(){
    return this->communityID;
}
