#pragma once


/*

a1.h

Joshua Kraitberg
BTP500
Assignment 1
October 26, 2016

*/


#include <utility> // Needed for std::move
#include "stack.h"


template <typename T>
class DList {
	struct Node {
		// Node members ***
		T _data;
		Node* _next;
		Node* _prev;

		// Create a node with the data
		Node(const T& data = T{}) : _data(data), _next(nullptr), _prev(nullptr) {}

		// This connects to nodes to each other
		// This will assign the right node as the next node for the left node and vice versa
		static void bind(Node* lhs, Node* rhs) {
			lhs->_next = rhs;
			rhs->_prev = lhs;
		}

		// This is an extension for the two node bind
		// Allows three nodes to be binded
		static void bind(Node* lhs, Node* mid, Node* rhs) {
			bind(lhs, mid);
			bind(mid, rhs);
		}
	};

	// DList members ***
	Node* _begin;
	Node* _end;
	unsigned _size;
public:
	class const_iterator {
		friend class DList;
	protected:
		Node* _node;

		// Increment to the next node in the list
		// If current node is end() will do nothing
		// Very slow
		inline void inc() {
			if (_node->_next) {
				_node = _node->_next;
			}
		}

		// Decrement to the next node in the list
		// If current node is begin() will do nothing
		// Very very slow
		inline void dec() {
			// Check the previous two nodes to avoid returning sentinel node
			if (_node->_prev && _node->_prev->_prev) {
				_node = _node->_prev;
			}
		}
	public:
		// Create a const_iterator to a given node, default nullptr
		const_iterator(Node* node = nullptr) : _node(node) {}

		// Check for equality
		bool operator==(const_iterator rhs) {
			return _node == rhs._node;
		}

		// Check for inequality
		bool operator!=(const_iterator rhs) {
			return _node != rhs._node;
		}

		// Increment the iterator and return new iterator
		const_iterator operator++() {
			inc();
			return *this;
		}

		// Increment the iterator and return the old iterator
		const_iterator operator++(int) {
			const_iterator old = *this;
			inc();
			return old;
		}

		// Decrement the iterator and return new iterator
		const_iterator operator--() {
			dec();
			return *this;
		}

		// Decrement the iterator and return old iterator
		const_iterator operator--(int) {
			const_iterator old = *this;
			dec();
			return old;
		}

		// Returns a const reference to the data of the node
		const T& operator*() const {
			return _node->_data;
		}
	};

	class iterator : public const_iterator {
		friend class DList;
	public:
		// Create a iterator to a given node, default nullptr
		iterator(Node* node = nullptr) : const_iterator(node) {}

		// Increment the iterator and return new iterator
		iterator operator++() {
			const_iterator::inc();
			return *this;
		}

		// Increment the iterator and return old iterator
		iterator operator++(int) {
			iterator old = *this;
			const_iterator::inc();
			return old;
		}

		// Decrement the iterator and return new iterator
		iterator operator--() {
			const_iterator::dec();
			return *this;
		}

		// Decrement the iterator and return old iterator
		iterator operator--(int) {
			iterator old = *this;
			const_iterator::dec();
			return old;
		}

		// Returns a reference to the data of the node
		T& operator*() {
			return const_iterator::_node->_data;
		}
	};

	// Constructs an empty double linked list
	DList() : _begin(nullptr), _end(nullptr), _size(0) {
		_begin = new Node();
		_end = new Node();
		Node::bind(_begin, _end);
	}

	// Returns a iterator to beginning of the list
	iterator begin() {
		return iterator(_begin->_next);
	}

	// Returns a iterator to the end of the list
	iterator end() {
		return iterator(_end);
	}

	// Returns a const_iterator to beginning of the list
	const_iterator begin() const {
		return const_iterator(_begin->_next);
	}

	// Returns a const_iterator to the end of the list
	const_iterator end() const {
		return const_iterator(_end);
	}

	// Push the data to the front of the list
	void push_front(const T& data) {
		Node* node = new Node(data);
		Node::bind(_begin, node, _begin->_next);
		++_size;
	}

	// Push the data to the back of the list
	void push_back(const T& data) {
		Node* node = new Node(data);
		Node::bind(_end->_prev, node, _end);
		++_size;
	}

	// Pop the data at the front of the list
	void pop_front() {
		if (_size) {
			Node* node = _begin->_next;
			Node::bind(_begin, node->_next);
			delete node;
			--_size;
		}
	}

	// Pop the data at the back of the list
	void pop_back() {
		if (_size) {
			Node* node = _end->_prev;
			Node::bind(node->_prev, _end);
			delete node;
			--_size;
		}
	}
	
	// Insert data infront of loc and return iterator to the inserted data
	iterator insert(iterator loc, const T& data) {
		Node* node = new Node(data);
		Node::bind(loc._node->_prev, node, loc._node);
		return iterator(node);
	}

