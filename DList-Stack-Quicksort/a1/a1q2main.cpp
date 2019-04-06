///*********************************************************************/
///*                                                                   */
///* Tester for DSA555 assignment 1, question 2                    	 */
///* Version 1.0                                                       */
///* To compile:                                                       */
///*                                                                   */
///*      g++ a1q2main.cpp -std=c++0x                                  */
///*                                                                   */
///*********************************************************************/
//
//#include "stack.h"
//#include <cstring>
//#include <cstdlib>
//#include <iostream>
//using namespace std;
//
//struct Record {
//	char word_[30];
//	int count_;
//};
//ostream& operator<<(ostream& os, const Record rec) {
//	os << rec.word_;
//	return os;
//}
//bool operator==(const Record& a, const Record& b) {
//	bool rc = false;
//	if (strcmp(a.word_, b.word_) == 0 && a.count_ == b.count_) {
//		rc = true;
//	}
//	return rc;
//}
//bool operator!=(const Record& a, const Record& b) {
//	return !(a == b);
//}
//bool operator <(const Record& a, const Record& b) {
//	bool rc = false;
//	if (strcmp(a.word_, b.word_) < 0) {
//		rc = true;
//	}
//	return rc;
//}
//
//template <typename T>
//void addBack(const T& data, T arr[], int sz);
//
//int main(void) {
//	Record data[20] = {
//		{ "the",1 },
//		{ "quick",2 },
//		{ "brown ",3 },
//		{ "fox",4 },
//		{ "jumped",5 },
//		{ "over",6 },
//		{ "lazy",7 },
//		{ "dog",8 },
//		{ "Calvin",9 },
//		{ "and",10 },
//		{ "Hobbes",11 },
//		{ "night",12 },
//		{ "watch",13 },
//		{ "captain",14 },
//		{ "carrot",15 },
//		{ "lilac",16 },
//		{ "lavender",17 },
//		{ "lily",18 },
//		{ "coffee",19 },
//		{ "tea",20 }
//	};
//
//	//these array will mirror what happens to LL
//	Record mirror[20];
//
//	Stack<Record> theStack;
//
//	bool passtest = true;
//	int numPassed = 0;
//	int mirrorCount = 0;
//
//
//
//	/* Test constructors, begin and end functions*/
//	cout << "test 1: create stack and check that it is empty" << endl;
//
//
//	if (passtest) {
//		if (!theStack.isEmpty()) {
//			passtest = false;
//			cout << "Error 1: isEmpty() not returning true on empty stack" << endl;
//
//		}
//	}
//	if (passtest) {
//		numPassed++;
//		cout << "test 2: perform some push() calls, and check the top()" << endl;
//		for (int i = 0; i<5; i++) {
//			theStack.push(data[i]);
//			addBack(data[i], mirror, i);
//			mirrorCount++;
//
//			if (theStack.isEmpty() || theStack.top() != mirror[i]) {
//				passtest = false;
//				cout << "Error 2: data at top does not match most recently pushed data" << endl;
//
//			}
//		}
//	}
//
//	if (passtest) {
//		numPassed++;
//		cout << "test 3: Test the pop() function" << endl;
//		for (int i = 0; passtest && i<4; i++) {
//			mirrorCount--;
//			theStack.pop();
//			if (theStack.isEmpty() || theStack.top() != mirror[mirrorCount - 1]) {
//				passtest = false;
//				cout << "Error 3: data at top does not match most recently pushed data" << endl;
//				cout << "what should be at top: " << endl;
//				cout << mirror[mirrorCount - 1];
//				cout << "what you have at the top: " << endl;
//				cout << theStack.top();
//			}
//
//		}
//	}
//	if (passtest) {
//		numPassed++;
//		cout << "test 4: remove last item and ensure list is empty" << endl;
//		mirrorCount--;
//		theStack.pop();
//		if (!theStack.isEmpty()) {
//			passtest = false;
//			cout << "Error 4: list should be empty but it is not" << endl;
//		}
//	}
//	if (passtest) {
//		numPassed++;
//	}
//	if (numPassed == 4) {
//		cout << "Testing for Assignment 1 Question 2 completed successfully" << endl;
//	}
//	else {
//		cout << numPassed << " / 4 tests passed.  Looks like you still" << endl;
//		cout << "have some work to do" << endl;
//	}
//	return 0;
//}
//
//template <typename T>
//void addBack(const T& data, T arr[], int sz) {
//	arr[sz] = data;
//}
