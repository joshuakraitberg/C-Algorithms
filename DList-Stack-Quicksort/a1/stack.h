#pragma once


/*

stack.h

Joshua Kraitberg
BTP500
Assignment 1
October 26, 2016

*/


#include "a1.h"


template<typename T>
class DList;

template<typename T>
class Stack {
	DList<T> _dlist;
public:

	// Push the data to the stack
	void push(const T& data) {
		_dlist.push_back(data);
	}

	// Pop the top data from the stack
	void pop() {
		_dlist.pop_back();
	}

	// Get the data at the top of the stack
	T top() const {
		return *--_dlist.end();
	}

	// Check if the stack is empty
	bool isEmpty() const {
		return _dlist.empty();
	}
};