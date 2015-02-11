//
//  market.cpp
//  EECS 281 Project 2
//
//  Created by John West on 2/7/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#include "market.h"
#include "P2.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <getopt.h>

#include <string>

using namespace std;

int main(int argc, char *argv[]) {

	// valgrind will report memory leak when sync_with_stdio is false//
	ios_base::sync_with_stdio(false);								///
	///////////////////////////////////////////////////////////////////

	// Declare variables
	vector <Orderpq> stock_market;

	// include when beginning median data
	vector< vector <int> >* median_list = new vector< vector<int> >;

	MarketMode mode = NONE;
	Verbose verbose = NO_VERBOSE;
	Median median = NO_MEDIAN;
	ClientInfo client_info = NO_CLIENT_INFO;
	TimeTravelers time_travelers = NO_TIME_TRAVELERS;

	string input_str, clients_str, equities_str;
	stringstream in_ss;
	ostringstream out_ss;

	int current_timestamp = 0;
//	int orders_processed = 0;

	parse_command_line_input(argc, argv, verbose, median, client_info, time_travelers);

	// Pull in initial info
	getline(cin, input_str);

	if (input_str.at(6) == 'T') {
		mode = TRADELIST;
	}
	else {
		mode = PSEUDORANDOM;
	}

	getline(cin, input_str);

	clients_str = input_str.substr(13, input_str.length() - 13);
	const int NUM_CLIENTS = atoi(clients_str.c_str());

	getline(cin, input_str);

	equities_str = input_str.substr(14, input_str.length() - 13);
	const int NUM_EQUITIES = atoi(equities_str.c_str());

	stock_market = vector <Orderpq> (NUM_EQUITIES);

	if (mode == PSEUDORANDOM) {
		int seed;
		int number_of_orders;
		int arrival_rate;

		getline(cin, input_str);

		seed = atoi(input_str.substr(13, input_str.length() - 13).c_str());

		getline(cin, input_str);

		number_of_orders = atoi(input_str.substr(18, input_str.length() - 18).c_str());

		getline(cin, input_str);

		arrival_rate = atoi(input_str.substr(14, input_str.length() - 14).c_str());

		P2::PR_init(in_ss, seed, NUM_EQUITIES, NUM_CLIENTS, number_of_orders, arrival_rate);
	} // if

	istream& input_stream = (mode == TRADELIST) ? cin : in_ss;

	cout << "Processing orders...\n";
	// Run the market
	while (getline(input_stream, input_str)) {
		Order* order_ptr = create_order_from_input(&input_str, NUM_CLIENTS, NUM_EQUITIES,
												   current_timestamp);
		if (current_timestamp != order_ptr->getTimestamp()) {
			// add more for median in the future
			if (median == YES_MEDIAN) {
				output_median(median_list);
			} // if

			current_timestamp = order_ptr->getTimestamp();
		} // if

		make_matches(&stock_market, median_list, order_ptr, verbose);
	} // while

	if (median_list) {
		delete median_list; median_list = NULL;
	}

	return 0;
} // main

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

void make_matches(vector <Orderpq>* market, vector< vector<int> >* median,
				  Order* order, Verbose &verbose) {

	Orderpq market_cpy = market->at(order->getEquityId());

	while ((order->getQuantity() != 0) || (!market_cpy.empty())) {

		if (can_trade(order, market_cpy.top())) {

			if (order->getTransaction() == Order::BUY) {

				int equity_bought = (order->getQuantity() <= market_cpy.top()->getQuantity()) ? order->getQuantity() : market_cpy.top()->getQuantity();

				order->changeQuantity(equity_bought);
				market_cpy.top()->changeQuantity(equity_bought);

				// add stuff for verbose
//				if (verbose == YES_VERBOSE) {
//					output_verbose();
//				}

			} // if order->getTransaction() == Order::BUY
			else {
				
			} // else
		} // if can_trade(order, market_cpy.top()

		
	}

	return;
} // make_matches

bool can_trade(Order* ord1, Order* ord2) {

	if ((ord1->getQuantity() == 0) || (ord2->getQuantity() == 0)) {
		return false;
	}

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

void output_median(vector< vector<int> >* median) {
	

	return;
} // output_median

void output_client_info() {
	

	return;
} // output_client_info

void output_time_travelers() {
	

	return;
} // output_time_travelers
