#ifndef JLASTSTRAW_H
#define JLASTSTRAW_H

#include <QObject>
#include <QList>
#include <QTimer>


#include "jbittrex.h"
#include "JBittrex/jbalance.h"
#include "JBittrex/jmarket.h"
#include "JBittrex/jtickers.h"
#include "JBittrex/jopenedorder.h"

#include "JLastStraw/jworkmarket.h"

class JLastStraw : public QObject
{
	Q_OBJECT
private:
	QList<JOpenedOrder *> openOrders_;

	JBittrex *Bittrex_ = new JBittrex(this);
	JBittrex *BittrexBTC_ = new JBittrex(this);
	JBittrex *BittrexETH_ = new JBittrex(this);
	JBittrex *BittrexUSDT_ = new JBittrex(this);


	QString apiKey_;
	QString secretKey_;
	QList <JBalance *> wallet_;
	QList<JMarket *> markets_;
	QList <JTickers *> tickers_;
	QTimer *mainTimer = new QTimer(this);
	QList <QString> baseCurrencies;
	int timeoutMainTimer_ = 5000;

    bool resolution = true; //false чтобы запретить выставлять ордера.

    double minSpreadBTC = 0.025;
    double maxSpreadBTC = 0.04;
    double minSpreadETH = 0.025;
    double maxSpreadETH = 0.04;
    double minSpreadUSDT = 0.015;
    double maxSpreadUSDT = 0.04;



	bool activeBaseCurrencyBTC_ = false;
	bool activeBaseCurrencyETH_ = false;
	bool activeBaseCurrencyUSDT_ = false;
	QList <JTickers*> tickersBTC;
	QList <JTickers*> tickersETH;
	QList <JTickers*> tickersUSDT;
	JWorkMarket marketBTC;
	JWorkMarket marketETH;
	JWorkMarket marketUSDT;

	int confirmationBoughtBTC = 0;
	int confirmationBoughtETH = 0;
	int confirmationBoughtUSDT = 0;


	int confirmationSoldBTC = 0;
	int confirmationSoldETH = 0;
	int confirmationSoldUSDT = 0;

public:
	explicit JLastStraw(QObject *parent = nullptr);
	void getWalletLogIn(QString _apiKey, QString _secretKey);
	void getWallet(QString _apiKey, QString _secretKey);
	void getWallet(void);
	void firstStart(bool _activeBaseCurrencyBTC, bool _activeBaseCurrencyETH,
						 bool _activeBaseCurrencyUSDT, int _timeout);
	void getTickersAndOpenedOrders();
	void mainProcess();
signals:
	ping(int _ping);
	gotWallet(QList <JBalance *> _wallet);
	logined();
	started(int countMarkets);
	chengeCountWorkMarket(QString markets_);
	marketNotFound();
	selectMarketForBTC(QString _market);
	selectMarketForETH(QString _market);
	selectMarketForUSDT(QString _market);
	processBTC(int _process);
	processETH(int _process);
	processUSDT(int _process);
public slots:
	void gotPing_(int _ping);
	void gotWalletLogIn_(QList <JBalance*> _wallet);
	void gotWallet_(QList <JBalance *> _wallet);
	void gotMarkets_(QList <JMarket *> _market);
	void gotTickers_(QList <JTickers *> _tickers);
	void checkTickers_();
	void firstGotTickers_(QList <JTickers *> _tickers);
	void gotOpenOrders(QList <JOpenedOrder*> _openOrders);

	void openedBuyOrderBTC(QString _uuid);
	void openedBuyOrderETH(QString _uuid);
	void openedBuyOrderUSDT(QString _uuid);

	void openedSellOrderBTC(QString _uuid);
	void openedSellOrderETH(QString _uuid);
	void openedSellOrderUSDT(QString _uuid);



};

#endif // JLASTSTRAW_H
