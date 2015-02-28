/*
	Compile this test against your .h files to make sure they compile. Note how the eecs281 priority queues can be constructed with the different types.  We suggest adding to this file or creating your own test cases to test your priority queue implementations. 
*/

#include "Eecs281PQ.h"
#include "SortedPQ.h"
#include "BinaryPQ.h"
#include "PairingPQ.h"
#include <vector>
#include <assert.h>
#include <iostream>

using namespace std;

void testSorted(vector<int>);
void testBinary(vector<int>);
void testPairing(vector<int>);

//int main(int argc, char * argv[])
int main()
{
	vector<int> vec;
	vec.push_back(0);
	vec.push_back(1);
	testSorted(vec);
	testBinary(vec);
	testPairing(vec);
}

void testSorted(vector<int> vec)
{
	Eecs281PQ<int> * pq1 = new SortedPQ<int>(vec.begin(), vec.end());
	
	assert(!pq1->empty());
	assert(pq1->top() == 1);
	assert(pq1->size() == 2);
	
	pq1->push(3);
	
	assert(pq1->top() == 3);
	assert(pq1->size() == 3);
	
	pq1->pop();
	
	assert(pq1->size() == 2);
	assert(!pq1->empty());
	assert(pq1->top() == 1);
	
	pq1->pop();
	pq1->pop();
	
	assert(pq1->empty());
	
	for (int i = 0; i < 100; ++i) {
		pq1->push(i);
		assert(int(pq1->size()) == i + 1);
	}
	
	for (int j = 99; j >= 0; --j) {
		assert(pq1->top() == j);
		assert(int(pq1->size()) == j + 1);
		pq1->pop();
	}
	
	assert(pq1->empty());

	delete pq1;
}

void testBinary(vector<int> vec)
{
	Eecs281PQ<int> * pq1 = new BinaryPQ<int>(vec.begin(), vec.end());

	assert(!pq1->empty());
	assert(pq1->top() == 1);
	assert(pq1->size() == 2);

	pq1->push(3);

	assert(pq1->top() == 3);
	assert(pq1->size() == 3);

	pq1->pop();

	assert(pq1->size() == 2);
	assert(!pq1->empty());
	assert(pq1->top() == 1);

	pq1->pop();
	pq1->pop();

	assert(pq1->empty());

	for (int i = 0; i < 100; ++i) {
		pq1->push(i);
		assert(int(pq1->size()) == i + 1);
	}

	for (int j = 99; j >= 0; --j) {
		assert(pq1->top() == j);
		assert(int(pq1->size()) == j + 1);
		pq1->pop();
	}

	assert(pq1->empty());

	delete pq1;
}

void testPairing(vector<int> vec)
{
	cerr << "Pairing\n";

	Eecs281PQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());

	cerr << "Iterated" << endl;

	Eecs281PQ<int> * pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));

	cerr << "CC'ed" << endl;

	Eecs281PQ<int> * pq3 = new PairingPQ<int>();

	cerr << "constructed" << endl;

	pq3 = pq2;
	pq1->push(3);
	pq2->pop();
	pq1->size();
	pq1->empty();
	assert(pq1->top() == 3);

	delete pq1;
	delete pq2;
	delete pq3;
}


