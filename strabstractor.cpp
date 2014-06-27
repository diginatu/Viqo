#include "strabstractor.h"

StrAbstractor::StrAbstractor(QByteArray& doc, QObject *parent) :
	QObject(parent)
{
	pos = 0;
	this->doc = &doc;
}

// Return the QString between "start" and "end".
// Set current position to the next one of "end" found if foward is true (default).
QString StrAbstractor::midStr(QString start, QString end, bool foward)
{
	int st = doc->indexOf(start,pos);
	if ( st == -1 ) return "";
	st += start.length();

	const int ed = doc->indexOf(end,st);
	if ( ed == -1 ) return "";

	if (foward)
		pos = ed + end.length();

	return doc->mid(st, ed-st);
}

// Set current position, midStr searches from here, to "pos".
void StrAbstractor::setPosition(int pos)
{
	this->pos = pos;
}
