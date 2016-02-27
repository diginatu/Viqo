#include "../ui/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
  QCoreApplication::addLibraryPath("/usr/lib/viqo");
#endif

  QApplication a(argc, argv);

  QCoreApplication::setApplicationName("Viqo");
  QCoreApplication::setApplicationVersion("2.3.3");

  QCommandLineParser parser;
  parser.setApplicationDescription(
        QStringLiteral("Qt で作成されたマルチプラットフォームコメントビューワです"));
  parser.addHelpOption();
  parser.addVersionOption();

  // Process the actual command line arguments given by the user
  parser.process(a);

  MainWindow w;
  w.show();

  return a.exec();
}
