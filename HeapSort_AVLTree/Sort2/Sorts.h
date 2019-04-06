#pragma once

template<class T>
void print(T* array, unsigned size) {
	std::cout << "ARRAY: ";
	for (auto i = 0; i < size; ++i) {
		std::cout << array[i] << ' ';
	}
	std::cout << '\n';
}

template<class T>
void HeapSort(T* array, int size) {
	int end = size - 1;
	
	// Convert array to max heap
	heapify(array, end);

	while (end) {

		// Swap
		T temp = std::move(array[end]);
		array[end] = std::move(array[0]);
		array[0] = std::move(temp);

		// Reconstitute heap
		siftDown(array, 0, --end);
	}
}

template<class T>
void heapify(T* array, int end) {
	int begin = (end - 1) >> 1;

	while (begin >= 0) {
		siftDown(array, begin--, end);
	}
}

template<class T>
void siftDown(T* array, int begin, int end) {
	int child = (begin << 1) + 1;

	if ((child <= end) && (array[child] > array[begin])) {
		if ((child + 1 <= end) && (array[child + 1] > array[child])) {
			++child;
		}

		// Swap
		T temp = std::move(array[child]);
		array[child] = std::move(array[begin]);
		array[begin] = std::move(temp);

		siftDown(array, child, end);
	}
}

const int BALANCE_FACTORS[2] = { -1, 1 };

template<class T>
class AVLTree {
	enum Direction { left, Right };

	struct Node	{
		Node* _child[2];
		int _balance;
		T _data;
	public:
		Node() : _child{ nullptr, nullptr }, _balance(0) {}

		Node(const T& data) : _data(data), _child{ nullptr, nullptr }, _balance(0) {}

		Node(T&& data) : _data(std::move(data)), _child { nullptr, nullptr }, _balance(0) {}
	};

	Node* _root;
	unsigned _size;
public:
	AVLTree() : _root(nullptr), _size(0) {}

	Node* singleRotation(Node* root, Direction dir) {
		Node* pivot = root->_child[!dir];

		root->_child[!dir] = pivot->_child[dir];
		root->_balance = (root->_child[Direction::Right]) ? abs(root->_child[Direction::Right]->_balance) + 1 : 0 -
			(root->_child[Direction::Left]) ? abs(root->_child[Direction::Left]->_balance) + 1 : 0;

		pivot->_child[dir] = root;

		if (root == _root) {
			_root = pivot;
		}

		pivot->_balance = 0;

		return pivot;
	}

	Node* insert(Node* current, Node* node, bool& balance) {
		if (!current) { return node; }

		Direction dir = (node->_data < current->_data) ? Direction::left : Direction::Right;
		current->_child[dir] = insert(current->_child[dir], node, balance);

		if (balance) {
			current->_balance += BALANCE_FACTORS[dir];

			if (current->_balance > 1 || current->_balance < -1) {
				current = singleRotation(current, dir);
			}
			else {
				balance = current->_balance;
			}
		}

		return current;
	}

	void insert(const T& data) {
		Node* node = new Node(data);

		if (!_root) { _root = node;	}
		else {
			bool balance = true;
			insert(_root, node, balance);
		}
	}
};