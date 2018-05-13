#include "jlaststraw.h"

#include <QDebug>

JLastStraw::JLastStraw(QObject *parent) : QObject(parent)
{
	connect(Bittrex_,&JBittrex::ping,this,&JLastStraw::gotPing_);
}

void JLastStraw::getWalletLogIn(QString _apiKey, QString _secretKey)
{
	connect(Bittrex_,&JBittrex::gotWallet,this,&JLastStraw::gotWalletLogIn_);
	apiKey_ = _apiKey;
	secretKey_ = _secretKey;
	Bittrex_->getWallet(_apiKey,_secretKey);
}

void JLastStraw::getWallet(QString _apiKey, QString _secretKey)
{
	connect(Bittrex_,&JBittrex::gotWallet,this,&JLastStraw::gotWallet_);
	Bittrex_->getWallet(_apiKey,_secretKey);
}

void JLastStraw::getWallet()
{
	connect(Bittrex_,&JBittrex::gotWallet,this,&JLastStraw::gotWallet_);
	Bittrex_->getWallet(apiKey_,secretKey_);
}

void JLastStraw::firstStart(bool _activeBaseCurrencyBTC, bool _activeBaseCurrencyETH,
									 bool _activeBaseCurrencyUSDT, int _timeout)
{

	timeoutMainTimer_ = _timeout*1000;
	activeBaseCurrencyBTC_ = _activeBaseCurrencyBTC;
	activeBaseCurrencyETH_ = _activeBaseCurrencyETH;
	activeBaseCurrencyUSDT_ = _activeBaseCurrencyUSDT;
	connect(Bittrex_,&JBittrex::gotTickers,this,&JLastStraw::firstGotTickers_);
	Bittrex_->getTickers();
}

void JLastStraw::getTickersAndOpenedOrders()
{
	connect(Bittrex_,&JBittrex::gotTickers,this,gotTickers_);
	connect(Bittrex_,&JBittrex::gotOpenOrders,this,gotOpenOrders);
	Bittrex_->getTickers();
	Bittrex_->getOpenOrders(apiKey_,secretKey_);
}

