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
	Client c0 = Client(0);
	Client c1 = Client(1);
	Client c2 = Client(2);

	Equity e0 = Equity(0);

	// add bought, sold, and net_value to client
	c0.add_bought(2);
	c1.add_sold(1);
	c2.add_net_value(100);

	e0.add_price(10);
	e0.add_price(3);
	e0.add_price(10);

	cout << "e0 median: " << e0.get_median() << endl;
	assert(e0.get_median() == 10);

	assert(c0.get_bought() == 2);
	assert(c1.get_sold() == 1);
	assert(c2.get_net_value() == 100);

	Order ord1 = Order(0, &c1, Order::BUY, &e0, 10, 1, 0, 3, 1);
	Order ord2 = Order(1, &c0, Order::SELL, &e0, 5, 2, 1, 3, 1);
	Order ord3 = Order(0, &c2, Order::SELL, &e0, 1, 1, 2, 3, 1);
	Order ord4 = Order(1, &c2, Order::BUY, &e0, 5, 1, 3, 3, 1);
	Order ord5 = Order(2, &c2, Order::BUY, &e0, 10, 1, 4, 3, 1);
	Order ord6 = Order(0, &c0, Order::BUY, &e0, 1, 1, 5, 3, 1);

	// changing one changes em all
	ord3.get_client()->add_bought(7);
	assert(ord5.get_client()->get_bought() == 7);

	// create_order_from_input tests //
	// ord1
	assert(ord1.get_timestamp() == 0);
	assert(ord1.get_client()->get_client_id() == 1);
	assert(ord1.get_transaction() == Order::BUY);
	assert(ord1.get_equity()->get_equity_id() == 0);
	assert(ord1.get_price() == 10);
	assert(ord1.get_quantity() == 1);

	// ord2
	assert(ord2.get_timestamp() == 1);
	assert(ord2.get_client()->get_client_id() == 0);
	assert(ord2.get_transaction() == Order::SELL);
	assert(ord2.get_equity()->get_equity_id() == 0);
	assert(ord2.get_price() == 5);
	assert(ord2.get_quantity() == 2);

	// BuyOrderpq Tests //
	Buypq q = Buypq();

	q.push(&ord1);
	q.push(&ord4);
	q.push(&ord5);
	q.push(&ord6);

	assert(q.top()->get_price() == 10);
	assert(q.top()->get_client()->get_client_id() == 1);
	assert(q.top()->get_timestamp() == 0);
	cout << "ord1: " << q.top()->get_client()->get_client_id() << " " << q.top()->get_price() << endl;
	q.pop();

	assert(q.top()->get_price() == 10);
	assert(q.top()->get_client()->get_client_id() == 2);
	assert(q.top()->get_timestamp() == 2);
	cout << "ord5: " << q.top()->get_client()->get_client_id() << " " << q.top()->get_price() << endl;
	q.pop();

	assert(q.top()->get_price() == 5);
	assert(q.top()->get_client()->get_client_id() == 2);
	cout << "ord4: " << q.top()->get_client()->get_client_id() << " " << q.top()->get_price() << endl;
	q.pop();

	assert(q.top()->get_price() == 1);
	cout << "ord6: " << q.top()->get_client()->get_client_id() << " " << q.top()->get_price() << endl;
	q.pop();

	// multiple same price same time //

	Order b1 = Order(1, &c0, Order::BUY, &e0, 5, 1, 1, 3, 1);
	Order b2 = Order(1, &c1, Order::BUY, &e0, 5, 1, 2, 3, 1);

	q.push(&b1);
	q.push(&ord4);
	q.push(&b2);

	assert(q.top()->get_client()->get_client_id() == 0);
	q.pop();

	assert(q.top()->get_client()->get_client_id() == 1);
	q.pop();

	assert(q.top()->get_client()->get_client_id() == 2);
	q.pop();

	///////////////////////////////////////////

	Order ord7 = Order(3, &c2, Order::SELL, &e0, 20, 1, 6, 3, 1);
	Order ord8 = Order(1, &c1, Order::SELL, &e0, 5, 1, 7, 3, 1);

	// SellOrderpq Tests //
	Sellpq p = Sellpq();
	p.push(&ord2);
	p.push(&ord3);
	p.push(&ord7);
	p.push(&ord8);

	assert(p.top()->get_price() == 1);
	assert(p.top()->get_client()->get_client_id() == 2);
	assert(p.top()->get_timestamp() == 0);
	cout << "\nord3: " << p.top()->get_client()->get_client_id() << " " << p.top()->get_price() << endl;
	p.pop();
	
	assert(p.top()->get_price() == 5);
	assert(p.top()->get_client()->get_client_id() == 0);
	assert(p.top()->get_timestamp() == 1);
	cout << "ord2: " << p.top()->get_client()->get_client_id() << " " << p.top()->get_price() << endl;
	p.pop();
	
	assert(p.top()->get_price() == 5);
	assert(p.top()->get_client()->get_client_id() == 1);
	assert(p.top()->get_timestamp() == 1);
	cout << "ord8: " << p.top()->get_client()->get_client_id() << " " << p.top()->get_price() << endl;
	p.pop();
	
	assert(p.top()->get_price() == 20);
	cout << "ord7: " << p.top()->get_client()->get_client_id() << " " << p.top()->get_price() << endl;
	p.pop();

	////////////////////////////////////////////
	Order o1 = Order(1, &c0, Order::SELL, &e0, 5, 1, 1, 3, 1);
	Order o2 = Order(1, &c1, Order::SELL, &e0, 5, 1, 2, 3, 1);
	Order o3 = Order(1, &c2, Order::SELL, &e0, 5, 1, 3, 3, 1);

	p.push(&o1);
	p.push(&o2);
	p.push(&o3);

	assert(p.top()->get_client()->get_client_id() == 0);
	cout << "o1: " << p.top()->get_client()->get_client_id() << endl;
	p.pop();

	assert(p.top()->get_client()->get_client_id() == 1);
	cout << "o2: " << p.top()->get_client()->get_client_id() << endl;
	p.pop();

	assert(p.top()->get_client()->get_client_id() == 2);
	cout << "o3: " << p.top()->get_client()->get_client_id() << endl;
	p.pop();

	// can_trade test //
	assert(!can_trade(&ord1, &ord4));
	assert(can_trade(&ord1, &ord2));
	assert(can_trade(&ord1, &ord3));
	assert(!can_trade(&ord2, &ord3));
	assert(can_trade(&ord2, &ord4));
	assert(can_trade(&ord3, &ord4));
	assert(can_trade(&ord2, &ord5));

		// flipped //
	assert(!can_trade(&ord4, &ord1));
	assert(can_trade(&ord2, &ord1));
	assert(can_trade(&ord3, &ord1));
	assert(!can_trade(&ord3, &ord2));
	assert(can_trade(&ord4, &ord2));
	assert(can_trade(&ord4, &ord3));
	assert(can_trade(&ord5, &ord2));

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

