//
//  Order.h
//  EECS 281 Project 2
//
//  Created by John West on 2/7/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#ifndef EECS_281_Project_2_Order_h
#define EECS_281_Project_2_Order_h

#include <iostream>

using namespace std;

class Order {
public:
	enum Transaction {NONE, BUY, SELL};

	Order();
	Order(int time_stamp_in, int client_in, Transaction transaction_in,
		  int equity_in, int price_in, int quantity_in, int order_num_in,
		  int num_clients_in, int num_equities_in);

	int getTimestamp() const;
	int getClientId() const;
	Transaction getTransaction() const;
	int getEquityId() const;
	int getPrice() const;
	int getQuantity() const;
	int getOrderNumber() const;

	void changeQuantity(int change_in_quantity);

private:
	int timestamp;
	int client_id;
	Transaction transaction;
	int equity_id;
	int price;
	int quantity;

	// used to sort equivalent orders
	int order_num;
};

class BuyOrderComparison {
public:
	bool operator () (Order* ord1, Order* ord2) {
		if (ord1->getPrice() < ord2->getPrice()) {
			return true;
		}
		else if (ord1->getPrice() > ord2->getPrice()) {
			return false;
		}
		else {
			if (ord1->getTimestamp() > ord2->getTimestamp()) {
				return true;
			}
			else if (ord1->getTimestamp() < ord2->getTimestamp()) {
				return false;
			}
			else {
				if (ord1->getOrderNumber() > ord2->getOrderNumber()) {
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
};

class SellOrderComparison {
public:
	bool operator () (Order* ord1, Order* ord2) {
		if (ord1->getPrice() > ord2->getPrice()) {
			return true;
		}
		else if (ord1->getPrice() < ord2->getPrice()) {
			return false;
		}
		else {
			if (ord1->getTimestamp() > ord2->getTimestamp()) {
				return true;
			}
			else if (ord1->getTimestamp() < ord2->getTimestamp()) {
				return false;
			}
			else {
				if (ord1->getOrderNumber() > ord2->getOrderNumber()) {
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
};

Order::Order()
: timestamp(0), client_id(0), transaction(NONE),
equity_id(0), price(0), quantity(0) {}

Order::Order(int time_stamp_in, int client_in, Transaction transaction_in,
			 int equity_in, int price_in, int quantity_in, int order_num_in,
			 int num_clients_in, int num_equities_in)

: timestamp(time_stamp_in), client_id(client_in), transaction(transaction_in),
equity_id(equity_in), price(price_in), quantity(quantity_in), order_num(order_num_in) {

	if (time_stamp_in < 0) {
		cerr << "\nTimestamp for an order was less than zero: exit(1)" << endl;
		exit(1);
	}
	if ((client_in < 0) || (client_in >= num_clients_in)) {
		cerr << "\nClient_id for an order was out of range: exit(1)" << endl;
		exit(1);
	}
	if ((equity_in < 0) || (equity_in >= num_equities_in)) {
		cerr << "\nEquity_id for an order was out of range: exit(1)" << endl;
		exit(1);
	}
	if (price_in < 0) {
		cerr << "\nPrice for an order was less than zero: exit(1)" << endl;
	}
	if (quantity_in < 0) {
		cerr << "\nQuantity for an order was less than zero: exit(1)" << endl;
	}
}

int Order::getTimestamp() const {
	return timestamp;
}

int Order::getClientId() const {
	return client_id;
}

Order::Transaction Order::getTransaction() const {
	return transaction;
}

int Order::getEquityId() const {
	return equity_id;
}

int Order::getPrice() const {
	return price;
}

int Order::getQuantity() const {
	return quantity;
}

int Order::getOrderNumber() const {
	return order_num;
}

void Order::changeQuantity(int change_in_quantity) {
	quantity = quantity - change_in_quantity;

	return;
}

#endif
