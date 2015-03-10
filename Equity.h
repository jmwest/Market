//
//  Equity.h
//  EECS 281 Project 2
//
//  Created by John West on 3/1/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#ifndef EECS_281_Project_2_Equity_h
#define EECS_281_Project_2_Equity_h

#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

// This is the Equity class. It contains the min and max values
// for the time traveler output, along with two priority queues
// that contain the median data for the Equity.
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
	priority_queue <int, vector <int>, greater <int> > upper_match_prices;
	priority_queue <int, vector <int>, less <int> > lower_match_prices;

public:
	Equity();
	Equity(int e_id);

	int get_equity_id();

	void add_price(int price_in);
	void add_min(int timestamp, int price_in);
	void add_max(int timestamp, int price_in);
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
	max.price = 0;
}

Equity::Equity(int e_id)
: equity_id(e_id)
{
	current_min.time = next_min.time = -1;
	current_min.price = next_min.price = 0;
	max.time = -1;
	max.price = 0;
}

int Equity::get_equity_id() {

	return equity_id;
}

void Equity::add_price(int price_in) {

	if (!lower_match_prices.empty() && !upper_match_prices.empty()) {
		if (lower_match_prices.size() == upper_match_prices.size()) {
			if (upper_match_prices.top() <= price_in) {
				upper_match_prices.push(price_in);
			}
			else {
				lower_match_prices.push(price_in);
			}
		}
		else {
			if (upper_match_prices.size() > lower_match_prices.size()) {
				if (upper_match_prices.top() < price_in) {
					lower_match_prices.push(const_cast<int&>(upper_match_prices.top()));
					upper_match_prices.pop();
					upper_match_prices.push(price_in);
				}
				else {
					lower_match_prices.push(price_in);
				}
			}
			else {
				if (lower_match_prices.top() > price_in) {
					upper_match_prices.push(const_cast<int&>(lower_match_prices.top()));
					lower_match_prices.pop();
					lower_match_prices.push(price_in);
				}
				else {
					upper_match_prices.push(price_in);
				}
			}
		}
	}
	else if (!upper_match_prices.empty()) {
		lower_match_prices.push(price_in);
	}
	else {
		upper_match_prices.push(price_in);
	}

	return;
}

void Equity::add_min(int timestamp, int price_in) {

	if (current_min.time == -1) {
		current_min.price = price_in;
		current_min.time = timestamp;
	}
	else if (price_in < current_min.price) {
		if (max.time == -1) {
			current_min.price = price_in;
			current_min.time = timestamp;
		}
		else if (next_min.time == -1) {
			next_min.price = price_in;
			next_min.time = timestamp;
		}
		else if (price_in < next_min.price) {
			next_min.price = price_in;
			next_min.time = timestamp;
		}
	}

	return;
}

void Equity::add_max(int timestamp, int price_in) {

	if (current_min.time != -1) {

		if (((next_min.time != -1) && (price_in > next_min.price)) || (price_in > current_min.price)) {

			if (price_in == max.price) {

				if (next_min.time != -1) {
					max.time = timestamp;
					max.price = price_in;

					current_min.time = next_min.time;
					current_min.price = next_min.price;
					next_min.time = -1;
				}
			}
			else if (price_in > max.price) {

				max.time = timestamp;
				max.price = price_in;

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

	if (!lower_match_prices.empty() && !upper_match_prices.empty()) {
		if (lower_match_prices.size() == upper_match_prices.size()) {
			return (upper_match_prices.top() + lower_match_prices.top()) / 2;
		}
		else if (lower_match_prices.size() < upper_match_prices.size()) {
			return upper_match_prices.top();
		}
		else {
			return lower_match_prices.top();
		}
	}
	else if (!upper_match_prices.empty()) {
		return upper_match_prices.top();
	}
	else {
		return -1;
	}
}

int Equity::get_min_time() {

	return current_min.time;
}

int Equity::get_max_time() {

	return max.time;
}

#endif
