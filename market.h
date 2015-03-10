//
//  market.h
//  EECS 281 Project 2
//
//  Created by John West on 2/7/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#ifndef __EECS_281_Project_2__market__
#define __EECS_281_Project_2__market__

#include <stdio.h>
#include <queue>
#include <sstream>

#include "Order.h"
#include "Client.h"
#include "Equity.h"

typedef priority_queue <Order, vector <Order>, BuyOrderComparison> Buypq;
typedef priority_queue <Order, vector <Order>, SellOrderComparison> Sellpq;

enum MarketMode {NONE, TRADELIST, PSEUDORANDOM};
enum Verbose {NO_VERBOSE, YES_VERBOSE};
enum Median {NO_MEDIAN, YES_MEDIAN};
enum ClientInfo {NO_CLIENT_INFO, YES_CLIENT_INFO};
enum TimeTravelers {NO_TIME_TRAVELERS, YES_TIME_TRAVELERS};

// REQUIRES:
// MODIFIES: verbose, median, client_info, and time_travelers.
// EFFECTS: uses argc and argv that are passed into the program
//			to give values to verbose, median, client_info, and time_travelers.
void parse_command_line_input(int & argc, char *argv[], Verbose &verbose, Median &median,
							  ClientInfo &client_info, TimeTravelers &time_travelers);

// REQUIRES: current_time is >= 0, total_orders is >= 0.
//			 clients and equities are properly instantiated pointers.
// MODIFIES:
// EFFECTS: returns an order created from str.
Order create_order_from_input(string* str, vector <Client>* clients, vector <Equity>* equities,
							   int current_time, int total_orders);

// REQUIRES: order, s_market, and b_market are valid ptrs.
//			 ss is initialized.
// MODIFIES: s_market, b_market, order, clients, orders_processed, ss.
// EFFECTS: takes a new order and matches it up with previous orders
//			until its quantity is depleted or no more matches can be made.
void make_matches(vector <Sellpq>* s_market, vector <Buypq>* b_market, Order* order,
				  vector <Client>* clients, Verbose &verbose, Median &median,
				  int &orders_processed, ostringstream &ss);

// REQUIRES: ord1 and ord2 are non-null pointers.
// MODIFIES:
// EFFECTS: compares ord1 and ord2 and returns true if they can trade.
bool can_trade(Order* ord1, Order* ord2);

// REQUIRES: ss is a valid ostringstream
// MODIFIES: ss.
// EFFECTS: passes verbose output to ss.
void output_verbose(int buying_client, int selling_client, int equity, int num_shares,
					int price, ostringstream &ss);

// REQUIRES: ss is a valid ostringstream
// MODIFIES: ss.
// EFFECTS: passes median output to ss.
void output_median(vector <Equity>* equities, int timestamp, ostringstream &ss);

// REQUIRES: ss is a valid ostringstream
// MODIFIES: ss.
// EFFECTS: passes summary output to ss.
void output_summary(int orders_processed, ostringstream* ss);

// REQUIRES: ss is a valid ostringstream
// MODIFIES: ss.
// EFFECTS: passes client info output to ss.
void output_client_info(vector <Client>* clients, ostringstream* ss);

// REQUIRES: ss is a valid ostringstream
// MODIFIES: ss.
// EFFECTS: passes time traveler output to ss.
void output_time_travelers(vector <Equity>* equities, ostringstream* ss);

#endif /* defined(__EECS_281_Project_2__market__) */
