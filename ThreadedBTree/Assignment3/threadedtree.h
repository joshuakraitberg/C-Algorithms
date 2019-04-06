#pragma once


const int BALANCE_FACTORS[2] = { -1, 1 };

template <class T>
class ThreadedTree {
	enum Branch { Left, Right };


	// *** Rules
	// A child should never be nullptr

	struct Node {
		bool _isThread[2];	// This is 2 bytes?
		Node* _child[2];
		T _data;
		//

		Node() : _isThread{ true, true }, _child{ nullptr, nullptr } {}

		Node(const T& data) : _data(data), _isThread{ true, true }, _child{ nullptr, nullptr } {}

		// Delete nodes recursively
		static void forest_fire(Node* node) {
			if (node) {
				if (!node->_isThread[Branch::Left]) {
					forest_fire(node->_child[Branch::Left]);
				}
				if (!node->_isThread[Branch::Right]) {
					forest_fire(node->_child[Branch::Right]);
				}
				delete node;
			}
		}
	};

	Node* _root;
	Node* _sentinels[2];
public:
	class const_iterator {
	protected:
		Node* _curr;
		//

		const_iterator(Node* c) : _curr(c) {}
	public:
		const_iterator() {
			_curr = nullptr;
		}

		// Post-fix increment
		const_iterator operator++(int) {
			const_iterator old = *this;
			operator++();

			return old;
		}

		// Post-fix deccrement
		const_iterator operator--(int) {
			const_iterator old = *this;
			operator--();

			return old;
		}

		// Pre-fix increment
		const_iterator operator++() {
			bool findNext = !_curr->_isThread[Branch::Right];
			_curr = _curr->_child[Branch::Right];

			if (findNext) {
				while (!_curr->_isThread[Branch::Left]) {
					_curr = _curr->_child[Branch::Left];
				}
			}

			return *this;
		}

		// Pre-fix deccrement
		const_iterator operator--() {
			bool findPrevious = !_curr->_isThread[Branch::Left];
			_curr = _curr->_child[Branch::Left];

			if (findPrevious) {
				while (!_curr->_isThread[Branch::Right]) {
					_curr = _curr->_child[Branch::Right];
				}
			}

			return *this;
		}

		// Return refernce to data
		const T& operator*() {
			return _curr->_data;
		}

		// Return true is const_terators are the same, false otherwise
		bool operator==(const const_iterator& rhs) const {
			return _curr == rhs._curr;
		}

		// Return false is const_terators are the same, true otherwise
		bool operator!=(const const_iterator& rhs) const {
			return _curr != rhs._curr;
		}

		friend class ThreadedTree;
	};

	class iterator : public const_iterator {
		iterator(Node* c) : const_iterator(c) {}

	public:
		iterator() : const_iterator() {}

		// Return reference to data
		const T& operator*() {
			return this->const_iterator::_curr->_data;
		}

		// Post-fix increment
		iterator operator++(int) {
			iterator old = *this;
			operator++();

			return old;
		}

		// post-fix deccrement
		iterator operator--(int) {
			iterator old = *this;
			operator--();

			return old;
		}

		// Pre-fix increment
		iterator operator++() {
			bool findNext = !const_iterator::_curr->_isThread[Branch::Right];
			const_iterator::_curr = const_iterator::_curr->_child[Branch::Right];

			if (findNext) {
				while (!const_iterator::_curr->_isThread[Branch::Left]) {
					const_iterator::_curr = const_iterator::_curr->_child[Branch::Left];
				}
			}

			return *this;
		}

		// Pre-fix deccrement
		iterator operator--() {
			bool findPrevious = !const_iterator::_curr->_isThread[Branch::Left];
			const_iterator::_curr = const_iterator::_curr->_child[Branch::Left];

			if (findPrevious) {
				while (!const_iterator::_curr->_isThread[Branch::Right]) {
					const_iterator::_curr = const_iterator::_curr->_child[Branch::Right];
				}
			}

			return *this;
		}

		friend class ThreadedTree;
	};

	// Default constructor
	ThreadedTree() : _root(nullptr), _sentinels{ new Node(), new Node() } {

		// Thread the sentinels to eachother
		_sentinels[Branch::Left]->_child[Branch::Left] = _sentinels[Branch::Left];
		_sentinels[Branch::Left]->_child[Branch::Right] = _sentinels[Branch::Right];

		_sentinels[Branch::Right]->_child[Branch::Left] = _sentinels[Branch::Left];
		_sentinels[Branch::Right]->_child[Branch::Right] = _sentinels[Branch::Right];
	}

private:

	// Recursively traverse tree to insertion point
	void insert(Node* node, Node* current) {
		Branch branch = (node->_data > current->_data) ? Branch::Right : Branch::Left;

		// End of brach
		if (current->_isThread[branch]) {
			// Rethread sentinel if needed
			if (current->_child[branch] == _sentinels[branch]) {
				_sentinels[branch]->_child[!branch] = node;
			}

			node->_child[branch] = current->_child[branch];
			node->_child[!branch] = current;

			current->_child[branch] = node;
			current->_isThread[branch] = false;
			return;
		}

		insert(node, current->_child[branch]);
	}
public:

	// insert value into tree
	// Returns iterator to value
	iterator insert(const T& data) {
		Node* node = new Node(data);

		// node children should not be nullptr by the end of this function
		if (!_root) {
			_root = node;

			_root->_child[Branch::Left] = _sentinels[Branch::Left];
			_sentinels[Branch::Left]->_child[Branch::Right] = _root;

			_root->_child[Branch::Right] = _sentinels[Branch::Right];
			_sentinels[Branch::Right]->_child[Branch::Left] = _root;
		}
		else {
			insert(node, _root);
		}

		return iterator(node);
	}

private:

	// Recursively traverse tree until value found
	// Return its iterator
	iterator find(const T& data, Node* current) const {
		if (!current || data == current->_data) { return iterator(current); }

		Branch branch = (data > current->_data) ? Branch::Right : Branch::Left;
		current = (!current->_isThread[branch]) ? current->_child[branch] : nullptr;

		return find(data, current);
	}
public:

	// Find value in tree and return it's iterator
	iterator find(const T& data) const {
		return find(data, _root);
	}

	// Return iterator to lowest value
	iterator begin() {
		return ++iterator(_sentinels[Branch::Left]);
	}

	// Returns iterator to one after highest value
	iterator end() {
		return iterator(_sentinels[Branch::Right]);
	}

	// Return const_iterator to lowest value
	const_iterator begin() const {
		return ++const_iterator(_sentinels[Branch::Left]);
	}

	// Returns const_iterator to one after highest value
	const_iterator end() const {
		return const_iterator(_sentinels[Branch::Right]);
	}

	// Destructor
	~ThreadedTree() {
		// forest_fire won't delete these
		if (_sentinels[Branch::Left]) {
			delete _sentinels[Branch::Left];
		}
		if (_sentinels[Branch::Right]) {
			delete _sentinels[Branch::Right];
		}

		Node::forest_fire(_root);
	}
};