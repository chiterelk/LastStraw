#include "jtickers.h"
#include "QDebug"
#include "QDateTime"

JTickers::JTickers(QObject *parent) : QObject(parent)
{

}

JTickers::JTickers(QString _marketName, double _ask, double _bid, double _baseVolume)
{

	marketName_ = _marketName;
	baseVolume_ = _baseVolume;
	ask_ = _ask;
	bid_ = _bid;
	spread_ = (ask_ - bid_)/ask_;
	rank_ = spread_*baseVolume_;

	//
	//Выделяю главную валюту пары
	//
	baseCurrency_.clear();
	for(int i = 0; i<marketName_.count();i++)
	{
		if(!(marketName_.at(i)==QChar('-')))
		{
			baseCurrency_.append(marketName_.at(i));
		}else{
			break;
		}
	}
	//
	//
	//
}

void JTickers::setCreated(QString _created)
//Монеты которые добавили недаво могут находиться в пампе, из-за этого с ними торговать не буду.
{
	created_ = _created;
	bool newFormat = false;
	for(int i = 0;i<created_.count();i++)
		if(created_.at(i)==QChar('.'))
			newFormat = true;

	if(newFormat)
	{
		createdUNIX_ = QDateTime::fromString(created_,"yyyy-MM-dd'T'HH:mm:ss.z").toTime_t();
	}else
	{
		createdUNIX_ = QDateTime::fromString(created_,"yyyy-MM-dd'T'HH:mm:ss").toTime_t();
	}
}

uint JTickers::hawOld()
{
	return QDateTime::currentDateTime().toTime_t() - createdUNIX_;
}


