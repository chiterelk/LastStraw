#ifndef JOPENEDORDER_H
#define JOPENEDORDER_H

#include <QString>

class JOpenedOrder
{

private:
	QString orderUuid_;
	QString exchange_;
	QString orderType_;
	double quantity_ = 0.0;
	double quantityRemaining_ = 0.0;
	double limit_ = 0.0;
public:

	JOpenedOrder();
	QString getOrderUuid(){	return orderUuid_;	};
	QString getExchange(){	return exchange_;	};
	QString getOrderType(){	return orderType_;	};
	double getQuantity(){	return quantity_;	};
	double getQuantityRemaining(){	return quantityRemaining_;	};
	double getLimit(){	return limit_;	};

	void setOrderUuid(QString _orderUuid){	orderUuid_ = _orderUuid;	};
	void setExchange(QString _exchange){	exchange_ = _exchange;	};
	void setOrderType(QString _orderType){	orderType_ = _orderType;	};
	void setQuantity(double _quantity){	quantity_ = _quantity;	};
	void setQuantityRemaining(double _quantityRemaining){	quantityRemaining_ = _quantityRemaining;	};
	void setLimit(double _limit){	limit_ = _limit;	};
};

#endif // JOPENEDORDER_H
