//
//  MarketStructure.cpp
//  EECS 281 Project 2
//
//  Created by John West on 2/8/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#include "MarketStructure.h"

void binary_insertion_price(vector <Order>* vec, Order* order);

void binary_insertion_timestamp(vector <Order>* vec, Order* order,
								int lower, int upper);

MarketStructure::MarketStructure() {
	structure = new vector <vector <Order> >();
}

MarketStructure::MarketStructure(int structure_size) {
	structure = new vector <vector <Order> >(structure_size);
}

MarketStructure::~MarketStructure() {
	delete structure; structure = NULL;
}

void MarketStructure::add_order(Order* order) {

	binary_insertion_price(&structure->at(order->getEquityId()), order);

	return;
}

void binary_insertion_price(vector <Order>* vec, Order* order) {
	int b = 0, e = (int)vec->size();
	int m;
	while (b != e) {
		m = (b + e) / 2;

		if (vec->at(m).getPrice() > order->getPrice()) {
			e = m;
		}
		else if (vec->at(m).getPrice() < order->getPrice()) {
			if (b == m) {
				if (vec->at(m + 1).getPrice() < order->getPrice()) {
					
				}
				
			}
	
			b = m;
		}
		else {
			
		}
	}

	return;
}

void binary_insertion_timestamp(vector <Order>* vec, Order* order,
								int lower, int upper) {
	

	return;
}

