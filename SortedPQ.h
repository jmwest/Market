#ifndef SORTED_PQ_H
#define SORTED_PQ_H

#include "Eecs281PQ.h"

#include <algorithm>

//A specialized version of the 'priority_queue' ADT that is implemented with an
//underlying sorted array-based container.
//Note: The most extreme element should be found at the end of the
//'data' container, such that traversing the iterators yields the elements in
//sorted order.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class SortedPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
public:
  typedef unsigned size_type;

  //Description: Construct a priority_queue out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n log n) where n is number of elements in range.
  template<typename InputIterator>
  SortedPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Construct an empty priority_queue with an optional comparison functor.
  //Runtime: O(1)
  SortedPQ(COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Add a new element to the priority_queue.
  //Runtime: O(n)
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the priority_queue.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the priority_queue is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: Amortized O(1)
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the priority_queue.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the priority_queue. This has been
  //             implemented for you.
  //Runtime: O(1)
  virtual size_type size() const { return data.size(); }

  //Description: Return true if the priority_queue is empty. This has been implemented
  //             for you.
  //Runtime: O(1)
  virtual bool empty() const { return data.empty(); }
private:
  //Note: This vector *must* be used by your priority_queue implementation.
  std::vector<TYPE> data;
private:
  //***Add any additional member functions or data you require here.
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
SortedPQ<TYPE, COMP_FUNCTOR>::SortedPQ(
  InputIterator start,
  InputIterator end,
  COMP_FUNCTOR comp
) {

	data = std::vector<TYPE>(start, end);
	std::sort(data.begin(), data.end(), comp);
	this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
SortedPQ<TYPE, COMP_FUNCTOR>::SortedPQ(COMP_FUNCTOR comp) {

	this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
void SortedPQ<TYPE, COMP_FUNCTOR>::push(const TYPE& val) {

	// maybe use lower_bound here instead. Only difference would be
	// first in of equivalent TYPE would be first out
	typename std::vector<TYPE>::iterator it = std::upper_bound(data.begin(), data.end(), val, this->compare);
	data.insert(it, val);

	return;
}

template<typename TYPE, typename COMP_FUNCTOR>
void SortedPQ<TYPE, COMP_FUNCTOR>::pop() {

	data.pop_back();
	return;
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& SortedPQ<TYPE, COMP_FUNCTOR>::top() const {

	return data.back();
}

#endif //SORTED_PQ_H
