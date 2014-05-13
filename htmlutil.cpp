#include "htmlutil.h"

HTMLUtil::HTMLUtil()
{
}
/**
 * <tag>value</tag>で囲まれたvalue を取り出します。
 * @brief HTMLUtil::getSimpleTagParser
 * @param html 対象のHTML
 * @param tag 対象のタグ
 * @param start　解析開始文字列
 * @return value
 */
QString HTMLUtil::getSimpleTagParser(QByteArray html,QString tag,int start){
    return getTagParser(html,tag,start).first;
}

/**
 * <tag>value</tag>で囲まれたvalue を取り出します。
 * さらに解析終了の文字位置をQPairで返します
 * @brief HTMLUtil::getSimpleTagParser
 * @param html 対象のHTML
 * @param tag 対象のタグ
 * @param start　解析開始文字列
 * @return valueと最後の文字位置
 */
QPair<QString,int> HTMLUtil::getTagParser(QByteArray html,QString tag,int start=0){
    int adrp  = html.indexOf("<"+tag+">",start) + tag.length() + 2;
    int adrpe = html.indexOf("</"+tag+">",adrp);
    return QPair<QString,int>(html.mid(adrp,adrpe),adrpe+tag.length()+3);
}


/**
 * <tag>value</tag>で囲まれたvalue を取り出します。
 * さらに解析終了の文字位置をQPairで返します
 * @brief HTMLUtil::getSimpleTagParser
 * @param html 対象のHTML
 * @param tags 対象の複数タグ
 * @param start　解析開始文字列
 * @return valueのリスト
 */
QList<QString>HTMLUtil::getSimpleTagParser(QByteArray html,QList<QString> tags,int start=0){

    return getTagParser(html,tags,start).first;
}

/**
 * <tag>value</tag>で囲まれたvalue を取り出します。
 * さらに解析終了の文字位置をQPairで返します
 * @brief HTMLUtil::getSimpleTagParser
 * @param html 対象のHTML
 * @param tags 対象の複数タグ
 * @param start　解析開始文字列
 * @return valueのリストと解析終了位置のQPair
 */
QPair<QList<QString>,int> HTMLUtil::getTagParser(QByteArray html,QList<QString> tags,int start=0){
    int current=start;
    QList<QString> valueResult;

    for (int i=0;i<tags.length();i++){
        QPair<QString,int> tmpResult=getTagParser(html,tags[0],current);
        current=tmpResult.second;
        valueResult.append(tmpResult.first);
    }
    return QPair<QList<QString>,int>(valueResult,current);
}
