#include "jworkmarket.h"

#include <QDebug>

JWorkMarket::JWorkMarket()
{

}

JWorkMarket::JWorkMarket(double _priceBuy, double _priceSell, double _availableBalance, QString _marketName)
{
	priceBuy_ = _priceBuy;
	priceSell_ = _priceSell;


	quantity_ = (_availableBalance/(_priceBuy))*0.9999;
	marketName_ = _marketName;
}

void JWorkMarket::clear()
{
	priceBuy_ = 0;
	priceSell_ = 0;
	minPrise_ = 0;
	maxPrise_ = 0;
	process_ = 0;
	quantity_ = 0;
	marketName_.clear();
	uuidOfBuyOrders_.clear();
	uuidOfSellOrders_.clear();

}

bool JWorkMarket::isActive()
{
	if(process_!=0)
	{
		return true;
	}else{
		return false;
	}
}

void JWorkMarket::change(double _priceBuy, double _priceSell, double _availableBalance, QString _marketName)
{
	priceBuy_ = _priceBuy;
	priceSell_ = _priceSell;
    quantity_ = (_availableBalance/(_priceBuy))*0.9999;
	marketName_ = _marketName;
}
