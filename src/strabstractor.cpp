#include "strabstractor.h"

StrAbstractor::StrAbstractor(const QString doc, QObject *parent) :
  QObject(parent)
{
  pos = 0;
  this->doc = doc;
}

// Returns the QString between "start" and "end".
// Returns null QString if any matched substring are not found.
// Set current position to the next one of "end" found if foward is true (default).
// if start is empty string, the returned value starts from begining of doc.
// if end do so, it ends the end of the doc.
QString StrAbstractor::midStr(QString start, QString end, bool foward)
{
  int st;
  if (start.isEmpty()) st = pos;
  else                 st = doc.indexOf(start,pos);

  if ( st == -1 ) return QString();
  st += start.length();

  int ed;
  if (end.isEmpty()) ed = doc.length();
  else               ed = doc.indexOf(end,st);

  if ( ed == -1 ) return QString();

  if (foward) pos = ed + end.length();

  return doc.mid(st, ed-st);
}

// returns the pointer to new StrAbstractor which contains the substring found by midStr.
StrAbstractor* StrAbstractor::mid(QString start, QString end, bool foward)
{
  QString tmp = midStr(start, end, foward);
  if ( tmp.isNull() ) return nullptr;
  else return new StrAbstractor(tmp, this);
}

// forward position to the position that st found.
// return -1 if no st found, otherwise return the position that st found.
int StrAbstractor::forward(QString st)
{
  int po = doc.indexOf(st,pos);
  if ( po == -1 ) return -1;

  pos = po;
  return po;
}

// Set current position, midStr searches from here, to "pos".
void StrAbstractor::setPosition(int pos)
{
  this->pos = pos;
}
void StrAbstractor::setRelativePosition(int pos)
{
  this->pos += pos;
}

int StrAbstractor::getPosition()
{
  return pos;
}

QString StrAbstractor::toString()
{
  return doc;
}
