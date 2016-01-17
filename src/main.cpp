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
    }

    txt = QString("[%1] %2")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
        .arg(txt);

    if (! GlobalSettings::debugToStd) {
        QFile outFile(GlobalSettings::settingsDir + "/log");
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream ts(&outFile);
            ts << txt << endl;
        } else {
          GlobalSettings::debugToStd = true;
        }
    }

    if (GlobalSettings::debugToStd) {
        QTextStream ts(stdout);
        ts << txt << endl;
    }

    if (type == QtMsgType::QtFatalMsg) abort();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Viqo");
    QCoreApplication::setApplicationVersion("3 (dev)");

    QCommandLineParser parser;
    parser.setApplicationDescription(
                QStringLiteral(
                    "Qt5 で作成されたマルチプラットフォームニコ生コメントビューワ"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption saveDirOp({"p", "savepath"},
                                 "Set <directory> to save directory.",
                                 "directory");
    QStringList dirs =
        QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    if (! dirs.empty()) saveDirOp.setDefaultValue(dirs[0]);
    parser.addOption(saveDirOp);

    QCommandLineOption setDebugToStdoutOp("stddbg", "Use stdout to write debug infomation.");
    parser.addOption(setDebugToStdoutOp);

    // Process the actual command line arguments given by the user
    parser.process(a);

    GlobalSettings::settingsDir = parser.value(saveDirOp);
    GlobalSettings::debugToStd = parser.isSet(setDebugToStdoutOp);

    qInstallMessageHandler(viqoMessageHandler);

    MainWindow w;
    w.show();

    return a.exec();
}
