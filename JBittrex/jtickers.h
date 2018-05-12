#ifndef JTICKERS_H
#define JTICKERS_H

#include <QObject>

class JTickers : public QObject
{
	Q_OBJECT
private:
	double ask_ = 0;
	double bid_ = 0;
	double baseVolume_ = 0;
	QString marketName_;
	double spread_;
	double rank_;
	public:
	explicit JTickers(QObject *parent = nullptr);
	JTickers(QString _marketName, double _ask, double _bid, double _baseVolume);
	void setAsk(double _ask){	ask_ = _ask;	};
	void setBid(double _bid){	bid_ = _bid;	};
	void setMarketName(QString _marketName){	marketName_ = _marketName;	};

	double getAsk(void){	return ask_;	};
	double getBid(void){	return bid_;	};
	double getSpread(void){	return spread_;	};
	double getRank(void){	return rank_;	};
	QString getMarketName(void){	return marketName_;	};
	double getBaseVolume(){ return baseVolume_; };


signals:

public slots:
};

#endif // JTICKERS_H
