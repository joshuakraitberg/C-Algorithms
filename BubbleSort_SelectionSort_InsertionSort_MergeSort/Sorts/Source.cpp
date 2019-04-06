#include <iostream>

template<class T>
void printarray(T array[], size_t n) {
	std::cout << "array = ";
	for (auto i = 0u; i < n; i++) {
		std::cout << array[i] << ' ';
	}
	std::cout << '\n';
}

template<class T>
void BubbleSort(T array[], size_t n) {

	for (auto i = 0u; i < n - 1; i++) {
		for (auto j = 0u; j < n - i - 1; j++) {
			if (array[j] < array[j + 1]) {
				T tmp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = tmp;
			}
		}
	}
}

template<class T>
void InsertionSort(T array[], size_t n) {
	T curr;
	
	for (auto i = 0u; i < n; i++) {
		size_t j;
		curr = array[i];

		for (j = i; j > 0 && array[j - 1] < curr; j--) {
			array[j] = array[j - 1];
		}
		array[j] = curr;
	}
}

template<class T>
void SelectionSort(T array[], size_t n) {
	T min;

	for (auto i = 0u; i < n; i++) {
		min = i;

		for (auto j = i; j < n; j++) {
			if (array[j] < array[min]) {
				min = j;
			}
		}

		T t = array[i];
		array[i] = array[min];
		array[min] = t;
	}
}

template<class T>
void Merge(T array[], T sorted[], size_t begin, size_t mid, size_t end) {
	size_t i = begin;
	size_t j = mid;
	size_t curr = begin;

	while (i < mid && j <= end) {
		sorted[curr++] = (array[i] < array[j]) ? array[i++] : array[j++];
	}

	while (i < mid) {
		sorted[curr++] = array[i++];
	}

	while (j <= end) {
		sorted[curr++] = array[j++];
	}

	for (auto c = begin; c <= end; c++) {
		array[c] = sorted[c];
	}
}

template<class T>
void MergeSort(T array[], T sorted[], size_t begin, size_t end) {
	if (begin < end) {
		size_t mid = (end + begin) / 2;
		MergeSort(array, sorted, begin, mid);
		MergeSort(array, sorted, mid + 1, end);
		Merge(array, sorted, begin, mid + 1, end);
	}
}

template<class T>
void MergeSort(T array[], size_t n) {
	if (n > 2) {
		size_t mid = n / 2;
		T* sorted = new T[n];

		MergeSort(array, sorted, 0, n - 1);

		delete sorted;
	}
}

int main() {
	int arr[] = { 12, 123, 23, 5, 6754, 12, 312, 4 };


	printarray(arr, sizeof(arr) / sizeof(int));
	MergeSort(arr, sizeof(arr) / sizeof(int));
	printarray(arr, sizeof(arr) / sizeof(int));

	std::cin.get();
	return 0;
}