#pragma once


#include <iostream>


template <typename T>
class DList;


template<class T>
class BST {
	struct Node {
		T _value;
		Node* _parent;
		Node* _left;
		Node* _right;

		Node(T&& value) : _value(std::forward<T>(value)), _parent(nullptr), _left(nullptr), _right(nullptr) {}

		void severParent() {
			if (_parent) {
				if (this == _parent->_left) {
					_parent->_left = nullptr;
					_parent = nullptr;
				}
				else if (this == _parent->_right) {
					_parent->_right = nullptr;
					_parent = nullptr;
				}
			}
		}

		void assignLeft(Node* node) {
			if (node != this) {
				_left = node;
				if (node) {
					node->_parent = this;
				}
			}
			else {
				_left = nullptr;
			}
		}

		void assignRight(Node* node) {
			if (node != this) {
				_right = node;
				if (node) {
					node->_parent = this;
				}
			}
			else {
				_right = nullptr;
			}
		}

		void swapAssign(Node* replacement) {
			replacement->assignRight(_right);
			_right = nullptr;

			replacement->assignRight(_left);
			_left = nullptr;

			if (_parent) {
				if (this == _parent->_left) {
					_parent->_left = replacement;
					replacement->_parent = _parent;
				}
				else if (this == _parent->_right) {
					_parent->_right = replacement;
					replacement->_parent = _parent;
				}
				_parent = nullptr;
			}
		}

		static void forest_fire(Node* node) {
			if (node) {
				forest_fire(node->_left);
				forest_fire(node->_right);
				delete node;
			}
		}
	};

	Node* _head;
public:
	BST() : _head(nullptr) {}

	~BST() {
		Node::forest_fire(_head);
	}

	void insert(T&& v) {
		Node* node = new Node(std::forward<T>(v));

		if (_head) {
			Node* current = _head;
			while (current) {
				if (v > current->_value) {
					if (current->_right) {
						current = current->_right;
					}
					else {
						current->assignRight(node);
						current = nullptr;
					}
				}
				else {
					if (current->_left) {
						current = current->_left;
					}
					else {
						current->assignLeft(node);
						current = nullptr;
					}
				}
			}
		}
		else {
			_head = node;
		}
	}

	void remove(const T& v) {
		Node* current = _head;
		Node* removal = nullptr;

		while (current) {
			if (v == current->_value) {
				removal = current;
				current = nullptr;
			}
			else if (v > current->_value) {
				current = current->_right;
			}
			else {
				current = current->_left;
			}
		}

		if (removal && removal->_right) {
			Node* successor = nullptr;
			current = removal->_right;

			if (current) {
				do {
					successor = current;
					current = current->_left;
				} while (current);

				// remove succesor + handle possible right leaf
				if (successor != removal->_right) {
					successor->_parent->assignLeft(successor->_right);
				}
				removal->swapAssign(successor);		
			}
			else {
				if (removal->_left) {
					successor = removal->_left;
					if (removal->_parent) {
						successor->_parent = removal->_parent;
						removal->_parent->_left = successor;
					}
				}
			}

			if (removal == _head) {
				_head = successor;
			}
			delete removal;
		}
	}

	void print() {
		DList<Node*> current;

		std::cout << "print BST\n";


		if (_head) {
			current.push_front(_head);

			int height = 0;
			while (!current.empty()) {
				std::cout << "HEIGHT " << height++ << " -> ";

				DList<Node*> fill;
				while (!current.empty()) {
					Node* temp = current.front();
					std::cout << temp->_value << ' ';
					
					if (temp->_left) {
						fill.push_back(temp->_left);
					}
					if (temp->_right) {
						fill.push_back(temp->_right);
					}
					current.pop_front();
				}
				std::cout << '\n';
				current = std::move(fill);
			}

		}
		else {
			std::cout << "Tree empty";
		}

		std::cout << "\n\n";
	}
};



/////////////////////////////////////////////////////////////

// ripped from a1


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

	// Constructs an empty double linked list
	DList() : _begin(nullptr), _end(nullptr), _size(0) {
		_begin = new Node();
		_end = new Node();
		Node::bind(_begin, _end);
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

	T& front() const {
		return _begin->_next->_data;
	}

	T& end() const {
		return _end->_prev->_data;
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

	// Returns if the list is empty
	bool empty() const {
		return !_size;
	}

	// Returns the size of the list
	int size() const {
		return _size;
	}

	// Deletes all nodes
	void clear() {
		Node* next = _begin;
		while (next) {
			Node* t = next->_next;
			delete next;
			next = t;
		}
	}

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