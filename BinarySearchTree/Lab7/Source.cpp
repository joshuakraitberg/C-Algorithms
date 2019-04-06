#include <iostream>
#include "bst.h"


int main() {
	BST<int> b;

	b.insert(4);
	b.insert(2);
	b.insert(8);
	b.insert(1);
	b.insert(3);
	b.insert(6);
	b.insert(12);
	b.insert(5);
	b.insert(7);
	b.insert(11);
	b.insert(9);
	b.insert(10);

	b.print();


	b.remove(6);
	b.print();
	b.remove(2);
	b.print();
	b.remove(4);
	b.print();
	b.remove(8);
	b.print();

	std::cin.get();
	return 0;
}
