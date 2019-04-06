/*********************************************************************/
/*                                                                   */
/* Tester for DSA555 assignment 1, question 1                    	 */
/* Version 1.0                                                       */
/*  You will also need the files timer.h and timer.cpp               */
/* To compile:                                                       */
/*                                                                   */
/*      g++ a1q1main.cpp timer.cpp -std=c++0x                        */
/*                                                                   */
/*********************************************************************/

#include "a1.h"
#include "timer.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;
//change the #define VERBOSE to 1 for a bit more error information
#define VERBOSE 1

template <typename T>
bool checkList(const DList<T>& list, const T arr2[], int sz);
template <typename T>
void printLists(const DList<T>& list, const T array[], int sz);

void insertionSort(int arr[], int left, int right);
void quickSort(int arr[], int size);
void quickSort(int arr[], int left, int right);

int main(int argc, char* argv[]) {
	int cap = 5000000;
	if (argc != 1) {
		cap = atoi(argv[1]);
	}
	int* data = new int[cap];
	DList<int> recursiveList;
	DList<int> iterativeList;
	for (int i = 0; i<cap; i++) {
		data[i] = rand();
		recursiveList.push_back(data[i]);
		iterativeList.push_back(data[i]);
	}
	bool passtest = true;
	int numPassed = 0;



	cout << "This tester assumes your code has passed a1q1main." << endl;
	cout << "If it has not the bug could be there and not your" << endl;
	cout << "sort() functions" << endl;
	cout << "This tester will initialize two lists with random numbers" << endl;
	cout << "By default it will use 200,000 random numbers to test your lists" << endl;
	cout << "It will then sort them (using different sorts)" << endl;
	cout << "and compare them against a sorted array initialized with the" << endl;
	cout << "same values" << endl;
	cout << "the sorts are timed.  While I expect the linked list to be slightly" << endl;
	cout << "slower than an array version of the sort, it shouldn't be dramatically" << endl;
	cout << "slower.  Furthermore if you time it with data sizes that follow a linear" << endl;
	cout << "increase in size, the timings should exhibit a log linear pattern" << endl;

	Timer t1;
	t1.start();
	quickSort(data, cap);
	t1.stop();
	cout << "quicksort array ( " << cap << " ) values: " << t1.currtime() << "s" << endl;

	Timer t2;
	t2.start();
	recursiveList.qSort();
	t2.stop();
	cout << "recursive LL sort( " << cap << " ) values: " << t2.currtime() << "s" << endl;

	Timer t3;
	t3.start();
	iterativeList.sortIterative();
	t3.stop();
	cout << "iterative LL sort( " << cap << " ) values: " << t3.currtime() << "s" << endl;
	cout << endl;
	cout << endl;
	cout << "verifying sort function correctness" << endl;
	if (!checkList(recursiveList, data, cap)) {
		passtest = false;
		cout << "bug in recursive sort() function" << endl;
#if VERBOSE == 1
		printLists(recursiveList, data, cap);
#endif
	}
	if (passtest) {
		numPassed++;
		if (!checkList(iterativeList, data, cap)) {
			passtest = false;
			cout << "bug in iterative sort() function" << endl;
#if VERBOSE == 1
			printLists(iterativeList, data, cap);
#endif
		}

	}
	if (passtest) {
		numPassed++;
	}
	if (numPassed == 2) {
		cout << "Testing for Assignment 1, Question 3 completed successfully" << endl;
	}
	else {
		cout << numPassed << " / 2 tests passed.  Looks like you still" << endl;
		cout << "have some work to do" << endl;
	}
	return 0;
}

template <typename T>
bool checkList(const DList<T>& list, const T array[], int sz) {
	bool rc = true;
	auto it = list.begin();
	int i;
	for (i = 0; i<sz && it != list.end() && rc; i++, it++) {
		if (*it != array[i]) {
			rc = false;
		}
	}
	if (it != list.end() || i < sz) {
		rc = false;
	}
	return rc;
}

template <typename T>
void printLists(const DList<T>& list, const T array[], int sz) {
	auto it = list.begin();
	for (int i = 0; i<sz; i++, it++) {
		cout << "correct: " << array[i] << " your list: " << *it << endl;
	}
}

void insertionSort(int arr[], int left, int right) {
	int curr;
	int i, j;
	for (i = left + 1; i <= right; i++) {
		curr = arr[i];
		for (j = i; j>0 && arr[j - 1] > curr; j--) {
			arr[j] = arr[j - 1];
		}
		arr[j] = curr;
	}
}


void quickSort(int arr[], int left, int right) {
	if (right - left <= 3) {
		insertionSort(arr, left, right);
	}
	else {
		int pivotpt = right;
		int pivot = arr[pivotpt];
		int i = left;
		int j = right - 1;
		while (i<j) {
			while (i< right - 1 && arr[i]<pivot) i++;
			while (j > 0 && arr[j]>pivot) j--;
			if (i<j)
				swap(arr[i++], arr[j--]);
		}
		if (i == j && arr[i] < arr[pivotpt])
			i++;
		swap(arr[i], arr[pivotpt]);
		quickSort(arr, left, i - 1);
		quickSort(arr, i + 1, right);
	}
}

void quickSort(int arr[], int size) {
	quickSort(arr, 0, size - 1);
}