	// Erase data at the iterator
	void erase(iterator it) {
		Node::bind(it._node->_prev, it._node->_next);
		delete it._node;
	}

	// Erases the data between the iterators first (inclusive) and last (exclusive)
	void erase(iterator first, iterator last) {
		if (first._dl == this && last._dl == this && first != last) {
			Node* left = first._node->_prev;

			iterator i = first;
			Node* temp;

			while (i != last && i != end()) {
				Node* temp = i._node;
				++i;
				delete temp;
			}

			Node::bind(left, i._node);
		}
	}

	// Searches for the first iterator with matching data and returns it
	iterator search(const T& data) {
		iterator i;

		for (i = begin(); (*i != data) && (i != end()); ++i) {}

		return i;
	}

	// Searches for the first cont_iterator with matching data and returns it
	const_iterator search(const T& data) const {
		const_iterator i;

		for (i = begin(); (*i != data) && (i != end()); ++i) {}

		return i;
	}

private:
	// Originally I had it as iterator but that was too slow

	// Partitions the data in the list for quicksort
	// Using the right iterator as a pivot and starting at the left iterator
	// Will move every iterator with a value greater than the pivot to behind the pivot
	// Returns the iterator to the pivot
	Node* partition(Node*& left, Node*& right) {

		// Since the iterators are not invalidated the left and right iterators may be lost
		// So I take the absolute position in the list by taking the iterators surrounding them
		// Since iterator::operator--() can't return the sentinel node I create them manually
		Node* begin = left->_prev;
		Node* end = right->_next;
		Node* i = left;
		Node* j = right; // Tracks last node
		Node* pivot = right;
		Node* current;

		while (i != pivot) {
			// Find the next number greater than the pivot
			while (i != pivot && i->_data <= pivot->_data) { i = i->_next; }
			if (i != pivot) {
				current = i;
				i = i->_next;

				// Extract the node from the list
				Node::bind(current->_prev, current->_next);

				// Insert the current node after the pivot
				Node::bind(current, j->_next);
				Node::bind(j, current);

				// Move j forward
				j = current;
			}
		}

		// Get the original absolute positions of the list back
		left = begin->_next;
		right = end->_prev;
		return i;
	}
public:

	// Sorts the list between the left and right iterators, uses recursive quicksort
	void qSort(Node* left, Node* right) {
		if (left != right && left != right->_next) {
			Node* pivot = partition(left, right);
			
			if (pivot != left && pivot->_prev != left) {
				qSort(left, pivot->_prev);
			}
			if (pivot != right) {
				qSort(pivot->_next, right);
			}
		}
	}  

	// Sorts the entire list, qSort wrapper
	void qSort() {
		qSort(_begin->_next, _end->_prev);
	}

	// Sorts the list between begin and end, uses iterative quicksort
	void sortIterative(Node* begin, Node* end) {
		Stack<Node*> stack;
		stack.push(begin);
		stack.push(end);

		while (!stack.isEmpty()) {
			Node* right = stack.top();
			stack.pop();
			Node* left = stack.top();
			stack.pop();

			if (left != right && left != right->_next) {
				Node* pivot = partition(left, right);

				if (pivot != left && pivot->_prev != left) {
					stack.push(left);
					stack.push(pivot->_prev);
				}
				if (pivot != right) {
					stack.push(pivot->_next);
					stack.push(right);
				}
			}
		}
	}

	// Sort the entire list, sortIterative wrapper
	void sortIterative() {
		sortIterative(_begin->_next, _end->_prev);
	}

	// Returns if the list is empty
	bool empty() const {
		return !_size;
	}

	// Returns the size of the list
	int size() const {
		return _size;
	}

private:
	// Deletes all nodes
	void clear() {
		Node* next = _begin;
		while (next) {
			Node* t = next->_next;
			delete next;
			next = t;
		}
	}
public:

	// Delete the list
	~DList() {
		clear();
	}

	// Create a copy of a list
	DList(const DList& rhs) {
		operator=(rhs);
	}

	// Copy a list
	DList& operator=(const DList& rhs) {
		clear();

		_size = rhs._size;
		_begin = new Node();
		_end = new Node();
		Node* current = _begin;

		for (auto i = rhs.begin(); i != rhs.end(); ++i) {
			Node* temp = new Node(*i);
			Node::bind(current, temp);
			current = temp;
		}

		Node::bind(current, _end);
		return *this;
	}

	// Move construct a list
	DList(DList&& rhs) {
		operator=(std::move(rhs));
	}

	// Move a list
	DList& operator=(DList&& rhs) {
		clear();

		_size = rhs._size;
		rhs._size = 0;

		_begin = rhs._begin;
		rhs._begin = nullptr;

		_end = rhs._end;
		rhs._end = nullptr;

		return *this;
	}
};