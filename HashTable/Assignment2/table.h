#pragma once

/*

	Joshua Kraitberg
	Assignment 2
	BTP500
	Fall 2016

*/


#include <string>
#include <utility>
#include <functional>
#include <cstring>

using namespace std;


/*

MODIFIED

constructor
grow
search
find
update
remove
destructor
copy
assignment
sort
bsearch

Record::constructor

*/


template <class TYPE>
class Table {
public:
	Table() {}
	virtual bool update(const string& key, const TYPE& value) = 0;
	virtual bool remove(const string& key) = 0;
	virtual bool find(const string& key, TYPE& value) = 0;
	virtual ~Table() {}
};

template <class TYPE>
class SimpleTable :public Table<TYPE> {

	struct Record {
		TYPE data_;
		string key_;
		Record(const string& key, const TYPE& data) {
			key_ = key;
			data_ = data;
		}

		Record() {}
	};

	Record* records_;   //the table
	int max_;           //capacity of the array
	int size_;          //current number of records held
	int bsearch(const string& key, int begin, int end);
	int search(const string& key);
	void grow();
public:
	SimpleTable(int maxExpected);
	SimpleTable(const SimpleTable& other);
	SimpleTable(SimpleTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other);
	virtual const SimpleTable& operator=(SimpleTable&& other);
	virtual ~SimpleTable();
};


//returns index of where key is found.
template <class TYPE>
int SimpleTable<TYPE>::bsearch(const string& key, int begin, int end) {
	if (begin > end) { return -1; }
	
	int mid = (begin + end) / 2;

	if (records_[mid].key_ == key) { return mid; }
	else if (records_[mid].key_ < key) { return bsearch(key, mid + 1, end); }
	return bsearch(key, begin, mid - 1);
}

//returns index of where key is found.
template <class TYPE>
int SimpleTable<TYPE>::search(const string& key) {
	return bsearch(key, 0, size_ - 1);
}

//grow the array by one element
template <class TYPE>
void SimpleTable<TYPE>::grow() {
	max_ <<= 1;
	Record* grown = new Record[max_];

	for (auto i = 0u; i < size_; i++) {
		grown[i] = std::move(records_[i]);
	}

	delete[] records_;
	records_ = grown;
}

/* none of the code in the function definitions below are correct.  You can replace what you need
*/
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int maxExpected) : Table<TYPE>() {
	records_ = new Record[maxExpected];
	max_ = maxExpected;
	size_ = 0;
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other) {
	records_ = new Record[other.max_];
	max_ = other.max_;
	size_ = other.size_;

	for (int i = 0; i < size_; i++) {
		records_[i] = other.records_[i];
	}
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other) {
	size_ = other.size_;
	max_ = other.max_;
	records_ = other.records_;
	other.records_ = nullptr;
	other.size_ = 0;
	other.max_ = 0;
}

template <class TYPE>
bool SimpleTable<TYPE>::update(const string& key, const TYPE& value) {
	int idx = search(key);
	if (idx == -1) {
		if (size_ + 1 >= max_) {
			grow();
		}
		auto i = 0u;
		for (; i < size_ && records_[i].key_ < key; ++i) { }
		for (auto j = size_; j > i; --j) {
			records_[j] = std::move(records_[j - 1]);
		}
		++size_;
		records_[i] = Record(key, value);
	}
	else {
		records_[idx].data_ = value;
	}
	return true;
}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key) {
	int idx = search(key);
	if (idx != -1) {
		for (int i = idx; i < size_ - 1; i++) {
			records_[i] = std::move(records_[i + 1]);
		}
		size_--;
		return true;
	}
	else {
		return false;
	}
}

