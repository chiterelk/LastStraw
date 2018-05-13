#include "jbittrex.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QByteArray>
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>
#include <QDateTime>



JBittrex::JBittrex(QObject *parent) : QObject(parent)
{

}
void JBittrex::getTickers()
{
	pingGetTickers_.start();
	connect(NMGetTikers_,&QNetworkAccessManager::finished,this,&JBittrex::gotTickers_);
	NMGetTikers_->get(QNetworkRequest(QUrl("https://bittrex.com/api/v1.1/public/getmarketsummaries")));
}

void JBittrex::getMarkets()
{
	pingGetMarkets_.start();
	connect(NMGetMarkets_,&QNetworkAccessManager::finished,this,&JBittrex::gotMarkets_);
	NMGetMarkets_->get(QNetworkRequest(QUrl("https://bittrex.com/api/v1.1/public/getmarkets")));
}

void JBittrex::getCurrencies()
{
	pingGetCurrencies_.start();
	connect(NMGetCurrencies_,&QNetworkAccessManager::finished,this,&JBittrex::gotCurrencies_);
	NMGetCurrencies_->get(QNetworkRequest(QUrl("https://bittrex.com/api/v1.1/public/getcurrencies")));
}

void JBittrex::getWallet(QString _apiKey, QString _secretKey)
{
	QByteArray uri = "https://bittrex.com/api/v1.1/account/getbalances?apikey="+_apiKey.toUtf8()+"&nonce=" + QString::number(QDateTime::currentDateTime().toTime_t()).toUtf8();
	QMessageAuthenticationCode code(QCryptographicHash::Sha512);
	code.setKey(_secretKey.toUtf8());
	code.addData(uri);
	QNetworkRequest request;
	request.setUrl(QUrl(uri));
	request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
	request.setRawHeader("apisign",code.result().toHex());
	connect(NMGetBalances_,&QNetworkAccessManager::finished,this,&JBittrex::gotBalances_);
	pingGetBalances_.start();
	NMGetBalances_->get(request);
}


void JBittrex::openBuyOrder(QString _apiKey, QString _secretKey,QString _market, double _quantity, double _rate)
{
	QByteArray uri = "https://bittrex.com/api/v1.1/market/buylimit?apikey="+_apiKey.toUtf8()+"&market=" + _market.toUtf8() +"&nonce=" + QString::number(QDateTime::currentDateTime().toTime_t()).toUtf8()+"&quantity=" + QString::number(_quantity).toUtf8() +"&rate="+QString::number(_rate).toUtf8();
	qDebug()<<uri;
	QMessageAuthenticationCode code(QCryptographicHash::Sha512);
	code.setKey(_secretKey.toUtf8());
	code.addData(uri);
	QNetworkRequest request;
	request.setUrl(QUrl(uri));
	request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
	request.setRawHeader("apisign",code.result().toHex());
	connect(NMOpenBuyOrder_,&QNetworkAccessManager::finished,this,&JBittrex::openedBuyOrder_);
	pingOpenBuyOrder_.start();
	NMOpenBuyOrder_->get(request);
}

void JBittrex::openSellOrder(QString _apiKey, QString _secretKey,QString _market, double _quantity, double _rate)
{
	QByteArray uri = "https://bittrex.com/api/v1.1/market/selllimit?apikey="+_apiKey.toUtf8()+"&market=" + _market.toUtf8() +"&nonce=" + QString::number(QDateTime::currentDateTime().toTime_t()).toUtf8()+"&quantity=" + QString::number(_quantity).toUtf8() +"&rate="+QString::number(_rate).toUtf8();
	QMessageAuthenticationCode code(QCryptographicHash::Sha512);
	code.setKey(_secretKey.toUtf8());
	code.addData(uri);
	QNetworkRequest request;
	request.setUrl(QUrl(uri));
	request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
	request.setRawHeader("apisign",code.result().toHex());
	connect(NMOpenSellOrder_,&QNetworkAccessManager::finished,this,&JBittrex::openedSellOrder_);
	pingOpenSellOrder_.start();
	NMOpenSellOrder_->get(request);
}

