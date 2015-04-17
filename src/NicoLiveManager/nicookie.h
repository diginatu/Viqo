/*
 * Nicookie.h ニコニコ動画ユーザセッションクッキー取得ライブラリ for Qt
 * 使い方
 *   Nicookie *nicookie = new Nicookie(this);
 *   // リストを取得
 *   QStringList list = nicooki->getBrowserList();
 *   // どれか一個指定して実行。
 *   QString user_session = nicookie->getUserSession(list[0]);
 *   if (nicookie->hasError()) {
 *     // エラー処理
 *     qError() << nicookie->errorString();
 *   }
 */
#ifndef NICOOKIE_H
#define NICOOKIE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QByteArray>
#include <QIODevice>

class Nicookie : public QObject
{
    Q_OBJECT
public:
    static const QString COOKIE_URL;
    static const QString COOKIE_HOST;
    static const QString COOKIE_NAME;
    static const QString COOKIE_PATH;

    static const QString INTERNET_EXPLORER;
    static const QString SAFARI;
    static const QString FIREFOX;
    static const QString CHROME;
    static const QString OPERA;

    static const QStringList BROWSER_LIST;

    enum Error : int {
        NoError = 0,
        NotImplementError,
        UnknownBrowserError,
        NotSupportBrowserError,
        NotFoundDataError,
        NotFoundCookiesFileError,
        InvalidDataFormtaError,
        FailedDecrytError,
        FailedOpenCookiesFileError,
        FailedReadDataError,
        SQLiteError,
        FailedParseProfileError,
    };

private:
    enum Error errorNum;
    QString userSession;

public:
    explicit Nicookie(QObject *parent = 0);
    ~Nicookie();
    QString getUserSession(QString browser);
    const QStringList &getBrowserList() const;
    Error error() const;
    const QString errorString() const;
    bool hasError() const;
    void clear();

private:
    // find Cookie
#ifdef Q_OS_WIN
    bool findInternetExplorer();
#endif // Q_OS_WIN
#ifdef Q_OS_OSX
    bool findSafari();
    bool safariFindFile(QIODevice &device);
    bool safariFindPage(QIODevice &device);
    bool safariFindCookie(QIODevice &device);
#endif // Q_OS_OSX
    bool findFirefox();
    QStringList firefoxGetProfileList(const QString &profile_ini);
    bool firefoxFindValue(const QString &cookies_path);

    bool findChrome();
    bool chromeFindValue(const QString &cookies_path);
    QString chromeDecrypt(const QByteArray &encrypt_data);

    bool findOpera();

    // utility
    void setError(Error num);
    bool querySqlite3(const QString &sqlite3_file,
                                 const QString &query,
                                 const QMap<QString, QVariant> &placeholders,
                                 QMap<QString, QVariant> &values);
    quint32 readUint32BE(QIODevice &device);
    quint32 readUint32LE(QIODevice &device);
    bool checkSameStr(QIODevice &device, const QString &str);
    QString readStr(QIODevice &device);
signals:

public slots:
};

#endif // NICOOKIE_H