void JLastStraw::mainProcess()
{
	//BTC
	if(marketBTC.getProcess()==0)
	{

	}
	if(marketBTC.getProcess()==1)
	{
		connect(BittrexBTC_,&JBittrex::openedBuyOrder,this,&JLastStraw::openedBuyOrderBTC);
		BittrexBTC_->openBuyOrder(apiKey_,secretKey_,marketBTC.getMarketName(),marketBTC.getQuantity(),marketBTC.getPriseBuy());
	}
	if(marketBTC.getProcess()==2)
	{
		bool open = false;
		for(int i = 0; i<openOrders_.count();i++)
		{
			if(openOrders_.at(i)->getOrderUuid() == marketBTC.getUuidOfBuyOrders())
			{
				open = true;
			}
		}
		if(!open)
		{
			confirmationBoughtBTC++;
			getWallet();
		}else{
			confirmationBoughtBTC = 0;
		}

		if(confirmationBoughtBTC>2)
		{
			getWallet();
			emit processBTC(3);
			marketBTC.setProcess(3);
			confirmationBoughtBTC = 0;
		}
	}
	if(marketBTC.getProcess()==3)
	{
		connect(BittrexBTC_,&JBittrex::openedSellOrder,this,&JLastStraw::openedSellOrderBTC);
		BittrexBTC_->openSellOrder(apiKey_,secretKey_,marketBTC.getMarketName(),marketBTC.getQuantity(),marketBTC.getPriseSell());
	}
	if(marketBTC.getProcess()==4)
	{
		bool open = false;
		for(int i = 0; i<openOrders_.count();i++)
		{
			if(openOrders_.at(i)->getOrderUuid() == marketBTC.getUuidOfSellOrders())
			{
				open = true;
			}
		}
		if(!open)
		{
			confirmationSoldBTC++;
			getWallet();
		}else{
			confirmationSoldBTC = 0;
		}
		if(confirmationSoldBTC>2)
		{
			getWallet();
			marketBTC.setProcess(5);
			confirmationSoldBTC = 0;
			emit processBTC(5);
		}
	}
	if(marketBTC.getProcess()==5)
	{
		marketBTC.clear();
	}


	//ETH
	if(marketETH.getProcess()==0)
	{

	}
	if(marketETH.getProcess()==1)
	{
		connect(BittrexETH_,&JBittrex::openedBuyOrder,this,&JLastStraw::openedBuyOrderETH);
		BittrexETH_->openBuyOrder(apiKey_,secretKey_,marketETH.getMarketName(),marketETH.getQuantity(),marketETH.getPriseBuy());
	}
	if(marketETH.getProcess()==2)
	{
		bool open = false;
		for(int i = 0; i<openOrders_.count();i++)
		{
			if(openOrders_.at(i)->getOrderUuid() == marketETH.getUuidOfBuyOrders())
			{
				open = true;
			}
		}
		if(!open)
		{
			confirmationBoughtETH++;
			getWallet();
		}else{
			confirmationBoughtETH = 0;
		}

		if(confirmationBoughtETH>2)
		{
			getWallet();
			marketETH.setProcess(3);
			confirmationBoughtETH = 0;
			emit processETH(3);

		}
	}
	if(marketETH.getProcess()==3)
	{

		connect(BittrexETH_,&JBittrex::openedSellOrder,this,&JLastStraw::openedSellOrderETH);
		BittrexETH_->openSellOrder(apiKey_,secretKey_,marketETH.getMarketName(),marketETH.getQuantity(),marketETH.getPriseSell());
	}
	if(marketETH.getProcess()==4)
	{
		bool open = false;
		for(int i = 0; i<openOrders_.count();i++)
		{
			if(openOrders_.at(i)->getOrderUuid() == marketETH.getUuidOfSellOrders())
			{
				open = true;
			}
		}
		if(!open)
		{
			confirmationSoldETH++;
			getWallet();
		}else{
			confirmationSoldETH = 0;
		}
		if(confirmationSoldETH>2)
		{
			getWallet();
			marketETH.setProcess(5);
			confirmationSoldETH = 0;
			emit processETH(5);
		}
	}
	if(marketETH.getProcess()==5)
	{
		marketETH.clear();
	}



	//USDT
	if(marketUSDT.getProcess()==0)
	{

	}
	if(marketUSDT.getProcess()==1)
	{
		connect(BittrexUSDT_,&JBittrex::openedBuyOrder,this,&JLastStraw::openedBuyOrderUSDT);
		BittrexUSDT_->openBuyOrder(apiKey_,secretKey_,marketUSDT.getMarketName(),marketUSDT.getQuantity(),marketUSDT.getPriseBuy());
	}
	if(marketUSDT.getProcess()==2)
	{
		bool open = false;
		for(int i = 0; i<openOrders_.count();i++)
		{
			if(openOrders_.at(i)->getOrderUuid() == marketUSDT.getUuidOfBuyOrders())
			{
				open = true;
			}
		}
		if(!open)
		{
			confirmationBoughtUSDT++;
			getWallet();
		}else{
			confirmationBoughtUSDT = 0;
		}
		if(confirmationBoughtUSDT>2)
		{
			getWallet();
			emit processUSDT(3);
			marketUSDT.setProcess(3);
			confirmationBoughtUSDT = 0;
		}
	}
	if(marketUSDT.getProcess()==3)
	{
		connect(BittrexUSDT_,&JBittrex::openedSellOrder,this,&JLastStraw::openedSellOrderUSDT);
		BittrexUSDT_->openSellOrder(apiKey_,secretKey_,marketUSDT.getMarketName(),marketUSDT.getQuantity(),marketUSDT.getPriseSell());
	}
	if(marketUSDT.getProcess()==4)
	{
		bool open = false;
		for(int i = 0; i<openOrders_.count();i++)
		{
			if(openOrders_.at(i)->getOrderUuid() == marketUSDT.getUuidOfSellOrders())
			{
				open = true;
			}
		}
		if(!open)
		{
			confirmationSoldUSDT++;
			getWallet();
		}else{
			confirmationSoldUSDT = 0;
		}
		if(confirmationSoldUSDT>2)
		{
			getWallet();
			marketUSDT.setProcess(5);
			confirmationSoldUSDT = 0;
			emit processUSDT(5);
		}
	}
	if(marketUSDT.getProcess()==5)
	{
		marketUSDT.clear();
	}
}

