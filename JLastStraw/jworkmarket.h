#ifndef JWORKMARKET_H
#define JWORKMARKET_H

#include <QString>

class JWorkMarket
{
private:
	double priceBuy_ = 0;
	double priceSell_ = 0;
	double minPrise_ = 0;
	double maxPrise_ = 0;
	int process_ = 0;
	//0-пустой
	//1-В ожидании выставления ордера
	//2-Ордер на покупку выставлен.
	//3-Ордер на покупку исполнен. В ожидании выставления ордера на продажу
	//4-Ордер на продажу выставлен.
	//5-Ордер на продажу исполнен.
	double quantity_ = 0;
	QString marketName_ = "";
	QString uuidOfBuyOrders_;
	QString uuidOfSellOrders_;
public:
	JWorkMarket();
	JWorkMarket(double _priceBuy, double _priceSell, double _availableBalance, QString _marketName);

	double getPriseBuy(){ return priceBuy_;	};
	double getPriseSell(){ return priceSell_;	};
	double getMinPrise(){ return minPrise_;	};
	double getMaxPrise(){ return maxPrise_;	};
	int getProcess(){ return process_;	};
	double getQuantity(){ return quantity_;	};
	QString getMarketName(){ return marketName_;	};
	QString getUuidOfBuyOrders(){	return uuidOfBuyOrders_; };
	QString getUuidOfSellOrders(){	return uuidOfSellOrders_; };

	void setMinPrice(double _minPrice){	minPrise_ = _minPrice;	};
	void setMaxPrice(double _maxPrice){	maxPrise_ = _maxPrice;	};
	void setProcess(int _process){ process_ = _process;	};
	void clear();
	bool isActive();
	void change(double _priceBuy, double _priceSell, double _availableBalance, QString _marketName);
	void setUuidOfBuyOrders(QString _uuidOfBuyOrders){	uuidOfBuyOrders_ = _uuidOfBuyOrders; };
	void setUuidOfSellOrders(QString _uuidOfSellOrders){	uuidOfSellOrders_ = _uuidOfSellOrders; };
};

#endif // JWORKMARKET_H
