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
#include <getopt.h>

#include <string>

using namespace std;

int main(int argc, char *argv[]) {

	// valgrind will report memory leak when sync_with_stdio is false //
	ios_base::sync_with_stdio(false);								 ///
	////////////////////////////////////////////////////////////////////

	// Declare variables
	vector <Sellpq> sell_market;
	vector <Buypq> buy_market;
	vector <Client> client_list;
	vector <Equity> equity_list;

	MarketMode mode = NONE;
	Verbose verbose = NO_VERBOSE;
	Median median = NO_MEDIAN;
	ClientInfo client_info = NO_CLIENT_INFO;
	TimeTravelers time_travelers = NO_TIME_TRAVELERS;

	string input_str, clients_str, equities_str;
	stringstream in_ss;
	ostringstream out_ss;

	int current_timestamp = 0;
	int orders_processed = 0;
	int total_orders = 0;

	// Command line input
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

	sell_market = vector <Sellpq> (NUM_EQUITIES);
	buy_market = vector <Buypq> (NUM_EQUITIES);
	client_list = vector <Client> (NUM_CLIENTS);
	equity_list = vector <Equity> (NUM_EQUITIES);

	// initialize clients and equities
	for (int i = 0; i < NUM_CLIENTS; ++i) {
		client_list.at(i) = Client(i);
	}

	for (int i = 0; i < NUM_EQUITIES; ++i) {
		equity_list.at(i) = Equity(i);
	}

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

	out_ss << "Processing orders...\n";

	// Run the market
	while (getline(input_stream, input_str)) {
		Order order = create_order_from_input(&input_str, &client_list, &equity_list,
												   current_timestamp, total_orders);
		++total_orders;

		// for time traveler
		if (time_travelers == YES_TIME_TRAVELERS) {
			if (order.get_transaction() == Order::SELL) {
				equity_list.at(order.get_equity()->get_equity_id()).add_min(order.get_timestamp(), order.get_price());
			}
			else {
				equity_list.at(order.get_equity()->get_equity_id()).add_max(order.get_timestamp(), order.get_price());
			}
		}

		if (current_timestamp != order.get_timestamp()) {

			if (median == YES_MEDIAN) {
				output_median(&equity_list, current_timestamp, out_ss);
			} // if

			current_timestamp = order.get_timestamp();
		} // if

		make_matches(&sell_market, &buy_market, &order, &client_list, verbose, median, orders_processed, out_ss);
	} // while


	// Deal with end of program output
	if (median == YES_MEDIAN) {
		output_median(&equity_list, current_timestamp, out_ss);
	} // if

	output_summary(orders_processed, &out_ss);

	if (client_info == YES_CLIENT_INFO) {
		output_client_info(&client_list, &out_ss);
	} // if

	if (time_travelers == YES_TIME_TRAVELERS) {
		output_time_travelers(&equity_list, &out_ss);
	}

	cout << out_ss.str();

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

Order create_order_from_input(string* str, vector <Client> * clients, vector <Equity> * equities,
							   int current_time, int total_orders) {
	int t_stamp = 0, c_id = 0, e_id = 0, price = 0, quant = 0;
	Order::Transaction action = Order::NONE;

	int current_idx = 0;
	for (int i = 1; i <= (int)str->length(); ++i) {
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
		cerr << "Timestamp for an order was less than current time: exit(1)" << endl;
		exit(1);
	}
	if ((c_id < 0) || (int(clients->size()) <= c_id)) {
		cerr << "Client_id for an order was out of range: exit(1)" << endl;
		exit(1);
	}
	if ((e_id < 0) || (int(equities->size()) <= e_id)) {
		cerr << "Equity_id for an order was out of range: exit(1)" << endl;
		exit(1);
	}

	return Order(t_stamp, &clients->at(c_id), action, &equities->at(e_id), price,
					 quant, total_orders, int(clients->size()), int(equities->size()));
} // create_order_from_input

void make_matches(vector <Sellpq>* s_market, vector <Buypq>* b_market, Order* order,
				  vector <Client>* clients, Verbose &verbose, Median &median,
				  int &orders_processed, ostringstream &ss) {
	
	int current_client = order->get_client()->get_client_id();
	int current_equity = order->get_equity()->get_equity_id();

	if (order->get_transaction() == Order::BUY) {

		Sellpq* market_ptr = &s_market->at(current_equity);

		queue <Order> market_cpy;

		while (order->get_quantity() && !market_ptr->empty()) {

			if (order->get_price() < market_ptr->top().get_price()) {
				break;
			}

			if (can_trade(order, &const_cast<Order&>(market_ptr->top()))) {
				++orders_processed;

				int market_client = market_ptr->top().get_client()->get_client_id();

				int equity_bought = (order->get_quantity() <= market_ptr->top().get_quantity()) ? order->get_quantity() : market_ptr->top().get_quantity();
				int price_bought = market_ptr->top().get_price();
				int total_spent = equity_bought * price_bought;

				order->change_quantity(equity_bought);
				const_cast<Order&>(market_ptr->top()).change_quantity(equity_bought);

				clients->at(current_client).add_bought(equity_bought);
				clients->at(current_client).add_net_value(-total_spent);
				clients->at(market_client).add_sold(equity_bought);
				clients->at(market_client).add_net_value(total_spent);

				if (median == YES_MEDIAN) {
					order->get_equity()->add_price(price_bought);
				}

				if (verbose == YES_VERBOSE) {
					output_verbose(current_client, market_client, current_equity, equity_bought,
								   price_bought, ss);
				} // if verbose == YES_VERBOSE
			} // if can_trade(order, market_cpy.top()

			if (const_cast<Order&>(market_ptr->top()).get_quantity()) {
				market_cpy.push(const_cast<Order&>(market_ptr->top()));
			}

			market_ptr->pop();
		} // while

		while (!market_cpy.empty()) {
				market_ptr->push(market_cpy.front());
				market_cpy.pop();
		}

		if (order->get_quantity()) {
			b_market->at(current_equity).push(*order);
		} // if
	}
	else {

		Buypq* market_ptr = &b_market->at(current_equity);

		queue <Order> market_cpy;

		while (!market_ptr->empty() && order->get_quantity()) {

			if (order->get_price() > market_ptr->top().get_price()) {
				break;
			}

			if (can_trade(order, &const_cast<Order&>(market_ptr->top()))) {
				++orders_processed;

				int market_client = market_ptr->top().get_client()->get_client_id();

				int equity_bought = (order->get_quantity() <= market_ptr->top().get_quantity()) ? order->get_quantity() : market_ptr->top().get_quantity();
				int price_bought = market_ptr->top().get_price();
				int total_spent = equity_bought * price_bought;

				order->change_quantity(equity_bought);
				const_cast<Order&>(market_ptr->top()).change_quantity(equity_bought);

				clients->at(current_client).add_sold(equity_bought);
				clients->at(current_client).add_net_value(total_spent);
				clients->at(market_client).add_bought(equity_bought);
				clients->at(market_client).add_net_value(-total_spent);

				if (median == YES_MEDIAN) {
					order->get_equity()->add_price(price_bought);
				}
				
				if (verbose == YES_VERBOSE) {
					output_verbose(market_client, current_client, current_equity, equity_bought, price_bought, ss);
				} // if verbose == YES_VERBOSE
			} // if can_trade(order, market_cpy.top()

			if (market_ptr->top().get_quantity()) {
				market_cpy.push(market_ptr->top());
			}

			market_ptr->pop();
		} // while

		while (!market_cpy.empty()) {
				market_ptr->push(market_cpy.front());
				market_cpy.pop();
		}

		if (order->get_quantity()) {
			s_market->at(current_equity).push(*order);
		} // if
	}

	return;
} // make_matches

bool can_trade(Order* ord1, Order* ord2) {

	if ((ord1->get_quantity() == 0) || (ord2->get_quantity() == 0)) {
		return false;
	}

	if (!(ord1->get_transaction() == ord2->get_transaction())) {
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
					int price, ostringstream &ss) {

	ss << "Client " << buying_client << " purchased " << num_shares << " shares of Equity " << equity << " from Client " << selling_client << " for $" << price << "/share\n";

	return;
} // output_verbose

void output_median(vector <Equity>* equities, int timestamp, ostringstream &ss) {

	for (int i = 0; i < int(equities->size()); ++i) {
		int median = equities->at(i).get_median();
		if (median != -1) {
			ss << "Median match price of Equity " << i << " at time " << timestamp << " is $" << median << "\n";
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

void output_time_travelers(vector <Equity>* equities, ostringstream* ss) {

	*ss << "---Time Travelers---\n";

	int buy_time, sell_time;
	for (int i = 0; i < int(equities->size()); ++i) {

		if ((equities->at(i).get_min_time() == -1) || (equities->at(i).get_max_time() == -1)) {
			buy_time = sell_time = -1;
		}
		else {
			buy_time = equities->at(i).get_min_time();
			sell_time = equities->at(i).get_max_time();
		}

		*ss << "A time traveler would buy shares of Equity " << i << " at time: " << buy_time << " and sell these shares at time: " << sell_time << "\n";
	}

	return;
} // output_time_travelers
