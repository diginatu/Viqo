#ifndef STRABSTRACTOR_H
#define STRABSTRACTOR_H

#include <QObject>

class StrAbstractor : public QObject
{
	Q_OBJECT
public:
	explicit StrAbstractor(QByteArray& doc, QObject *parent = 0);
	QString midStr(QString start, QString end, bool foward = true);
	int forwardStr(QString st);

	void setPosition(int po);
	int getPosition();

private:
	int pos;
	QByteArray* doc;

signals:

public slots:

};

#endif // STRABSTRACTOR_H
