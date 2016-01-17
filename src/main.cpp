#include <QApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QStandardPaths>

#include "ui/mainwindow.h"
#include "globalsettings.h"

void viqoMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt = QString("%1:%2 %3 - %4")
        .arg(context.file)
        .arg(context.line)
        .arg(context.function)
        .arg(msg);

    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(txt);
        break;
    case QtInfoMsg:
        txt = QString("Info: %1").arg(txt);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(txt);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(txt);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(txt);
        abort();
    }

    txt = QString("[%1] %2")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
        .arg(txt);

    // QFile outFile("log");
    // if (!outFile.open(QIODevice::WriteOnly | QIODevice::Append))
    //     return;
    // QTextStream ts(&outFile);

    QTextStream ts(stdout);

    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Viqo");
    QCoreApplication::setApplicationVersion("3 (dev)");

    qInstallMessageHandler(viqoMessageHandler);

    QCommandLineParser parser;
    parser.setApplicationDescription(
                QStringLiteral("Qt5 で作成されたマルチプラットフォームニコ生コメントビューワ"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption saveDirOp({"p", "savepath"},
                                 "Set <directory> to save directory.",
                                 "directory");
    QStringList dirs =
        QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    qDebug() << dirs;
    if (dirs.empty()) qWarning() << "no standard Location to save";
    else saveDirOp.setDefaultValue(dirs[0]);
    parser.addOption(saveDirOp);

    // Process the actual command line arguments given by the user
    parser.process(a);

    GlobalSettings::settingsDir = parser.value(saveDirOp);
    qDebug() << GlobalSettings::settingsDir;

    MainWindow w;
    w.show();

    return a.exec();
}
