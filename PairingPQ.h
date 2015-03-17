#ifndef PAIRING_PQ_H
#define PAIRING_PQ_H

#include "Eecs281PQ.h"
#include <stack>
#include <iostream>
//A specialized version of the 'priority_queue' ADT implemented as a pairing priority_queue.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE> >
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
  virtual size_type size() const { return q_size; }

  //Description: Return true if the priority_queue is empty.
  //Runtime: O(1)
  virtual bool empty() const { return !root; }
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
		//Node class from within the pairing_priority_queue class. (ie: myNode.elt is a
		//legal statement in PairingPQ's add_node() function).
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

	size_type q_size;

	Node* meld(Node* left, Node* right);

	Node* build_tree(Node* other);
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
PairingPQ<TYPE, COMP_FUNCTOR>::PairingPQ(
  InputIterator start,
  InputIterator end,
  COMP_FUNCTOR comp
) {

	q_size = 0;
	root = nullptr;

	while (start != end) {
		push(*start);
		++start;
	}

	this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
PairingPQ<TYPE, COMP_FUNCTOR>::PairingPQ(COMP_FUNCTOR comp) {

	q_size = 0;
	root = nullptr;
	this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
PairingPQ<TYPE, COMP_FUNCTOR>::PairingPQ(const PairingPQ& other) {

	q_size = 0;
	root = nullptr;
	this->compare = other.compare;

	if (other.root) {
		q_size = other.q_size;
		root = build_tree(other.root);
	}
}

template<typename TYPE, typename COMP_FUNCTOR>
PairingPQ<TYPE, COMP_FUNCTOR>& PairingPQ<TYPE, COMP_FUNCTOR>::operator= (const PairingPQ & rhs) {

	if (this != &rhs) {
		// delete current pq so as not to leak memory
		while (q_size != 0) {
			pop();
		}

		// create new tree
		if (rhs.root) {
			q_size = rhs.q_size;

			root = build_tree(rhs.root);
		}
	}

	return *this;
}

template<typename TYPE, typename COMP_FUNCTOR>
PairingPQ<TYPE, COMP_FUNCTOR>::~PairingPQ() {

	while (q_size != 0) {
		pop();
	}
}

template<typename TYPE, typename COMP_FUNCTOR>
void PairingPQ<TYPE, COMP_FUNCTOR>::pop() {

	if (root) {
		Node* first = root->child;
		Node* current = first;
		Node* next = nullptr;

		if (current) {
			next = current->sibling;
		}

		std::vector <Node*> kids;

		delete root; root = nullptr;

		// first pass
		while (current) {

			if (current->sibling) {
				next = current->sibling->sibling;
				kids.push_back(meld(current, current->sibling));
				current = next;
			}
			else {
				kids.push_back(current);
				current = current->sibling;
			}

		}

		//second pass
		if (kids.size() >= 2) {
			current = meld(kids.back(), kids.at(kids.size() - 2));
			kids.pop_back();
			kids.pop_back();

			while (kids.size() != 0) {
				current = meld(kids.back(), current);
				kids.pop_back();
			}

			root = current;
		}
		else if (kids.size() == 1) {
			root = kids.back();
		}
		else { root = nullptr; }

		--q_size;
	}

	return;
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& PairingPQ<TYPE, COMP_FUNCTOR>::top() const {

	return root->elt;
}

template<typename TYPE, typename COMP_FUNCTOR>
void PairingPQ<TYPE, COMP_FUNCTOR>::push(const TYPE& val) {

	Node* temp = new Node(val);

	if (root) {
		root = meld(temp, root);
	}
	else {
		root = temp;
	}

	++q_size;
}

template<typename TYPE, typename COMP_FUNCTOR>
typename PairingPQ<TYPE, COMP_FUNCTOR>::Node* PairingPQ<TYPE, COMP_FUNCTOR>::meld(Node* left, Node* right) {

	if (this->compare(left->elt, right->elt)) {
		left->sibling = right->child;
		right->child = left;

		left = nullptr;

		return right;
	}
	else {
		right->sibling = left->child;
		left->child = right;
		
		right = nullptr;

		return left;
	}
}

template<typename TYPE, typename COMP_FUNCTOR>
typename PairingPQ<TYPE, COMP_FUNCTOR>::Node* PairingPQ<TYPE, COMP_FUNCTOR>::build_tree(Node* other) {

	Node* mytree = new Node(other->elt);

	std::stack <Node*> s;
	Node* current;

	if (other->child) {
		s.push(other->child);
	}

	while (!s.empty()) {
		current = new Node(s.top()->elt);
		s.pop();
		
		if (current->sibling) {
			s.push(current->sibling);
		}
		
		if (current->child) {
			s.push(current->child);
		}
		
		mytree = meld(mytree, current);
	}

	return mytree;
}

#endif //PAIRING_PQ_H

