//
//  market.cpp
//  EECS 281 Project 2
//
//  Created by John West on 2/7/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#include "market.h"
#include "Order.h"
#include "MarketStructure.h"
#include "P2.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <getopt.h>

#include <string>

using namespace std;

enum MarketMode {NONE, TRADELIST, PSEUDORANDOM};
enum Verbose {NO_VERBOSE, YES_VERBOSE};
enum Median {NO_MEDIAN, YES_MEDIAN};
enum ClientInfo {NO_CLIENT_INFO, YES_CLIENT_INFO};
enum TimeTravelers {NO_TIME_TRAVELERS, YES_TIME_TRAVELERS};

// REQUIRES:
// MODIFIES: begin, end, rout, modify, and output.
// EFFECTS: uses argc and argv that are passed into the program
//			to give values to begin, end, rout, modify, and output.
void parse_command_line_input(int & argc, char *argv[], Verbose &verbose, Median &median,
							  ClientInfo &client_info, TimeTravelers &time_travelers);

Order* create_order_from_input(string* str, const int NUM_CLIENTS,
							   const int NUM_EQUITIES, int current_time);

void output_verbose();

void output_median();

void output_client_info();

void output_time_travelers();

int main(int argc, char *argv[]) {

	// valgrind will report memory leak when sync_with_stdio is false//
	ios_base::sync_with_stdio(false);								///
	///////////////////////////////////////////////////////////////////

	// Declare variables
	MarketStructure stock_market;

	vector< list <int> >* median_list;

	MarketMode mode = NONE;
	Verbose verbose = NO_VERBOSE;
	Median median = NO_MEDIAN;
	ClientInfo client_info = NO_CLIENT_INFO;
	TimeTravelers time_travelers = NO_TIME_TRAVELERS;

	string input_str, clients_str, equities_str;
	stringstream in_ss;
	ostringstream out_ss;

	int current_timestamp = 0;

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

	stock_market = MarketStructure(NUM_EQUITIES);

	// cerr input // FOR TESTING ONLY, REMOVE LATER ///
//	if (mode == TRADELIST) {
//		cerr << "TL\nNUM_CLIENTS: " << NUM_CLIENTS << "\nNUM_EQUITIES: " << NUM_EQUITIES << endl;
//	}
	//////////////////////////////////////////////////

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

		// cerr input // FOR TESTING ONLY, REMOVE LATER //
//		cerr << "PR\nNUM_CLIENTS: " << NUM_CLIENTS << "\nNUM_EQUITIES: " << NUM_EQUITIES;
//		cerr << "\nseed: " << seed << "\nnum_orders: " << number_of_orders << "\narrival_rate: " << arrival_rate << endl;
		//////////////////////////////////////////////////
	}

	istream& input_stream = (mode == TRADELIST) ? cin : in_ss;

	cout << "Processing orders...\n";
	// Run the market
	while (getline(input_stream, input_str)) {
		Order* order_ptr = create_order_from_input(&input_str, NUM_CLIENTS, NUM_EQUITIES,
												   current_timestamp);
		if (current_timestamp != order_ptr->getTimestamp()) {
			// add more for median in the future
			if (median == YES_MEDIAN) {
				output_median();
			}

			current_timestamp = order_ptr->getTimestamp();
		}

		stock_market.make_matches(order_ptr, verbose == YES_VERBOSE);
	}

	if (median_list) {
		delete median_list; median_list = NULL;
	}

	return 0;
}

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
		}
	}
}

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
			}

			current_idx = i + 1;
		}
	}

	// Check that timestamp isn't bad
	if (t_stamp < current_time) {
		cerr << "Timestamp for an order was less than current time. exit(1)" << endl;
		exit(1);
	}

	return new Order(t_stamp, c_id, action, e_id, price, quant,
					 NUM_CLIENTS, NUM_EQUITIES);
}

void output_verbose() {
	

	return;
}

void output_median() {
	

	return;
}

void output_client_info() {
	

	return;
}

void output_time_travelers() {
	

	return;
}
