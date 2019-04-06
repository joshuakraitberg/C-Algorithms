#include <iostream>
#include <string>

using namespace std;


const int MAX_UNDOS = 5;

template<class T>
class Memento {
	T _state;
public:
	~Memento() {}

	Memento() {}

	void setState(T& state) {
		_state = state;
	}

	void setState(T&& state) {
		_state = std::move(state);
	}

	T& getState() {
		return _state;
	}
};

template<class T>
class CareTaker {
	struct Node
	{
		Memento<T>* memento;
		Node* next;
		Node* prev;

		Node() : memento(nullptr), next(nullptr), prev(nullptr) {}

		~Node() {
			if (memento) { delete memento; }
		}
	};

	int _nodes;
	int _maxNodes;
	int _current;
	Node* _node;
public:
	CareTaker(int max) : _nodes(0), _current(0), _maxNodes(max), _node(nullptr) {
		if (!_maxNodes) {
			throw std::exception();
		}

		Node* first = new Node();
		_node = first;
		for (auto i = 1u; i < _maxNodes; ++i) {
			auto temp = new Node();
			temp->next = _node;
			_node->prev = temp;
			_node = temp;
		}

		first->next = _node;
		_node->prev = first;
	}

	~CareTaker() {
		if (_node) {
			for (auto i = 0u; i < _maxNodes; ++i) {
				Node* temp = _node->next;
				delete _node;
				_node = temp;
			}
		}
	}

	Memento<T>* undo() {
		if (!_current) {
			return nullptr;
		}

		--_current;
		_node = _node->prev;
		return _node->next->memento;
	}

	Memento<T>* redo() {
		if (_current >= _nodes) {
			return nullptr;
		}

		++_current;
		_node = _node->next;
		return _node->prev->memento;
	}

	void add(Memento<T>* memento) {
		_node = _node->next;
		_node->memento = memento;

		if (_current == _nodes) {
			if (_nodes < _maxNodes) {
				++_nodes;
				++_current;
			}
		}
		else {
			++_current;
			_nodes = _current;
		}
	}
};


class Data {
	string str_;
	int number_;
public:
	Data() {
		str_ = "";
		number_ = 0;
	}
	void print() const {
		cout << number_ << " - " << str_ << endl;
	}
	void setString(string s) {
		str_ = s;
	}
	void setNumber(int n) {
		number_ = n;
	}

	Memento<Data>* createMemento() {
		Memento<Data>* memento = new Memento<Data>();
		memento->setState(*this);

		return memento;
	}

	void setMemento(Memento<Data>* memento) {
		*this = memento->getState();
	}
};


int main(void) {
	Data D;
	int choice;
	string inputString;
	int inputNumber;

	CareTaker<Data> careTaker(MAX_UNDOS);

	do {
		cout << "*********************************" << endl;
		cout << "what would you like to do: " << endl;
		cout << "1) Modify string" << endl;
		cout << "2) Modify number" << endl;
		cout << "3) undo" << endl;
		cout << "4) exit" << endl;
		cin >> choice;
		switch (choice) {
		case 1:
			cout << "enter a new string" << endl;
			cin >> inputString;
			careTaker.add(D.createMemento());
			D.setString(inputString);
			cin.clear();
			cin.ignore(1000, '\n');
			break;
		case 2:
			cout << "enter a new number" << endl;
			cin >> inputNumber;
			if (cin) {
				careTaker.add(D.createMemento());
				D.setNumber(inputNumber);
			}
			else {
				cin.clear();
				cin.ignore(1000, '\n');
			}
			break;
		case 3:
			//your code to do undo goes here
			Memento<Data>* memento = careTaker.undo();

			if (memento) {
				D = memento->getState();
			}
			else {
				std::cout << "There is nothing to undo." << std::endl;
			}

			break;

		}
		cout << "the object: " << endl;
		D.print();
		std::cout << std::endl;
	} while (choice != 4);
}