void JBittrex::getOpenOrders(QString _apiKey, QString _secretKey)
{
	QByteArray uri = "https://bittrex.com/api/v1.1/market/getopenorders?apikey="+_apiKey.toUtf8()+"&nonce=" + QString::number(QDateTime::currentDateTime().toTime_t()).toUtf8();
	QMessageAuthenticationCode code(QCryptographicHash::Sha512);
	code.setKey(_secretKey.toUtf8());
	code.addData(uri);
	QNetworkRequest request;
	request.setUrl(QUrl(uri));
	request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
	request.setRawHeader("apisign",code.result().toHex());
	connect(NMGetOpenOrders_,&QNetworkAccessManager::finished,this,&JBittrex::gotOpenOrders_);
	pingGetOpenOrders_.start();
	NMGetOpenOrders_->get(request);
}


void JBittrex::gotTickers_(QNetworkReply *reply)
{
	QDateTime good = QDateTime::currentDateTime();
	good = good.addMonths(-4);

	emit ping(pingGetTickers_.elapsed());

	disconnect(NMGetTikers_,&QNetworkAccessManager::finished,this,&JBittrex::gotTickers_);
	if(!reply->error())
	{
		QJsonDocument documment = QJsonDocument::fromJson(reply->readAll());
		QJsonObject root = documment.object();
		if(root.value("success").toBool())
		{
			if(root.value("result").isArray())
			{
				QList <JTickers*> tickers;
				QJsonArray result = root.value("result").toArray();
				for(int i = 0; i<result.count();i++)
				{
					QJsonObject n = result.at(i).toObject();
					tickers << new JTickers(n.value("MarketName").toString(),n.value("Ask").toDouble(),n.value("Bid").toDouble(),n.value("BaseVolume").toDouble());
					tickers.back()->setCreated(n.value("Created").toString());
				}
				// // // // //
				emit gotTickers(tickers);
			}else{
				qDebug()<<"Result is not array";
			}
		}else{
			qDebug()<<"Success is false";
		}
	}else{
		qDebug()<<"Error!";
	}
}


void JBittrex::gotBalances_(QNetworkReply *reply)
{
	emit ping(pingGetBalances_.elapsed());
	disconnect(NMGetBalances_,&QNetworkAccessManager::finished,this,&JBittrex::gotBalances_);
	if(!reply->error())
	{
		QJsonDocument documment = QJsonDocument::fromJson(reply->readAll());
		QJsonObject root = documment.object();

		if(root.value("success").toBool() == true)
		{

			if(root.value("result").isArray())
			{
				QJsonArray result = root.value("result").toArray();
				QList <JBalance*> wallet;
				for(int i = 0; i < result.count();i++)
				{
					QJsonObject n = result.at(i).toObject();
					wallet << new JBalance(n.value("Currency").toString(),n.value("Balance").toDouble(),n.value("Available").toDouble());
				}
				emit gotWallet(wallet);
			}else{
				qDebug()<<"Result is not array";
			}
		}else{

			qDebug()<<"Success is false";

		}

	}else{
		qDebug()<<"Error!";
	}
}

void JBittrex::gotMarkets_(QNetworkReply *reply)
{
	emit ping(pingGetMarkets_.elapsed());
	disconnect(NMGetMarkets_,&QNetworkAccessManager::finished,this,&JBittrex::gotMarkets_);
	if(!reply->error())
	{
		QJsonDocument documment = QJsonDocument::fromJson(reply->readAll());
		QJsonObject root = documment.object();

		if(root.value("success").toBool() == true)
		{

			if(root.value("result").isArray())
			{
				QJsonArray result = root.value("result").toArray();
				QList <JMarket*> markets;
				for(int i = 0; i < result.count();i++)
				{
					QJsonObject n = result.at(i).toObject();
					markets << new JMarket();
					markets.at(i)->setMarketCurrency(n.value("MarketCurrency").toString());
					markets.at(i)->setBaseCurrency(n.value("BaseCurrency").toString());
					markets.at(i)->setMarketCurrencyLong(n.value("MarketCurrencyLong").toString());
					markets.at(i)->setBaseCurrencyLong(n.value("BaseCurrencyLong").toString());
					markets.at(i)->setMinTradeSize(n.value("MinTradeSize").toDouble());
					markets.at(i)->setMarketName(n.value("MarketName").toString());
					markets.at(i)->setIsActive(n.value("IsActive").toBool());
					markets.at(i)->setCreated(n.value("Created").toString());
				}
				emit gotMarkets(markets);
			}else{
				qDebug()<<"Result is not array";
			}
		}else{
			qDebug()<<"Success is false";
		}

	}else{
		qDebug()<<"Error!";
	}
}