Order* create_order_from_input(string* str, vector <Client> * clients, vector <Equity> * equities,
							   int current_time, int orders_processed) {
	int t_stamp = 0, c_id = 0, e_id = 0, price = 0, quant = 0;
	Order::Transaction action = Order::NONE;
	
	int current_idx = 0;
	for (int i = 0; i <= (int)str->length(); ++i) {
		if ((i == (int)str->length()) || (str->at(i) == ' ')) {
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
	
	return new Order(t_stamp, &clients->at(c_id), action, &equities->at(e_id), price,
					 quant, orders_processed, int(clients->size()), int(equities->size()));
} // create_order_from_input

void make_matches(vector <Sellpq>* s_market, vector <Buypq>* b_market, Order* order,
				  Verbose &verbose, int &orders_processed, ostringstream* ss) {
	cerr << "mm client: " << order->get_client()->get_client_id() << endl;
	cerr << "time: " << order->get_timestamp() << endl;
	cerr << "quantity: " << order->get_quantity() << endl;
	cerr << "equity: " << order->get_equity()->get_equity_id() << endl;
	
	if (order->get_transaction() == Order::BUY) {
		
		Client current_client = *order->get_client();
		Equity current_equity = *order->get_equity();
		cerr << "s1" << endl;
		
		Sellpq market_cpy = s_market->at(current_equity.get_equity_id());
		
		// remove any empty equity orders from the top of the pq.
		while (!market_cpy.empty() && !market_cpy.top()->get_quantity()) {
			market_cpy.pop();
		}
		cerr << "s2" << endl;
		
		while (order->get_quantity() && !market_cpy.empty()) {
			cerr << "s2.5" << endl;
			if (can_trade(order, market_cpy.top())) {
				++orders_processed;
				
				int equity_bought = (order->get_quantity() <= market_cpy.top()->get_quantity()) ? order->get_quantity() : market_cpy.top()->get_quantity();
				int price_bought = market_cpy.top()->get_price();
				int total_spent = equity_bought * price_bought;
				
				order->change_quantity(equity_bought);
				market_cpy.top()->change_quantity(equity_bought);
				
				current_client.add_bought(equity_bought);
				current_client.add_net_value(-total_spent);
				market_cpy.top()->get_client()->add_sold(equity_bought);
				market_cpy.top()->get_client()->add_net_value(total_spent);
				
				current_equity.add_price(price_bought);
				
				if (verbose == YES_VERBOSE) {
					output_verbose(current_client.get_client_id(), market_cpy.top()->get_client()->get_client_id(),
								   current_equity.get_equity_id(), equity_bought, price_bought, ss);
				} // if verbose == YES_VERBOSE
			} // if can_trade(order, market_cpy.top()
			
			market_cpy.pop();
		} // while
		cerr << "s3" << endl;
		
		if (order->get_quantity()) {
			s_market->at(current_equity.get_equity_id()).push(order);
		} // if
	}
	else {
		
		Client current_client = *order->get_client();
		Equity current_equity = *order->get_equity();
		
		Buypq market_cpy = b_market->at(order->get_equity()->get_equity_id());
		
		// remove any empty equity orders from the top of the pq.
		while (!market_cpy.empty() && !market_cpy.top()->get_quantity()) {
			market_cpy.pop();
		}
		cerr << "b2" << endl;
		
		while (!market_cpy.empty() && order->get_quantity()) {
			cerr << "b2.5" << endl;
			
			if (can_trade(order, market_cpy.top())) {
				++orders_processed;
				
				int equity_bought = (order->get_quantity() <= market_cpy.top()->get_quantity()) ? order->get_quantity() : market_cpy.top()->get_quantity();
				int price_bought = market_cpy.top()->get_price();
				int total_spent = equity_bought * price_bought;
				
				order->change_quantity(equity_bought);
				market_cpy.top()->change_quantity(equity_bought);
				
				current_client.add_sold(equity_bought);
				current_client.add_net_value(total_spent);
				market_cpy.top()->get_client()->add_bought(equity_bought);
				market_cpy.top()->get_client()->add_net_value(-total_spent);
				
				current_equity.add_price(price_bought);
				
				if (verbose == YES_VERBOSE) {
					output_verbose(market_cpy.top()->get_client()->get_client_id(), current_client.get_client_id(),
								   current_equity.get_equity_id(), equity_bought, price_bought, ss);
				} // if verbose == YES_VERBOSE
			} // if can_trade(order, market_cpy.top()
			
			market_cpy.pop();
		} // while
		
		cerr << "b3" << endl;
		
		if (order->get_quantity()) {
			b_market->at(current_equity.get_equity_id()).push(order);
		} // if
	}
	
	return;
} // make_matches

bool can_trade(Order* ord1, Order* ord2) {
	
	if ((ord1->get_quantity() == 0) || (ord2->get_quantity() == 0)) {
		return false;
	}
	
	if (ord1->get_transaction() != ord2->get_transaction()) {
		if (ord1->get_transaction() == Order::BUY) {
			if (ord1->get_price() >= ord2->get_price()) {
				return true;
			} // if
			
			return false;
		} // if
		else {
			if (ord1->get_price() <= ord2->get_price()) {
				return true;
			} // if
			
			return false;
		} // else
	}
	else {
		return false;
	}
} // can_trade

void output_verbose(int buying_client, int selling_client, int equity, int num_shares,
					int price, ostringstream* ss) {
	
	*ss << "Client " << buying_client << " purchased " << num_shares << " shares of Equity " << equity << " from Client " << selling_client << " for $" << price << "/share\n";
	
	return;
} // output_verbose

void output_median(vector <Equity>* equities, int timestamp, ostringstream* ss) {
	
	for (int i = 0; i < int(equities->size()); ++i) {
		if (equities->at(i).get_median() != -1) {
			*ss << "Median match price of Equity " << i << " at time " << timestamp << " is $" << equities->at(i).get_median() << "\n";
		} // if
	} // for
	
	return;
} // output_median

void output_summary(int orders_processed, ostringstream* ss) {
	
	*ss << "---End of Day---\nOrders Processed: " << orders_processed << "\n";
	
	return;
} // output_summary

void output_client_info(vector <Client>* clients, ostringstream* ss) {
	
	*ss << "---Client Info---\n";
	
	for (int i = 0; i < int(clients->size()); ++i) {
		*ss << "Client " << i << " bought " << clients->at(i).get_bought() << " and sold " << clients->at(i).get_sold() << " for a net transfer of $" << clients->at(i).get_net_value() << "\n";
	}
	
	return;
} // output_client_info






void output_time_travelers(ostringstream* ss) {
	
	*ss << "---Time Travelers---\n";
	
	
	
	return;
} // output_time_travelers
