//
//  test-A-jmw.h
//  EECS 281 Project 2
//
//  Created by John West on 2/10/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#ifndef __EECS_281_Project_2__test_A_jmw__
#define __EECS_281_Project_2__test_A_jmw__

#include <stdio.h>
#include <queue>

#include "Order.h"

typedef priority_queue <Order*, vector <Order*>, BuyOrderComparison> Buypq;
typedef priority_queue <Order*, vector <Order*>, SellOrderComparison> Sellpq;

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
							   const int NUM_EQUITIES, int current_time, int orders_processed);

void make_matches(vector <Sellpq>* s_market, vector <Buypq>* b_market, vector< vector<int> >* median, Order* order, Verbose &verbose);

bool can_trade(Order* ord1, Order* ord2);

void output_verbose();

void output_median(vector< vector<int> >* median);

void output_client_info();

void output_time_travelers();

#endif /* defined(__EECS_281_Project_2__text_A_jmw__) */
