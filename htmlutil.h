#ifndef HTMLUTIL_H
#define HTMLUTIL_H

#include <QString>
#include <QList>
#include <QPair>

class HTMLUtil
{
public:
    HTMLUtil();
    static QString getSimpleTagParse(QByteArray& html, QString tag,int start);
    static QPair<QString,int> getTagParse(QByteArray &html, QString tag, int start);

    static QList<QString>getSimpleTagParse(QByteArray &html,QList<QString> tags,int start);
    static QPair<QList<QString>,int> getTagParse(QByteArray &html,QList<QString>& tags,int start);

};

#endif // HTMLUTIL_H
