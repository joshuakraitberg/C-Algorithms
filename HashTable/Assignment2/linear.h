#pragma once


#include <string>
#include <utility>
#include <functional>
#include <cstring>


using namespace std;

template <class TYPE>
class Table;

float ALLOCATE_RATIO = 1.5;

template <class TYPE>
class HashTable : public Table<TYPE> {
	unsigned _size;
	unsigned _capacity;
	//
	char* _spotFilled;
	string* _keys;
	TYPE* _data;

	unsigned find_next(unsigned hash, const string& key);
	void grow();
	void print();
	void add(string&& key, TYPE&& value);
public:
	HashTable(int maxExpected);
	HashTable(const HashTable& other);
	HashTable(HashTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const HashTable& operator=(const HashTable& other);
	virtual const HashTable& operator=(HashTable&& other);
	virtual ~HashTable();
};

template <class TYPE>
HashTable<TYPE>::HashTable(int maxExpected) : _size(0) {
	_capacity = static_cast<unsigned>(maxExpected * ALLOCATE_RATIO);
	if (_capacity < maxExpected) {
		throw std::string("Invalid table size");
	}

	_spotFilled = new char[_capacity];
	_keys = new string[_capacity];
	_data = new TYPE[_capacity];

	for (auto i = 0u; i < _capacity; ++i) {
		_spotFilled[i] = false;
	}
}

template <class TYPE>
HashTable<TYPE>::HashTable(const HashTable<TYPE>& other) : _spotFilled(nullptr), _keys(nullptr), _data(nullptr) {
	operator=(other);
}

template <class TYPE>
HashTable<TYPE>::HashTable(HashTable<TYPE>&& other) : _spotFilled(nullptr), _keys(nullptr), _data(nullptr) {
	operator=(std::move(other));
}

template <class TYPE>
bool HashTable<TYPE>::update(const string& key, const TYPE& value) {
	if (_size*ALLOCATE_RATIO >= _capacity) {
		grow();
	}

	unsigned hash = std::hash<string>{} (key);
	hash = hash - _capacity * (hash / _capacity);

	unsigned index = find_next(hash, key);

	if (!_spotFilled[index]) {
		_spotFilled[index] = 1;
		_keys[index] = key;
		_data[index] = value;
		++_size;
		return true;
	}
	else {
		_data[index] = value;
		return true;
	}
	return false;
}

template <class TYPE>
bool HashTable<TYPE>::remove(const string& key) {
	unsigned hash = std::hash<string>{} (key);
	hash = hash - _capacity * (hash / _capacity);

	unsigned index = find_next(hash, key);

	if (_spotFilled[index]) {
		_spotFilled[index] = 0;
		--_size;

		while (_spotFilled[++index]) {
			unsigned hash = std::hash<string>{} (_keys[index]);
			hash = hash - _capacity * (hash / _capacity);

			unsigned next = find_next(hash, _keys[index]);

			if (next != index) {
				_spotFilled[next] = 1;
				_spotFilled[index] = 0;
				_keys[next] = std::move(_keys[index]);
				_data[next] = std::move(_data[index]);
			}
		}
		return true;
	}
	return false;
}

template <class TYPE>
bool HashTable<TYPE>::find(const string& key, TYPE& value) {
	unsigned hash = std::hash<string>{} (key);
	hash = hash - _capacity * (hash / _capacity);

	unsigned index = find_next(hash, key);

	if (_spotFilled[index]) {
		value = _data[index];
		return true;
	}
	return false;
}

template <class TYPE>
const HashTable<TYPE>& HashTable<TYPE>::operator=(const HashTable<TYPE>& other) {
	if (_capacity != other._capacity) {
		if (_spotFilled) {
			delete[] _spotFilled;
			_spotFilled = new char[other._capacity];
		}
		if (_keys) {
			delete[] _keys;
			_keys = new string[other._capacity];
		}
		if (_data) {
			delete[] _data;
			_data = new TYPE[other._capacity];
		}
	}
	if (!_spotFilled) {
		_spotFilled = new char[other._capacity];
	}
	if (!_keys) {
		_keys = new string[other._capacity];
	}
	if (!_data) {
		_data = new TYPE[other._capacity];
	}


	_size = other._size;
	_capacity = other._capacity;

	memcpy(_spotFilled, other._spotFilled, _capacity);

	for (auto i = 0u; i < other._capacity; ++i) {
		//_spotFilled[i] = other._spotFilled[i];
		_keys[i] = other._keys[i];
		_data[i] = other._data[i];
	}

	return *this;
}

template <class TYPE>
const HashTable<TYPE>& HashTable<TYPE>::operator=(HashTable<TYPE>&& other) {
	if (_spotFilled) {
		delete[] _spotFilled;
	}
	if (_keys) {
		delete[] _keys;
	}
	if (_data) {
		delete[] _data;
	}

	_size = other._size;
	other._size = 0;
	_capacity = other._capacity;
	other._capacity = 0;

	_spotFilled = other._spotFilled;
	other._spotFilled = nullptr;
	_keys = other._keys;
	other._keys = nullptr;
	_data = other._data;
	other._data = nullptr;

	return *this;
}

template <class TYPE>
HashTable<TYPE>::~HashTable() {
	if (_spotFilled) {
		delete[] _spotFilled;
	}
	if (_keys) {
		delete[] _keys;
	}
	if (_data) {
		delete[] _data;
	}
}

template <class TYPE>
void HashTable<TYPE>::grow() {
	HashTable<TYPE> larger(_capacity);

	for (auto i = 0u; i < _capacity; ++i) {
		if (_spotFilled[i]) {
			larger.add(std::move(_keys[i]), std::move(_data[i]));
		}
	}

	*this = std::move(larger);
}

template <class TYPE>
void HashTable<TYPE>::add(string&& key, TYPE&& value) {
	unsigned hash = std::hash<string>{} (key);
	hash = hash - _capacity * (hash / _capacity);

	unsigned index = find_next(hash, key);

	_spotFilled[index] = 1;
	_keys[index] = std::move(key);
	_data[index] = std::move(value);
	++_size;
}

template <class TYPE>
unsigned HashTable<TYPE>::find_next(unsigned hash, const string& key) {
	unsigned index = hash;

	for (; index < _capacity; ++index) {
		if (!_spotFilled[index] || key == _keys[index]) {
			return index;
		}
	}
	for (index = 0; index < hash; ++index) {
		if (!_spotFilled[index] || key == _keys[index]) {
			return index;
		}
	}

	throw string("HashTable is full.");
	return -1;
}