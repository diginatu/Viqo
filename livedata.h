#ifndef BROADDATA_H
#define BROADDATA_H

#include <QObject>


class MainWindow;

class LiveData : public QObject
{
    Q_OBJECT
public:
    explicit LiveData(QString liveID, QString communityID, QString title);
    QString getLiveID();
    QString getTitle();
    QString getCommunityID();


signals:

public slots:

private:
    QString liveID;
    QString communityID;
    QString title;
};

#endif // BROADDATA_H
