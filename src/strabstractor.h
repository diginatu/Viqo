#ifndef STRABSTRACTOR_H
#define STRABSTRACTOR_H

#include <QObject>

class StrAbstractor : public QObject
{
	Q_OBJECT
public:
  explicit StrAbstractor(const QString doc, QObject *parent = 0);
	QString midStr(QString start, QString end, bool foward = true);
  StrAbstractor* mid(QString start, QString end, bool foward = true);
  int forward(QString st);

	void setPosition(int po);
  void setRelativePosition(int pos);
  int getPosition();
  QString toString();

private:
	int pos;
  QString doc;

signals:

public slots:

};

#endif // STRABSTRACTOR_H
