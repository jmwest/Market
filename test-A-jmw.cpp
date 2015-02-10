//
//  test-A-jmw.cpp
//  EECS 281 Project 2
//
//  Created by John West on 2/10/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#include "test-A-jmw.h"
#include "Order.h"

#include <cassert>
#include <iostream>
#include <queue>

typedef priority_queue <Order*, vector <Order*>, OrderComparison> Orderpq;


int main() {
	Order ord1 = Order(0, 1, Order::BUY, 0, 10, 1, 3, 1);
	Order ord2 = Order(1, 0, Order::BUY, 0, 5, 1, 3, 1);
	Order ord3 = Order(2, 2, Order::BUY, 0, 1, 1, 3, 1);
	Order ord4 = Order(1, 2, Order::BUY, 0, 5, 1, 3, 1);

	Orderpq q = Orderpq();

	q.push(&ord1);
	q.push(&ord2);
	q.push(&ord3);
	q.push(&ord4);

	assert(q.top()->getPrice() == 1);
	cout << "ord3: " << q.top()->getClientId() << " " << q.top()->getPrice() << endl;
	q.pop();

	assert(q.top()->getPrice() == 5);
	assert(q.top()->getClientId() == 0);
	cout << "ord2: " << q.top()->getClientId() << " " << q.top()->getPrice() << endl;
	q.pop();

	assert(q.top()->getPrice() == 5);
	assert(q.top()->getClientId() == 2);
	cout << "ord4: " << q.top()->getClientId() << " " << q.top()->getPrice() << endl;
	q.pop();

	assert(q.top()->getPrice() == 10);
	cout << "ord1: " << q.top()->getClientId() << " " << q.top()->getPrice() << endl;
	q.pop();

	return 0;
}