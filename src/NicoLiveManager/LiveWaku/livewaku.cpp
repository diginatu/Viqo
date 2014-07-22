#include "livewaku.h"

LiveWaku::LiveWaku(MainWindow* mwin, QObject* parent) :
	QObject(parent)
{
	this->mwin = mwin;
}

LiveWaku::LiveWaku(MainWindow* mwin, QString broadID, QObject *parent) :
	QObject(parent)
{
	this->mwin = mwin;
	this->broadID = broadID;
}

LiveWaku::LiveWaku(MainWindow* mwin, QString broadID, QString community, QString title, QObject *parent) :
	QObject(parent)
{
	this->mwin = mwin;
	this->broadID = broadID;
	this->community = community;
	this->title = title;
}

QString LiveWaku::getTitle() const
{
	return title;
}

void LiveWaku::setTitle(const QString& value)
{
	title = value;
}

QString LiveWaku::getBroadID() const
{
	return broadID;
}

void LiveWaku::setBroadID(const QString& value)
{
	broadID = value;
}

QString LiveWaku::getCommunity() const
{
	return community;
}

void LiveWaku::setCommunity(const QString& value)
{
	community = value;
}

QDateTime LiveWaku::getSt() const
{
	return st;
}

void LiveWaku::setSt(uint unixt)
{
	st.setTime_t(unixt);
}

QDateTime LiveWaku::getEd() const
{
	return ed;
}

void LiveWaku::setEd(uint unixt)
{
	ed.setTime_t(unixt);
}



