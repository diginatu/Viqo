#include "ui/mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Viqo");
    QCoreApplication::setApplicationVersion("3 (dev)");

    qSetMessagePattern("%{file}:%{line} %{function} - %{message}");

    QCommandLineParser parser;
    parser.setApplicationDescription(
                QStringLiteral("Qt5 で作成されたマルチプラットフォームニコ生コメントビューワ"));
    parser.addHelpOption();
    parser.addVersionOption();

    // Process the actual command line arguments given by the user
    parser.process(a);

    MainWindow w;
    w.show();

    return a.exec();
}
