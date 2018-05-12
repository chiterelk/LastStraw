#include "jtickers.h"

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
}


