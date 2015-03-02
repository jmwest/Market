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
	int equity_id;
	vector <int> match_prices;

public:
	Equity();
	Equity(int e_id);

	int get_equity_id();

	void add_price(int price);
	int get_median();
	int get_matches();
};

Equity::Equity()
: equity_id(0) {}

Equity::Equity(int e_id)
: equity_id(e_id) {}

int Equity::get_equity_id() {

	return equity_id;
}

void Equity::add_price(int price) {
	
	vector<int>::iterator it = lower_bound(match_prices.begin(), match_prices.end(), price);
	
	match_prices.insert(it, price);
	
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

int Equity::get_matches() {
	
	return int(match_prices.size());
}

#endif