template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value) {
	int idx = search(key);
	if (idx == -1)
		return false;
	else {
		value = records_[idx].data_;
		return true;
	}
}

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other) {
	if (this != &other) {
		if (records_) {
			delete[] records_;
		}

		records_ = new Record[other.max_];
		max_ = other.max_;
		size_ = other.size_;

		for (int i = 0; i < size_; i++) {
			records_[i] = other.records_[i];
		}
	}
	return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other) {
	swap(records_, other.records_);
	swap(size_, other.size_);
	swap(max_, other.max_);
	return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable() {
	if (records_) {
		delete[] records_;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Maximum size a jump can be
const int MAX_HOP_BITS = 16;

// Amount to over allocate
const double ALLOCATE_RATIO = 1.5;


// Bit masks for bit manipulations
const static unsigned char BIT_MASKS[8] = {
	0x01, 0x02, 0x04, 0x08, 0x10,
	0x20, 0x40, 0x80
};


// dynamic array of bits
class BitField {
	unsigned int _size;
	unsigned char* _bytes;
public:
	BitField(unsigned int size = 0) : _size(size), _bytes(new unsigned char[(size + 7) / 8]) {
		for (auto i = 0u; i < (size + 7) / 8; ++i) {
			_bytes[i] = 0;
		}
	}

	~BitField() { if (_bytes) delete[] _bytes; }


	BitField& operator=(const BitField& other) {
		if (_bytes) delete[] _bytes;

		_size = other._size;
		_bytes = new unsigned char[(_size + 7) / 8];
		std::memcpy(_bytes, other._bytes, (_size + 7) / 8);

		return *this;
	}

	BitField& operator=(BitField&& other) {
		if (_bytes) delete[] _bytes;

		_size = other._size;
		other._size = 0;

		_bytes = other._bytes;
		other._bytes = nullptr;

		return *this;
	}

	// Set the bit at i to state
	void set(unsigned int i, bool state) {
		unsigned byte = i / 8;
		unsigned bit = i - 8 * byte;

		if (state)
			_bytes[byte] |= BIT_MASKS[bit];
		else
			_bytes[byte] &= ~BIT_MASKS[bit];
	}

	// return the state of the bit at i
	bool get(unsigned int i) const {
		unsigned byte = i / 8;
		unsigned bit = i - 8 * byte;

		return static_cast<bool>(_bytes[byte] & BIT_MASKS[bit]);
	}

	// get the index of the next bit that is false, returns _size if none 
	unsigned next(unsigned start) const {
		unsigned i = start;
		for (; get(i) && (i < _size); ++i) {}
		return i;
	}
};

template <class TYPE>
struct Bucket {
	string _key;
	TYPE _value;
};

// static array of bits
template<size_t Bits>
class HopMap_Base {
	unsigned char _map[(Bits + 7) / 8];
public:
	HopMap_Base(bool fill = false) {
		std::memset(_map, (fill ? 0xFFu : 0u), (Bits + 7) / 8);
	}

	HopMap_Base(const HopMap_Base& other) {
		operator=(other);
	}

	HopMap_Base(HopMap_Base&& other) {
		operator=(std::move(other));
	}

	HopMap_Base& operator=(const HopMap_Base& other) {
		if (this != &other) {
			std::memcpy(_map, other._map, (Bits + 7) / 8);
		}

		return *this;
	}

	HopMap_Base& operator=(HopMap_Base&& other) {
		std::memcpy(_map, other._map, (Bits + 7) / 8);

		// Is this good practice?
		std::memset(other._map, 0u, (Bits + 7) / 8);

		return *this;
	}

	// return the state of the bit at i
	bool get(unsigned i) const {
		unsigned byte = i / 8;
		unsigned bit = i - 8 * byte;

		return static_cast<bool>(_map[byte] & BIT_MASKS[bit]);
	}

	// Set the bit at i to state
	void set(unsigned i, bool state) {
		unsigned byte = i / 8;
		unsigned bit = i - 8 * byte;

		if (state) {
			_map[byte] |= BIT_MASKS[bit];
		}
		else {
			_map[byte] &= ~BIT_MASKS[bit];
		}
	}

	// get the index of the next bit that is true, returns _size if none 
	unsigned next(unsigned start) const {
		unsigned i = start;
		for (; !get(i) && (i < Bits); ++i) {}
		return i;
	}
};

typedef HopMap_Base<MAX_HOP_BITS> HopMap;

template <class TYPE>
class HashTable : public Table<TYPE> {
	unsigned _size;
	unsigned _capacity;
	unsigned _hop;
	//
	BitField _occupied;
	HopMap* _hopMaps;
	Bucket<TYPE>* _buckets;

	template<class Key, class Value>
	bool add(Key&& key, Value&& value);
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
	void print();
};

// constructor 
// creates a table absed on the max expected size of the table
template <class TYPE>
HashTable<TYPE>::HashTable(int maxExpected) : _size(0) {
	unsigned size = static_cast<unsigned>(maxExpected * ALLOCATE_RATIO);
	if (size < maxExpected) {
		throw std::string("Invalid table size");
	}

	_capacity = size;
	_hop = (size > MAX_HOP_BITS ? MAX_HOP_BITS : size);
	_buckets = new Bucket<TYPE>[size];
	_occupied = size;
	_hopMaps = new HopMap[size];
}

// copy constructor
template <class TYPE>
HashTable<TYPE>::HashTable(const HashTable<TYPE>& other) : _buckets(nullptr), _hopMaps(nullptr) {
	operator=(other);
}


// move constructor
template <class TYPE>
HashTable<TYPE>::HashTable(HashTable<TYPE>&& other) : _buckets(nullptr), _hopMaps(nullptr) {
	operator=(std::move(other));
}

// Accepts a key and value
// Will update the key to the new value if key exists or else it will add new key and value
// return true if key was added or updated false otherwise
template <class TYPE>
bool HashTable<TYPE>::update(const string& key, const TYPE& value) {
	bool updated = false;

	unsigned begin = std::hash<string>{} (key);
	begin = begin - _capacity * (begin / _capacity);

	// First try update
	for (auto i = 0; !updated && (i < _hop); ++i) {
		i = _hopMaps[begin].next(i);
		unsigned pos = begin + i;
		pos = pos - _capacity * (pos / _capacity);
		if ((i < _hop) && _buckets[pos]._key == key) {
			_buckets[pos]._value = value;
			updated = true;
		}
	}

	// Add if not found
	if (!updated) {
		if (!_occupied.get(begin)) {
			_buckets[begin]._key = key;
			_buckets[begin]._value = value;
			_hopMaps[begin].set(0, true);
			_occupied.set(begin, true);
			updated = true;
			++_size;
		}
		else {
			updated = add(key, value);
		}
	}

	return updated;
}

// adds the key and value to the table
template<class TYPE>
template<class Key, class Value>
bool HashTable<TYPE>::add(Key&& key, Value&& value) {
	bool done = false;

	if (_size != _capacity) {
		unsigned begin = std::hash<string>{} (key);
		begin = begin - _capacity * (begin / _capacity);

		bool scanning = false;

		unsigned i = begin;
			
		i = _occupied.next(begin);
		if (i >= _capacity) {
			i = _occupied.next(0);
		}

		unsigned diff = (begin > i) ? (i + _capacity - begin) : (i - begin);
		if (diff < _hop) {
			_buckets[i]._key = std::forward<Key>(key);
			_buckets[i]._value = std::forward<Value>(value);
			_hopMaps[begin].set(diff, true);
			_occupied.set(i, true);
			done = true;
			++_size;
		}
		else {
			bool completed = false;
			unsigned empty = i;
			do {
				bool completed = false;
				unsigned swap;
				unsigned step = (diff > (_hop - 1)) ? _hop - 1 : diff;

				for (i = step; !completed && (i > 0); --i) {
					unsigned temp2 = (empty - i + _capacity);
					temp2 = temp2 - _capacity  * (temp2 / _capacity);
					swap = _hopMaps[temp2].next(0);
					if (swap < _hop && swap < i) {
						unsigned temp = (empty - i + swap + _capacity); 
						temp = temp - _capacity  * (temp / _capacity);					
						_buckets[empty]._key = std::move(_buckets[temp]._key);
						_buckets[empty]._value = std::move(_buckets[temp]._value);
						_occupied.set(temp, false);
						_occupied.set(empty, true);
						_hopMaps[temp2].set(swap, false);
						_hopMaps[temp2].set(i, true);
						empty = temp;
						diff -= (i - swap);
						completed = true;
					}
				}

				if (completed) {
					if (diff < _hop) {
						_buckets[empty]._key = std::forward<Key>(key); 
						_buckets[empty]._value = std::forward<Value>(value);
						_occupied.set(empty, true);
						_hopMaps[begin].set(diff, true);
						done = true;
						++_size;
					}
				}
				else {
					// resize
					HashTable<TYPE> new_table(_capacity * ALLOCATE_RATIO);

					for (auto i = 0u; i < _capacity; ++i) {
						if (_occupied.get(i)) {
							new_table.add(std::move(_buckets[i]._key), std::move(_buckets[i]._value));
						}
					}
					new_table.add(std::forward<Key>(key), std::forward<Value>(value));
					done = true;

					operator=(std::move(new_table));
				}
			} while (!done);
		}
	}
	return done;
}

// Remove the key from the table
// if the key was removed returns true false otherwise
template <class TYPE>
bool HashTable<TYPE>::remove(const string& key) {
	bool removed = false;

	unsigned hash = std::hash<string>{} (key);
	hash = hash - _capacity * (hash / _capacity);

	// First try update
	for (auto i = 0; !removed && (i < _hop); ++i) {
		i = _hopMaps[hash].next(i);
		unsigned pos = hash + i;
		pos = pos - _capacity * (pos / _capacity);
		if ((i < _hop) && _buckets[pos]._key == key) {
			_occupied.set(pos, false);
			_hopMaps[hash].set(i, false);
			removed = true;
			--_size;
		}
	}

	return removed;
}

// find the key in the table
// if the key was found assign the data associated with the key to value
// if the key was found return true else false
template <class TYPE>
bool HashTable<TYPE>::find(const string& key, TYPE& value) {
	bool found = false;
	bool completed = false;

	unsigned hash = std::hash<string>{} (key);
	hash = hash - _capacity * (hash / _capacity);

	for (auto i = 0; !found && (i < _hop); ++i) {
		i = _hopMaps[hash].next(i);
		unsigned pos = hash + i;
		pos = pos -_capacity * (pos / _capacity);
		if ((i < _hop) && _buckets[pos]._key == key) {
			value = _buckets[pos]._value;
			found = true;
		}
	}

	return found;
}

// Copy operator
template <class TYPE>
const HashTable<TYPE>& HashTable<TYPE>::operator=(const HashTable<TYPE>& other) {
	if (_buckets) {
		if (_capacity != other._capacity) {
			delete[] _buckets;
			_buckets = new Bucket<TYPE>[other._capacity];
		}
	}
	else {
		_buckets = new Bucket<TYPE>[other._capacity];
	}
	if (_hopMaps) {
		if (_capacity != other._capacity) {
			delete[] _hopMaps;
			_hopMaps = new HopMap[other._capacity];
		}
	}
	else {
		_hopMaps = new HopMap[other._capacity];
	}

	_size = other._size;
	_capacity = other._capacity;
	_occupied = other._occupied;
	_hop = other._hop;

	for (auto i = 0u; i < _capacity; ++i) {
		_buckets[i]._key = other._buckets[i]._key;
		_buckets[i]._value = other._buckets[i]._value;
		_hopMaps[i] = other._hopMaps[i];
	}

	return *this;
}

// Move operator
template <class TYPE>
const HashTable<TYPE>& HashTable<TYPE>::operator=(HashTable<TYPE>&& other) {
	if (_buckets) {
		delete[] _buckets;
	}
	if (_hopMaps) {
		delete[] _hopMaps;
	}

	_size = other._size;
	other._size = 0;
	_capacity = other._capacity;
	other._capacity = 0;
	_hop = other._hop;
	other._hop = 0;

	_occupied = std::move(other._occupied);

	_buckets = other._buckets;
	other._buckets = nullptr;
	_hopMaps = other._hopMaps;
	other._hopMaps = nullptr;

	return *this;
}


// destructor
template <class TYPE>
HashTable<TYPE>::~HashTable() {
	if (_buckets) {
		delete[] _buckets;
	}
	if (_hopMaps) {
		delete[] _hopMaps;
	}
}