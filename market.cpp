//
//  market.cpp
//  EECS 281 Project 2
//
//  Created by John West on 2/7/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#include "market.h"
#include "Order.h"
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



void output_verbose();

void output_median();

void output_client_info();

void output_time_travelers();

int main(int argc, char *argv[]) {

	// valgrind will report memory leak when sync_with_stdio is false//
	ios_base::sync_with_stdio(false);								///
	///////////////////////////////////////////////////////////////////

	MarketMode mode = NONE;
	Verbose verbose = NO_VERBOSE;
	Median median = NO_MEDIAN;
	ClientInfo client_info = NO_CLIENT_INFO;
	TimeTravelers time_travelers = NO_TIME_TRAVELERS;

	string input_str, clients_str, equities_str;
	stringstream ss;

	parse_command_line_input(argc, argv, verbose, median, client_info, time_travelers);

	getline(cin, input_str);

	if (input_str.at(6) == 'T') {
		mode = TRADELIST;
	}
	else {
		mode = PSEUDORANDOM;
	}

	getline(cin, input_str);

	clients_str = input_str.substr(13, input_str.length() - 14);
	const int NUM_CLIENTS = atoi(clients_str.c_str());

	getline(cin, input_str);

	equities_str = input_str.substr(14, input_str.length() - 15);
	const int NUM_EQUITIES = atoi(equities_str.c_str());

	if (mode == PSEUDORANDOM) {
		int seed;
		int number_of_orders;
		int arrival_rate;

		getline(cin, input_str);

		seed = atoi(input_str.substr(13, input_str.length() - 14).c_str());

		getline(cin, input_str);

		number_of_orders = atoi(input_str.substr(18, input_str.length() - 19).c_str());

		getline(cin, input_str);

		arrival_rate = atoi(input_str.substr(14, input_str.length() - 15).c_str());

		P2::PRstream(ss, seed, NUM_EQUITIES, NUM_CLIENTS, number_of_orders, arrival_rate);
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
	
	// Check for necessary conditions:

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
