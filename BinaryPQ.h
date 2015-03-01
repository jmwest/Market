#ifndef BINARY_PQ_H
#define BINARY_PQ_H

#include "Eecs281PQ.h"

//A specialized version of the 'priority_queue' ADT implemented as a binary priority_queue.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE> >
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
public:
  typedef unsigned size_type;

  //Description: Construct a priority_queue out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Construct an empty priority_queue with an optional comparison functor.
  //Runtime: O(1)
  BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Add a new element to the priority_queue.
  //Runtime: O(log(n))
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the priority_queue.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the priority_queue is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the priority_queue.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the priority_queue.
  //Runtime: O(1)
  virtual size_type size() const
	{ return int(data.size()) - 1; }

  //Description: Return true if the priority_queue is empty.
  //Runtime: O(1)
  virtual bool empty() const
	{ return !(int(data.size()) - 1); }

private:
  //Note: This vector *must* be used your priority_queue implementation.
  std::vector<TYPE> data;
private:
  //***Add any additional member functions or data you require here.
	void fixUp(int pos);
	void fixDown(int pos);
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
BinaryPQ<TYPE, COMP_FUNCTOR>::BinaryPQ(
  InputIterator start,
  InputIterator end,
  COMP_FUNCTOR comp
) {
	data.push_back(TYPE());
	data.insert(data.end(), start, end);

	for (int i = int(data.size() / 2); i > 0; --i) {
		fixDown(i);
	}

	this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
BinaryPQ<TYPE, COMP_FUNCTOR>::BinaryPQ(COMP_FUNCTOR comp) {
	data.push_back(TYPE());

	this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
void BinaryPQ<TYPE, COMP_FUNCTOR>::push(const TYPE& val) {
	data.push_back(val);

	fixUp(int(data.size()) - 1);
}

template<typename TYPE, typename COMP_FUNCTOR>
void BinaryPQ<TYPE, COMP_FUNCTOR>::pop() {
	data.at(1) = data.back();
	data.pop_back();

	if (int(data.size()) > 2) {
		fixDown(1);
	}
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& BinaryPQ<TYPE, COMP_FUNCTOR>::top() const {

	return data.at(1);
}

template<typename TYPE, typename COMP_FUNCTOR>
void BinaryPQ<TYPE,COMP_FUNCTOR>::fixUp(int pos) {
	for (int i = pos; i > 1; i /= 2) {
		if (this->compare(data.at(i/2), data.at(i))) {
			TYPE temp = data.at(i);
			data.at(i) = data.at(i/2);
			data.at(i/2) = temp;
		}
		else { break; }
	}

	return;
}

template<typename TYPE, typename COMP_FUNCTOR>
void BinaryPQ<TYPE,COMP_FUNCTOR>::fixDown(int pos) {

	for (int i = pos; i <= (int(data.size()) - 1) / 2;) {

		if (int(data.size()) - 1 == i * 2) {
			if (this->compare(data.at(i),data.at(i * 2))) {
				TYPE temp = data.at(i);
				data.at(i) = data.at(i * 2);
				data.at(i * 2) = temp;

				i *= 2;
			}
			else { break; }
		}
		else {
			int bigger_child = this->compare(data.at((i * 2) + 1), data.at(i * 2)) ? i * 2 : (i * 2) + 1;

			if (this->compare(data.at(i), data.at(bigger_child))) {
				TYPE temp = data.at(i);
				data.at(i) = data.at(bigger_child);
				data.at(bigger_child) = temp;

				i = bigger_child;
			}
			else { break; }
		}
	}
}

#endif //BINARY_PQ_H

