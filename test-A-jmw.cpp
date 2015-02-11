//
//  test-A-jmw.cpp
//  EECS 281 Project 2
//
//  Created by John West on 2/10/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#include "test-A-jmw.h"

#include <cassert>
#include <iostream>
#include <getopt.h>

int main() {
	Order ord1 = Order(0, 1, Order::BUY, 0, 10, 1, 3, 1);
	Order ord2 = Order(1, 0, Order::SELL, 0, 5, 2, 3, 1);
	Order ord3 = Order(2, 2, Order::SELL, 0, 1, 1, 3, 1);
	Order ord4 = Order(1, 2, Order::BUY, 0, 5, 1, 3, 1);

	// create_order_from_input tests //
	// ord1
	assert(ord1.getTimestamp() == 0);
	assert(ord1.getClientId() == 1);
	assert(ord1.getTransaction() == Order::BUY);
	assert(ord1.getEquityId() == 0);
	assert(ord1.getPrice() == 10);
	assert(ord1.getQuantity() == 1);

	// ord2
	assert(ord2.getTimestamp() == 1);
	assert(ord2.getClientId() == 0);
	assert(ord2.getTransaction() == Order::SELL);
	assert(ord2.getEquityId() == 0);
	assert(ord2.getPrice() == 5);
	assert(ord2.getQuantity() == 2);

	// Orderpq Tests //
	Orderpq q = Orderpq();

	q.push(&ord1);
	q.push(&ord2);
	q.push(&ord3);
	q.push(&ord4);

	assert(q.top()->getPrice() == 1);
	cout << "ord3: " << q.top()->getClientId() << " " << q.top()->getPrice() << endl;
	q.pop();

	assert(q.top()->getPrice() == 5);
	assert(q.top()->getClientId() == 0);
	cout << "ord2: " << q.top()->getClientId() << " " << q.top()->getPrice() << endl;
	q.pop();

	assert(q.top()->getPrice() == 5);
	assert(q.top()->getClientId() == 2);
	cout << "ord4: " << q.top()->getClientId() << " " << q.top()->getPrice() << endl;
	q.pop();

	assert(q.top()->getPrice() == 10);
	cout << "ord1: " << q.top()->getClientId() << " " << q.top()->getPrice() << endl;
	q.pop();

	Order ord5 = Order(3, 2, Order::SELL, 0, 20, 1, 3, 1);

	// can_trade test //
	assert(!can_trade(&ord1, &ord4));
	assert(can_trade(&ord1, &ord2));
	assert(can_trade(&ord1, &ord3));
	assert(!can_trade(&ord2, &ord3));
	assert(can_trade(&ord2, &ord4));
	assert(can_trade(&ord3, &ord4));
	assert(!can_trade(&ord2, &ord5));

		// flipped //
	assert(!can_trade(&ord4, &ord1));
	assert(can_trade(&ord2, &ord1));
	assert(can_trade(&ord3, &ord1));
	assert(!can_trade(&ord3, &ord2));
	assert(can_trade(&ord4, &ord2));
	assert(can_trade(&ord4, &ord3));
	assert(!can_trade(&ord5, &ord2));

	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


// Copied from market.cpp

void parse_command_line_input(int & argc, char *argv[], Verbose &verbose, Median &median,
							  ClientInfo &client_info, TimeTravelers &time_travelers) {
	static struct option longopts[] = {
		{"verbose",			no_argument,		NULL, 'v'},
		{"median",			no_argument,		NULL, 'm'},
		{"client_info",		no_argument,		NULL, 'c'},
		{"time_travelers",	no_argument,		NULL, 't'},
		{NULL,				0,					NULL, 0},
	};
	int idx = 0;
	char c;
	
	// Use getopt to parse through command line input
	while ((c = getopt_long(argc, argv, "vmct", longopts, &idx)) != -1) {
		switch (c) {
			case 'v':
				verbose = YES_VERBOSE;
				break;
				
			case 'm':
				median = YES_MEDIAN;
				break;
				
			case 'c':
				client_info = YES_CLIENT_INFO;
				break;
				
			case 't':
				time_travelers = YES_TIME_TRAVELERS;
				break;
				
			default:
				break;
		} // switch
	} // while
} // parse_command_line_input

Order* create_order_from_input(string* str, const int NUM_CLIENTS,
							   const int NUM_EQUITIES, int current_time) {
	int t_stamp = 0, c_id = 0, e_id = 0, price = 0, quant = 0;
	Order::Transaction action = Order::NONE;
	
	int current_idx = 0;
	for (int i = 0; i <= (int)str->length(); ++i) {
		if ((str->at(i) == ' ') || (i == (int)str->length())) {
			switch (str->at(current_idx)) {
				case 'C':
					c_id = stoi(str->substr(current_idx + 1, i - current_idx - 1));
					break;
					
				case 'B':
					action = Order::BUY;
					break;
					
				case 'S':
					action = Order::SELL;
					break;
					
				case 'E':
					e_id = stoi(str->substr(current_idx + 1, i - current_idx - 1));
					break;
					
				case '$':
					price = stoi(str->substr(current_idx + 1, i - current_idx - 1));
					break;
					
				case '#':
					quant = stoi(str->substr(current_idx + 1, i - current_idx - 1));
					break;
					
				default:
					t_stamp = stoi(str->substr(current_idx, i - current_idx));
					break;
			} // switch
			
			current_idx = i + 1;
		} // if
	} // for
	
	// Check that timestamp isn't bad
	if (t_stamp < current_time) {
		cerr << "Timestamp for an order was less than current time. exit(1)" << endl;
		exit(1);
	}
	
	return new Order(t_stamp, c_id, action, e_id, price, quant,
					 NUM_CLIENTS, NUM_EQUITIES);
} // create_order_from_input

void make_matches(vector <Orderpq>* market, Order* order) {
	Orderpq market_cpy = market->at(order->getEquityId());
	
	while ((order->getQuantity() != 0) || (!market_cpy.empty())) {
		if (can_trade(order, market_cpy.top())) {
			
		}
		
		
	}
	
	return;
} // make_matches

bool can_trade(Order* ord1, Order* ord2) {
	
	if (ord1->getTransaction() != ord2->getTransaction()) {
		if (ord1->getTransaction() == Order::BUY) {
			if (ord1->getPrice() >= ord2->getPrice()) {
				return true;
			} // if
			
			return false;
		} // if
		else {
			if (ord1->getPrice() <= ord2->getPrice()) {
				return true;
			} // if
			
			return false;
		} // else
	}
	else {
		return false;
	}
} // can_trade

void output_verbose() {
	
	
	return;
} // output_verbose

void output_median() {
	
	
	return;
} // output_median

void output_client_info() {
	
	
	return;
} // output_client_info

void output_time_travelers() {
	
	
	return;
} // output_time_travelers