void JBittrex::gotCurrencies_(QNetworkReply *reply)
{
	emit ping(pingGetCurrencies_.elapsed());
	disconnect(NMGetCurrencies_,&QNetworkAccessManager::finished,this,&JBittrex::gotCurrencies_);
	if(!reply->error())
	{
		QJsonDocument documment = QJsonDocument::fromJson(reply->readAll());
		QJsonObject root = documment.object();

		if(root.value("success").toBool() == true)
		{

			if(root.value("result").isArray())
			{
				QJsonArray result = root.value("result").toArray();
				QList <JCurrency*> currencies;
				for(int i = 0; i < result.count();i++)
				{
					QJsonObject n = result.at(i).toObject();
					currencies << new JCurrency(n.value("Currency").toString(),n.value("CurrencyLong").toString(),n.value("IsActive").toBool());
				}
				emit gotCurrencies(currencies);
			}else{
				qDebug()<<"Result is not array";
			}
		}else{
			qDebug()<<"Success is false";
		}

	}else{
		qDebug()<<"Error!";
	}
}




void JBittrex::openedBuyOrder_(QNetworkReply *reply)
{
	emit ping(pingOpenBuyOrder_.elapsed());
	disconnect(NMOpenBuyOrder_,&QNetworkAccessManager::finished,this,&JBittrex::openedBuyOrder_);
	if(!reply->error())
	{
		QJsonDocument documment = QJsonDocument::fromJson(reply->readAll());
		QJsonObject root = documment.object();

		if(root.value("success").toBool() == true)
		{
			emit openedBuyOrder(root.value("result").toObject().value("uuid").toString());
		}else{
			qDebug()<<"Success is false";
			qDebug()<<"message: "<<root.value("message").toString();
		}

	}else{
		qDebug()<<"Error!";
	}
}

void JBittrex::openedSellOrder_(QNetworkReply *reply)
{
	emit ping(pingOpenSellOrder_.elapsed());
	disconnect(NMOpenSellOrder_,&QNetworkAccessManager::finished,this,&JBittrex::openedSellOrder_);
	if(!reply->error())
	{
		QJsonDocument documment = QJsonDocument::fromJson(reply->readAll());
		QJsonObject root = documment.object();

		if(root.value("success").toBool() == true)
		{
			emit openedSellOrder(root.value("result").toObject().value("uuid").toString());
		}else{
			qDebug()<<"Success is false";
			qDebug()<<"message: "<<root.value("message").toString();
		}

	}else{
		qDebug()<<"Error!";
	}
}



void JBittrex::gotOpenOrders_(QNetworkReply *reply)
{
	emit ping(pingGetOpenOrders_.elapsed());
	disconnect(NMGetOpenOrders_,&QNetworkAccessManager::finished,this,&JBittrex::gotOpenOrders_);
	if(!reply->error())
	{
		QJsonDocument documment = QJsonDocument::fromJson(reply->readAll());
		QJsonObject root = documment.object();

		if(root.value("success").toBool() == true)
		{

			if(root.value("result").isArray())
			{
				QJsonArray result = root.value("result").toArray();
				QList <JOpenedOrder*> openedOrders;
				for(int i = 0; i < result.count();i++)
				{
					QJsonObject n = result.at(i).toObject();
					openedOrders << new JOpenedOrder();
					openedOrders.at(i)->setOrderUuid(n.value("OrderUuid").toString());
					openedOrders.at(i)->setExchange(n.value("Exchenge").toString());
					openedOrders.at(i)->setOrderType(n.value("OrderType").toString());
					openedOrders.at(i)->setQuantity(n.value("Quantity").toDouble());
					openedOrders.at(i)->setQuantityRemaining(n.value("QuantityRemaining").toDouble());
					openedOrders.at(i)->setLimit(n.value("Limit").toDouble());
				}
				emit gotOpenOrders(openedOrders);
			}else{
				qDebug()<<"Result is not array";
			}
		}else{
			qDebug()<<"Success is false";
		}

	}else{
		qDebug()<<"Error!";
	}
}



