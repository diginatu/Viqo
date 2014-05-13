#ifndef HTMLUTIL_H
#define HTMLUTIL_H

#include <QString>
#include <QList>
#include <QPair>

class HTMLUtil
{
public:
    HTMLUtil();
    static QString getSimpleTagParser(QByteArray html, QString tag,int start);
    static QPair<QString,int> getTagParser(QByteArray html,QString tag,int start);

    static QList<QString>getSimpleTagParser(QByteArray html,QList<QString> tags,int start);
    static QPair<QList<QString>,int> getTagParser(QByteArray html,QList<QString> tags,int start);

};

#endif // HTMLUTIL_H
