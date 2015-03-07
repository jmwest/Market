//
//  Equity.h
//  EECS 281 Project 2
//
//  Created by John West on 3/1/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#ifndef EECS_281_Project_2_Equity_h
#define EECS_281_Project_2_Equity_h

#include <vector>
#include <algorithm>

using namespace std;

class Equity {
private:
	struct TimeTraveler {
		int time;
		int price;
	};

	int equity_id;
	TimeTraveler current_min;
	TimeTraveler next_min;
	TimeTraveler max;
	vector <int> match_prices;

public:
	Equity();
	Equity(int e_id);

	int get_equity_id();

	void add_price(int price);
	void add_min(int timestamp, int price);
	void add_max(int timestamp, int price);
	int get_median();
	int get_min_time();
	int get_max_time();
};

Equity::Equity()
: equity_id(0)
{
	current_min.time = next_min.time = -1;
	current_min.price = next_min.price = 0;
	max.time = -1;
}

Equity::Equity(int e_id)
: equity_id(e_id)
{
	current_min.time = next_min.time = -1;
	current_min.price = next_min.price = 0;
	max.time = -1;
}

int Equity::get_equity_id() {

	return equity_id;
}

void Equity::add_price(int price) {

	vector<int>::iterator it = upper_bound(match_prices.begin(), match_prices.end(), price);

	match_prices.insert(it, price);

	return;
}

void Equity::add_min(int timestamp, int price) {

	if (current_min.time == -1) {
		current_min.price = price;
		current_min.time = timestamp;
	}
	else if (price < current_min.price) {
		if (max.time == -1) {
			current_min.price = price;
			current_min.time = timestamp;
		}
		else if (next_min.time == -1) {
			next_min.price = price;
			next_min.time = timestamp;
		}
		else if (price < next_min.price) {
			next_min.price = price;
			next_min.time = timestamp;
		}
	}

	return;
}

void Equity::add_max(int timestamp, int price) {

	if (current_min.time != -1) {

		if (((next_min.time != -1) && (price > next_min.price)) || (price > current_min.price)) {

			if (price == max.price) {

				if (next_min.time != -1) {
					max.time = timestamp;
					max.price = price;

					current_min.time = next_min.time;
					current_min.price = next_min.price;
					next_min.time = -1;
				}
			}
			else if (price > max.price) {

				max.time = timestamp;
				max.price = price;

				if (next_min.time != -1) {
					current_min.time = next_min.time;
					current_min.price = next_min.price;
					next_min.time = -1;
				}
			}
		}
	}

	return;
}

int Equity::get_median() {

	if (match_prices.size() == 0) {
		return -1;
	}
	else if (match_prices.size() == 1) {
		return match_prices.at(0);
	}
	else if (match_prices.size() % 2) {
		return match_prices.at(match_prices.size() / 2);
	}
	else {
		int one = match_prices.at((match_prices.size() / 2) - 1);
		int two = match_prices.at(match_prices.size() / 2);

		return (one + two) / 2;
	}
}

int Equity::get_min_time() {

	return current_min.time;
}

int Equity::get_max_time() {

	return max.time;
}

#endif
