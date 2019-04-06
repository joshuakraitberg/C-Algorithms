#pragma once

struct Object {
	int value;
};

//DList, a doubly linked list that will do all the work.  You can
//assume that every function listed is correctly implemented. 
//DO NOT implement this linked list... assume its already done.s
class DList {
	struct Node {
		Node(const Object& data, Node* next = nullptr, Node* prev = nullptr);
	};
	class const_iterator {
		friend class DList;
		const_iterator(Node* curr, const DList* myList);
	public:
		const_iterator();
		const_iterator operator++();
		const_iterator operator++(int);
		const_iterator operator--();
		const_iterator operator--(int);
		Object operator*() const;
		bool operator==(const_iterator rhs) const;
		bool operator!=(const_iterator rhs) const;
	};
	class iterator :public const_iterator {
		friend class DList;
		iterator(Node* curr, DList* myList);
	public:
		iterator();
		iterator operator++();
		iterator operator++(int);
		iterator operator--();
		iterator operator--(int);
		Object& operator*();
		bool operator==(iterator rhs) const;
		bool operator!=(iterator rhs) const;
	};
public:
	DList() {}
	//adds data to front of list
	void push_front(const Object& data);
	//adds data to back of list
	void push_back(const Object& data);
	//removes node from front of list
	void pop_front();
	//removes node from back of list
	void pop_back();
	//inserts node in front of node referred to by loc
	void insert(iterator loc, const Object& data);
	//removes node referred to by loc
	void erase(iterator loc);
	//returns const_iterator to first node in LL
	const_iterator begin() const;
	//returns const_iterator to the node that follows the last node
	const_iterator end() const;
	//returns const_iterator to first node in LL
	iterator begin();
	//returns const_iterator to the node that follows the last node
	iterator end();
	//copy and move operators/constructors
	DList(const DList& list);
	const DList& operator=(const DList& other);
	DList(const DList&& list);
	const DList& operator=(const DList&& other);
	//destructor
	~DList() {}
};



//the abstract base class Stack, the interface of a Stack.
class Stack {
public:
	Stack() {}
	virtual void push(const Object& o) = 0;
	virtual void pop() = 0;
	virtual Object top() = 0;
	virtual bool isEmpty() = 0;
	virtual ~Stack() {}
};

class classAdapterStack : public Stack, protected DList {
public:
	void push(const Object& o) {
		push_front(o);
	}

	void pop() {
		pop_front();
	}

	Object top() {
		return *begin();
	}
	
	bool isEmpty() {
		return begin() == end();
	}

	virtual ~classAdapterStack() {}
};

class objectAdapterStack : public Stack {
	DList _list;
public:
	void push(const Object& o) {
		_list.push_front(o);
	}

	void pop() {
		_list.pop_front();
	}

	Object top() {
		return *(_list.begin());
	}

	bool isEmpty() {
		return _list.begin() == _list.end();
	}

	virtual ~objectAdapterStack() {}
};