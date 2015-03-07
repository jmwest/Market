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

#include "Client.h"
#include "Equity.h"

using namespace std;

class Order {
public:
	enum Transaction {NONE, BUY, SELL};

	Order();
	Order(int time_stamp_in, Client* client_in, Transaction transaction_in,
		  Equity* equity_in, int price_in, int quantity_in, int order_num_in,
		  int num_clients_in, int num_equities_in);

	int get_timestamp() const;
	Client* get_client() const;
	Transaction get_transaction() const;
	Equity* get_equity() const;
	int get_price() const;
	int get_quantity() const;
	int get_order_number() const;

	void change_quantity(int change_in_quantity);

private:
	int timestamp;
	Client* client;
	Transaction transaction;
	Equity* equity;
	int price;
	int quantity;

	// used to sort equivalent orders
	int order_num;
};

class BuyOrderComparison {
public:
	bool operator () (Order* ord1, Order* ord2) {
		if (ord1->get_price() < ord2->get_price()) {
			return true;
		}
		else if (ord1->get_price() > ord2->get_price()) {
			return false;
		}
		else {
			if (ord1->get_order_number() > ord2->get_order_number()) {
				return true;
			}
			else {
				return false;
			}
		}
	}
};

class SellOrderComparison {
public:
	bool operator () (Order* ord1, Order* ord2) {
		if (ord1->get_price() > ord2->get_price()) {
			return true;
		}
		else if (ord1->get_price() < ord2->get_price()) {
			return false;
		}
		else {
			if (ord1->get_order_number() > ord2->get_order_number()) {
				return true;
			}
			else {
				return false;
			}
		}
	}
};

Order::Order()
: timestamp(0), transaction(NONE), price(0), quantity(0) {}

Order::Order(int time_stamp_in, Client* client_in, Transaction transaction_in,
			 Equity* equity_in, int price_in, int quantity_in, int order_num_in,
			 int num_clients_in, int num_equities_in)

: timestamp(time_stamp_in), client(client_in), transaction(transaction_in),
equity(equity_in), price(price_in), quantity(quantity_in), order_num(order_num_in) {

	if (time_stamp_in < 0) {
		cerr << "Timestamp for an order was less than zero: exit(1)" << endl;
		exit(1);
	}
	if ((client_in->get_client_id() < 0) || (client_in->get_client_id() >= num_clients_in)) {
		cerr << "Client_id for an order was out of range: exit(1)" << endl;
		exit(1);
	}
	if ((equity_in->get_equity_id() < 0) || (equity_in->get_equity_id() >= num_equities_in)) {
		cerr << "Equity_id for an order was out of range: exit(1)" << endl;
		exit(1);
	}
	if (price_in <= 0) {
		cerr << "Price for an order was less than/equal to zero: exit(1)" << endl;
		exit(1);
	}
	if (quantity_in <= 0) {
		cerr << "Quantity for an order was less than/equal to zero: exit(1)" << endl;
		exit(1);
	}
}

int Order::get_timestamp() const {
	return timestamp;
}

Client* Order::get_client() const {
	return client;
}

Order::Transaction Order::get_transaction() const {
	return transaction;
}

Equity* Order::get_equity() const {
	return equity;
}

int Order::get_price() const {
	return price;
}

int Order::get_quantity() const {
	return quantity;
}

int Order::get_order_number() const {
	return order_num;
}

void Order::change_quantity(int change_in_quantity) {
	quantity = quantity - change_in_quantity;

	return;
}

#endif
