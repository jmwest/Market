#ifndef BINARY_PQ_H
#define BINARY_PQ_H

#include "Eecs281PQ.h"

//A specialized version of the 'priority_queue' ADT implemented as a binary priority_queue.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
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
    { /*** Fill this in - might be very simple depending on implementation ***/ }

  //Description: Return true if the priority_queue is empty.
  //Runtime: O(1)
  virtual bool empty() const
    { /*** Fill this in - might be very simple depending on implementation ***/ }
private:
  //Note: This vector *must* be used your priority_queue implementation.
  std::vector<TYPE> data;
private:
  //***Add any additional member functions or data you require here.
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
BinaryPQ<TYPE, COMP_FUNCTOR>::BinaryPQ(
  InputIterator start,
  InputIterator end,
  COMP_FUNCTOR comp
) {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
BinaryPQ<TYPE, COMP_FUNCTOR>::BinaryPQ(COMP_FUNCTOR comp) {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
void BinaryPQ<TYPE, COMP_FUNCTOR>::push(const TYPE& val) {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
void BinaryPQ<TYPE, COMP_FUNCTOR>::pop() {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& BinaryPQ<TYPE, COMP_FUNCTOR>::top() const {
  //Your code.
  return TYPE(); //This line present only so that this provided file compiles.
}

#endif //BINARY_PQ_H

