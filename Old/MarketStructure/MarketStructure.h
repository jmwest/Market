//
//  MarketStructure.h
//  EECS 281 Project 2
//
//  Created by John West on 2/8/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#ifndef __EECS_281_Project_2__MarketStructure__
#define __EECS_281_Project_2__MarketStructure__

#include <stdio.h>
#include <vector>
#include <list>

#include "Order.h"

class MarketStructure {
public:
	MarketStructure();
	MarketStructure(int structure_size);

	~MarketStructure();

	void add_order(Order* order);
	void make_matches(Order* order, bool verbose);

private:
	vector <list <Order*> >* market_structure;
	vector <vector <int> >* median_structure;
};

#endif /* defined(__EECS_281_Project_2__MarketStructure__) */
