#ifndef PAIRING_PQ_H
#define PAIRING_PQ_H

#include "Eecs281PQ.h"

//A specialized version of the 'priority_queue' ADT implemented as a pairing priority_queue.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
public:
  typedef unsigned size_type;

  //Description: Construct a priority_queue out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Construct an empty priority_queue with an optional comparison functor.
  //Runtime: O(1)
  PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR());
  
  //Description: Copy constructor.
  //Runtime: O(n)
  PairingPQ(const PairingPQ& other);
  
  //Description: Copy assignment operator.
  //Runtime: O(n)
  PairingPQ& operator=(const PairingPQ& rhs);
  
  //Description: Destructor
  //Runtime: O(n)
  ~PairingPQ();

  //Description: Add a new element to the priority_queue.
  //Runtime: Amortized O(1)
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the priority_queue.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the priority_queue is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: Amortized O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the priority_queue.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the priority_queue.
  //Runtime: O(1)
  virtual size_type size() const { /*** Fill this in ***/ }

  //Description: Return true if the priority_queue is empty.
  //Runtime: O(1)
  virtual bool empty() const { /*** Fill this in ***/ }
  class Node {
  public:
	Node(const TYPE & val)
	{
			elt = val;
			sibling = child = nullptr;
	}
  public:
    //Description: Allows access to the element at that Node's position.
    //Runtime: O(1) - this has been provided for you.
    const TYPE& operator*() const { return elt; }
    const Node* sibling_ptr() const { return sibling; }
    const Node* child_ptr() const { return child; }

    //The following line allows you to access any private data members of this
    //Node class from within the pairing_priority_queue class. (ie: myNode.elt is a legal
    //statement in PairingPQ's add_node() function).
    friend PairingPQ;
  private:
    TYPE elt;
    Node* sibling;
    Node* child;
  };

  const Node* root_ptr() const { return root; }

private:
  Node* root;
  //***Add any additional member functions or data you require here.
  //***We recommend creating a 'meld' function (see the spec).
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
PairingPQ<TYPE, COMP_FUNCTOR>::PairingPQ(
  InputIterator start,
  InputIterator end,
  COMP_FUNCTOR comp
) {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
PairingPQ<TYPE, COMP_FUNCTOR>::PairingPQ(COMP_FUNCTOR comp) {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
PairingPQ<TYPE, COMP_FUNCTOR>::PairingPQ(const PairingPQ& other) {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
PairingPQ<TYPE, COMP_FUNCTOR>&
PairingPQ<TYPE, COMP_FUNCTOR>::operator=(const PairingPQ& rhs) {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
PairingPQ<TYPE, COMP_FUNCTOR>::~PairingPQ() {
  //Your code.
}


template<typename TYPE, typename COMP_FUNCTOR>
void PairingPQ<TYPE, COMP_FUNCTOR>::pop() {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& PairingPQ<TYPE, COMP_FUNCTOR>::top() const {
  //Your code.
  return TYPE(); //This line present only so that this provided file compiles.
}

template<typename TYPE, typename COMP_FUNCTOR>
void PairingPQ<TYPE, COMP_FUNCTOR>::push(const TYPE& val) {
  //Your code.
}

#endif //PAIRING_PQ_H

