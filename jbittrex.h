#ifndef JBITTREX_H
#define JBITTREX_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QNetworkRequest>
#include <QUrl>
#include <QTime>

#include "JBittrex/jtickers.h"
#include "JBittrex//jbalance.h"
#include "JBittrex/jmarket.h"
#include "JBittrex/jcurrency.h"
#include "JBittrex/jopenedorder.h"


class JBittrex : public QObject
{
	Q_OBJECT
private:
	QNetworkAccessManager *NMGetTikers_ = new QNetworkAccessManager(this);
	QNetworkAccessManager *NMGetBalances_ = new QNetworkAccessManager(this);
	QNetworkAccessManager *NMGetMarkets_ = new QNetworkAccessManager(this);
	QNetworkAccessManager *NMGetCurrencies_ = new QNetworkAccessManager(this);
	QNetworkAccessManager *NMOpenBuyOrder_ =  new QNetworkAccessManager(this);
	QNetworkAccessManager *NMOpenSellOrder_ =  new QNetworkAccessManager(this);
	QNetworkAccessManager *NMGetOpenOrders_ =  new QNetworkAccessManager(this);

	QTime pingGetTickers_;
	QTime pingGetBalances_;
	QTime pingGetMarkets_;
	QTime pingGetCurrencies_;
	QTime pingOpenBuyOrder_;
	QTime pingOpenSellOrder_;
	QTime pingGetOpenOrders_;

public:
	explicit JBittrex(QObject *parent = nullptr);
	void getTickers();
	void getMarkets();
	void getWallet(QString _apiKey, QString _secretKey);
	void getCurrencies();
	void openBuyOrder(QString _apiKey, QString _secretKey, QString _market, double _quantity, double _rate);
	void openSellOrder(QString _apiKey, QString _secretKey, QString _market, double _quantity, double _rate);
	void getOpenOrders(QString _apiKey, QString _secretKey);

signals:
	ping(int _ping);

	gotTickers(QList <JTickers*> _tickers);
	gotWallet(QList <JBalance*> _wallet);
	gotMarkets(QList <JMarket*> _markets);
	gotCurrencies(QList <JCurrency*> _markets);
	openedBuyOrder(QString _uuid);
	openedSellOrder(QString _uuid);
	gotOpenOrders(QList <JOpenedOrder*> _openedOrders);
private slots:
	void gotTickers_(QNetworkReply *reply);
	void gotBalances_(QNetworkReply *reply);
	void gotMarkets_(QNetworkReply *reply);
	void gotCurrencies_(QNetworkReply *reply);
	void openedBuyOrder_(QNetworkReply *reply);
	void openedSellOrder_(QNetworkReply *reply);
	void gotOpenOrders_(QNetworkReply *reply);

};

#endif // JBITTREX_H