void JLastStraw::firstGotTickers_(QList<JTickers *> _tickers)//Проверяю достаточно ли денег на балансах главных валют(только при запуске!!!).
{
	disconnect(Bittrex_,&JBittrex::gotTickers,this,&JLastStraw::firstGotTickers_);
	JTickers* btc_eth;
	JTickers* usdt_btc;
	for(int i = 0;i<_tickers.count();i++)
	{
		if(_tickers.at(i)->getMarketName() == "BTC-ETH")
			btc_eth = _tickers.at(i);
		if(_tickers.at(i)->getMarketName() == "USDT-BTC")
			usdt_btc = _tickers.at(i);
	}
	double countMinTradeSizeBtc = 0;
	double countMinTradeSizeEth = 0;
	double countMinTradeSizeUsdt = 0;
	for(int i = 0;i<wallet_.count();i++)
	{
		if(wallet_.at(i)->getCurrency()=="BTC")
            countMinTradeSizeBtc = floor(wallet_.at(i)->getAvailable()/(0.0005*1.05));
		if(wallet_.at(i)->getCurrency()=="ETH")
		{
			countMinTradeSizeEth = floor(wallet_.at(i)->getAvailable()/(0.0005*1.05/(btc_eth->getBid())));
		}
        if(wallet_.at(i)->getCurrency()=="USDT")
            countMinTradeSizeUsdt = floor(wallet_.at(i)->getAvailable()/(0.0005*1.05*usdt_btc->getAsk()));
	}
	if(countMinTradeSizeBtc==0.0&&countMinTradeSizeEth==0.0&&countMinTradeSizeUsdt==0.0)
	{
		emit marketNotFound();
	}else{
		if((countMinTradeSizeBtc==0)&&(activeBaseCurrencyBTC_==true))
		{
			activeBaseCurrencyBTC_ = false;
			emit chengeCountWorkMarket("BTC");
		}
		if((countMinTradeSizeEth==0)&&(activeBaseCurrencyETH_==true))
		{
			activeBaseCurrencyETH_ = false;
			emit chengeCountWorkMarket("ETH");
		}
		if((countMinTradeSizeUsdt==0)&&(activeBaseCurrencyUSDT_==true))
		{
			activeBaseCurrencyUSDT_ = false;
			emit chengeCountWorkMarket("USDT");
		}
		delete btc_eth;
		delete usdt_btc;
		if((!activeBaseCurrencyBTC_)&&(!activeBaseCurrencyETH_)&&(!activeBaseCurrencyUSDT_))
		{
			emit marketNotFound();
		}else{
			baseCurrencies.clear();
			if(activeBaseCurrencyBTC_)
				baseCurrencies<<"BTC";
			if(activeBaseCurrencyETH_)
				baseCurrencies<<"ETH";
			if(activeBaseCurrencyUSDT_)
				baseCurrencies<<"USDT";
			QList <JMarket *> _markets;
			for(int i = 0;i<markets_.count();i++)
				for(int n = 0;n<baseCurrencies.count();n++)
					if(markets_.at(i)->getBaseCurrency()==baseCurrencies.at(n))
						_markets<<markets_.at(i);
			markets_ = _markets;
			connect(mainTimer,&QTimer::timeout,this,&JLastStraw::getTickersAndOpenedOrders);
			mainTimer->start(timeoutMainTimer_);
			emit started(markets_.count());
		}

	}
}

void JLastStraw::gotOpenOrders(QList<JOpenedOrder *> _openOrders)
{
	disconnect(Bittrex_,&JBittrex::gotOpenOrders,this,gotOpenOrders);
	openOrders_ = _openOrders;
	mainProcess();
}

