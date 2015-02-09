//
//  MarketStructure.cpp
//  EECS 281 Project 2
//
//  Created by John West on 2/8/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#include "MarketStructure.h"

MarketStructure::MarketStructure() {
	market_structure = new vector <list <Order*> >();
}

MarketStructure::MarketStructure(int structure_size) {
	market_structure = new vector <list <Order*> >(structure_size);
	median_structure = new vector <vector <int> >(structure_size);
}

MarketStructure::~MarketStructure() {
	delete market_structure; market_structure = NULL;
	delete median_structure; median_structure = NULL;
}

void MarketStructure::add_order(Order* order) {

	if (!market_structure->at(order->getEquityId()).empty()) {
		market_structure->at(order->getEquityId()).push_back(order);
	}
	else {
		for (list <Order*>::iterator it = market_structure->at(order->getEquityId()).begin();
			 it != market_structure->at(order->getEquityId()).end();) {
			if ((*it)->getPrice() > order->getPrice()) {
				market_structure->at(order->getEquityId()).insert(it, order);

				break;
			}

			if (++it == market_structure->at(order->getEquityId()).end()) {
				market_structure->at(order->getEquityId()).push_back(order);
			}
		}
	}

	return;
}

void MarketStructure::make_matches(Order* order, bool verbose) {
	list <Order*>* equity_list = &market_structure->at(order->getEquityId());

	Order::Transaction desired_transaction;
	list <Order*>::iterator it;
	list <Order*>::iterator end;
	if (order->getTransaction() == Order::BUY) {
		desired_transaction = Order::SELL;
		it = equity_list->begin();
		end = equity_list->end();
	}
	else {
		desired_transaction = Order::BUY;
		it = equity_list->end();
		end = equity_list->begin();
	}

	for ( ; it != end; (desired_transaction == Order::SELL) ? ++it : --it) {

	}

	equity_list = NULL;

	return;
}





