void JLastStraw::openedBuyOrderBTC(QString _uuid)
{
	disconnect(BittrexBTC_,&JBittrex::openedBuyOrder,this,&JLastStraw::openedBuyOrderBTC);
	marketBTC.setUuidOfBuyOrders(_uuid);
	marketBTC.setProcess(2);
	getWallet();
	emit processBTC(2);
}

void JLastStraw::openedBuyOrderETH(QString _uuid)
{
	marketETH.setProcess(2);
	disconnect(BittrexETH_,&JBittrex::openedBuyOrder,this,&JLastStraw::openedBuyOrderETH);
	marketETH.setUuidOfBuyOrders(_uuid);
	getWallet();
	emit processETH(2);
}

void JLastStraw::openedBuyOrderUSDT(QString _uuid)
{
	disconnect(BittrexUSDT_,&JBittrex::openedBuyOrder,this,&JLastStraw::openedBuyOrderUSDT);
	marketUSDT.setUuidOfBuyOrders(_uuid);
	marketUSDT.setProcess(2);
	getWallet();
	emit processUSDT(2);
}

void JLastStraw::openedSellOrderBTC(QString _uuid)
{
	disconnect(BittrexBTC_,&JBittrex::openedSellOrder,this,&JLastStraw::openedSellOrderBTC);
	marketBTC.setUuidOfSellOrders(_uuid);
	marketBTC.setProcess(4);
	getWallet();
	emit processBTC(4);
}

void JLastStraw::openedSellOrderETH(QString _uuid)
{
	disconnect(BittrexETH_,&JBittrex::openedSellOrder,this,&JLastStraw::openedSellOrderETH);
	marketETH.setUuidOfSellOrders(_uuid);
	marketETH.setProcess(4);
	getWallet();
	emit processETH(4);
}

void JLastStraw::openedSellOrderUSDT(QString _uuid)
{
	disconnect(BittrexUSDT_,&JBittrex::openedSellOrder,this,&JLastStraw::openedSellOrderUSDT);
	marketUSDT.setUuidOfSellOrders(_uuid);
	marketUSDT.setProcess(4);
	getWallet();
	emit processUSDT(4);
}

void JLastStraw::gotPing_(int _ping)
{

	emit ping(_ping);
}

void JLastStraw::gotWalletLogIn_(QList<JBalance *> _wallet)
{
	wallet_ = _wallet;
	disconnect(Bittrex_,&JBittrex::gotWallet,this,&JLastStraw::gotWalletLogIn_);
	connect(Bittrex_,&JBittrex::gotMarkets,this,gotMarkets_);
	Bittrex_->getMarkets();
	emit logined();
	emit gotWallet(_wallet);

}

void JLastStraw::gotWallet_(QList<JBalance *> _wallet)
{
	wallet_ = _wallet;
	disconnect(Bittrex_,&JBittrex::gotWallet,this,&JLastStraw::gotWallet_);
	emit gotWallet(_wallet);
}

void JLastStraw::gotMarkets_(QList<JMarket *> _market)
{
	markets_.clear();
	disconnect(Bittrex_,&JBittrex::gotMarkets,this,gotMarkets_);
	for(int i = 0; i<_market.count();i++)
	{
		if(_market.at(i)->getIsActive())
		{
			markets_ << _market.at(i);
		}
	}
}

void JLastStraw::gotTickers_(QList<JTickers *> _tickers)
{
	disconnect(Bittrex_,&JBittrex::gotTickers,this,gotTickers_);
	tickers_.clear();
	tickers_ = _tickers;
	checkTickers_();
}

void JLastStraw::checkTickers_()
{
	//
	//Разделяю пары по главных валютах. Если низкий спред, то не добавляю их в список.
	// !! Нужно добавить внесение главной валюты в сам тикер
	//
	tickersBTC.clear();
	tickersETH.clear();
	tickersUSDT.clear();
	for(int i = 0;i<tickers_.count();i++)
	{
			if(activeBaseCurrencyBTC_)
			{
				if(tickers_.at(i)->getBaseCurrency() == "BTC")
				{
						if(!((tickers_.at(i)->getSpread()<minSpreadBTC)||(tickers_.at(i)->getSpread()>maxSpreadBTC)))
							if(tickers_.at(i)->hawOld() > 2592000)//Проверяю давно ли валюту залистили. Если позже чем я хочу, то добавляю в список. 2592000 - это 1 месяц
								tickersBTC<<tickers_.at(i);
				}
			}
			if(activeBaseCurrencyETH_)
			{
				if(tickers_.at(i)->getBaseCurrency() ==  "ETH")
				{
						if(!((tickers_.at(i)->getSpread()<minSpreadETH)||(tickers_.at(i)->getSpread()>maxSpreadETH)))
							if(tickers_.at(i)->hawOld() > 2592000)
								tickersETH<<tickers_.at(i);
				}
			}
			if(activeBaseCurrencyUSDT_)
			{
				if(tickers_.at(i)->getBaseCurrency() == "USDT")
				{
						if(!((tickers_.at(i)->getSpread()<minSpreadUSDT)||(tickers_.at(i)->getSpread()>maxSpreadUSDT)))
						{
							if(tickers_.at(i)->hawOld() > 2592000)
								tickersUSDT<<tickers_.at(i);
						}
				}
			}
	}


	//сортирую по rank
	QList <JTickers*> tickersBTCVolume;
	QList <JTickers*> tickersETHVolume;
	QList <JTickers*> tickersUSDTVolume;
	while (tickersBTC.count())
	{
		double n = 0;
		int k = 0;
		for(int i = 0;i<tickersBTC.count();i++)
		{
			if(n<tickersBTC.at(i)->getBaseVolume())
			{
				n=tickersBTC.at(i)->getBaseVolume();
				k = i;
			}
		}
		tickersBTCVolume<<tickersBTC.at(k);
		tickersBTC.removeAt(k);
	}
	while (tickersETH.count())
	{
		double n = 0;
		int k = 0;
		for(int i = 0;i<tickersETH.count();i++)
		{
			if(n<tickersETH.at(i)->getBaseVolume())
			{
				n=tickersETH.at(i)->getBaseVolume();
				k = i;
			}
		}
		tickersETHVolume<<tickersETH.at(k);
		tickersETH.removeAt(k);
	}
	while (tickersUSDT.count())
	{
		double n = 0;
		int k = 0;
		for(int i = 0;i<tickersUSDT.count();i++)
		{
			if(n<tickersUSDT.at(i)->getBaseVolume())
			{
				n=tickersUSDT.at(i)->getBaseVolume();
				k = i;
			}
		}
		tickersUSDTVolume<<tickersUSDT.at(k);
		tickersUSDT.removeAt(k);
	}
	if((!marketBTC.isActive())&&(tickersBTCVolume.count()!=0))
	{
        double x = (100*tickersBTCVolume.at(0)->getBid()-100*tickersBTCVolume.at(0)->getAsk()+tickersBTCVolume.at(0)->getAsk()*(tickersBTCVolume.at(0)->getSpread()*0.98*100))/((tickersBTCVolume.at(0)->getSpread()*0.98*100)-200);
		double availableBTC = 0;

		for(int i = 0;i<wallet_.count();i++)
			if(wallet_.at(i)->getCurrency()=="BTC")
				availableBTC = wallet_.at(i)->getAvailable();

        qDebug()<<"Spread1: "<<(tickersBTCVolume.at(0)->getAsk()-tickersBTCVolume.at(0)->getBid())*100/tickersBTCVolume.at(0)->getAsk();
		double priceBuy;
		double priceSell;
		priceBuy = QString::number(tickersBTCVolume.at(0)->getBid()+x*0.1,'f',8).toDouble();
		priceSell = QString::number(tickersBTCVolume.at(0)->getAsk()-x*1.9,'f',8).toDouble();
		if(priceBuy==priceSell)
			priceSell = priceSell + 0.00000001;

		//
		qDebug()<<"BTC:";
		qDebug()<<"Bid: "<<priceBuy;
		qDebug()<<"Ask: "<<priceSell;
        qDebug()<<"Spread: "<<(priceSell-priceBuy)*100/priceSell;
		//


		marketBTC.change(priceBuy,priceSell,availableBTC,tickersBTCVolume.at(0)->getMarketName());
        if(resolution)
        {
            marketBTC.setProcess(1);
        }

		emit processBTC(1);

		selectMarketForBTC(tickersBTCVolume.at(0)->getMarketName());
	}

	if((!marketETH.isActive())&&(tickersETHVolume.count()!=0))
	{
		double x = (100*tickersETHVolume.at(0)->getBid()-100*tickersETHVolume.at(0)->getAsk()+tickersETHVolume.at(0)->getAsk()*(tickersETHVolume.at(0)->getSpread()*0.6*100))/((tickersETHVolume.at(0)->getSpread()*0.6*100)-200);
		double availableETH = 0;

		for(int i = 0;i<wallet_.count();i++)
			if(wallet_.at(i)->getCurrency()=="ETH")
			{

				availableETH = wallet_.at(i)->getAvailable();

			}

		qDebug()<<"Spread1: "<<(tickersETHVolume.at(0)->getAsk()-tickersETHVolume.at(0)->getBid())*100/tickersETHVolume.at(0)->getAsk();
		double priceBuy;
		double priceSell;
		priceBuy = QString::number(tickersETHVolume.at(0)->getBid()+x*0.1,'f',8).toDouble();
		priceSell = QString::number(tickersETHVolume.at(0)->getAsk()-x*1.9,'f',8).toDouble();
		if(priceBuy==priceSell)
			priceSell = priceSell + 0.00000001;

		//
		qDebug()<<"ETH:";
		qDebug()<<"Bid: "<<priceBuy;
		qDebug()<<"Ask: "<<priceSell;
		qDebug()<<"Spread: "<<(priceSell-priceBuy)*100/priceSell;
		//

		marketETH.change(priceBuy,priceSell,availableETH,tickersETHVolume.at(0)->getMarketName());
        if(resolution)
        {
            marketETH.setProcess(1);
        }

		emit processETH(1);

		selectMarketForETH(tickersETHVolume.at(0)->getMarketName());
	}


	if((!marketUSDT.isActive())&&(tickersUSDTVolume.count()!=0))
	{
        double x = (100*tickersUSDTVolume.at(0)->getBid()-100*tickersUSDTVolume.at(0)->getAsk()+tickersUSDTVolume.at(0)->getAsk()*(tickersUSDTVolume.at(0)->getSpread()*0.8*100))/((tickersUSDTVolume.at(0)->getSpread()*0.8*100)-200);
		double availableUSDT = 0;

		for(int i = 0;i<wallet_.count();i++)
			if(wallet_.at(i)->getCurrency()=="USDT")
				availableUSDT = wallet_.at(i)->getAvailable();

        qDebug()<<"Spread1: "<<(tickersUSDTVolume.at(0)->getAsk()-tickersUSDTVolume.at(0)->getBid())*100/tickersUSDTVolume.at(0)->getAsk();
		double priceBuy;
		double priceSell;
		priceBuy = QString::number(tickersUSDTVolume.at(0)->getBid()+x*0.1,'f',8).toDouble();
		priceSell = QString::number(tickersUSDTVolume.at(0)->getAsk()-x*1.9,'f',8).toDouble();
		if(priceBuy==priceSell)
			priceSell = priceSell + 0.00000001;

		//
		qDebug()<<"USDT:";
		qDebug()<<"Bid: "<<priceBuy;
		qDebug()<<"Ask: "<<priceSell;
        qDebug()<<"Spread: "<<(priceSell-priceBuy)*100/priceSell;
		//

		marketUSDT.change(priceBuy,priceSell,availableUSDT,tickersUSDTVolume.at(0)->getMarketName());
        if(resolution)
        {
            marketUSDT.setProcess(1);
        }

		emit processUSDT(1);

		selectMarketForUSDT(tickersUSDTVolume.at(0)->getMarketName());
	}
}

