#pragma once


bool is_prime(size_t x) {
	for (auto i = 3; true; i += 2)
	{
		size_t q = x / i;
		if (q < i) {
			return true;
		}
		if (x == q * i) {
			return false;
		}
	}
	return true;
}

size_t next_prime(size_t x) {
	if (x <= 2) {
		return 2;
	}
	if (!(x & 1)) {
		++x;
	}
	for (; !is_prime(x); x += 2) {}
	return x;
}

//size_t congruent_next_prime(size_t x) {
//	size_t prime = next_prime(x);
//	for (; (prime - 4 * (prime / 4)) != 3; next_prime(prime)) {}
//	return prime;
//}

template <class Key, class TYPE>
struct Bucket {
	Key _key;
	TYPE _value;
};

template <class TYPE>
class HashTable : public Table<TYPE> {
	unsigned _size;
	unsigned _capacity;
	//
	char* _filled;
	string* _keys;
	TYPE* _values;
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

template <class TYPE>
HashTable<TYPE>::HashTable(int maxExpected) : _size(0) {
	//_capacity = congruent_next_prime(maxExpected * 2 + 1);
	//_capacity = next_prime(maxExpected * 2 + 1);
	if (_capacity < maxExpected) {
		throw std::string("Invalid table size");
	}

	_filled = new char[_capacity];
	_buckets = new Bucket<TYPE>[_capacity];

	for (auto i = 0; i < _capacity ++i) {
		_filled = 0;
	}
}

template <class TYPE>
HashTable<TYPE>::HashTable(const HashTable<TYPE>& other) : _filled(nullptr), _buckets(nullptr) {
	operator=(other);
}

template <class TYPE>
HashTable<TYPE>::HashTable(HashTable<TYPE>&& other) : _filled(nullptr), _buckets(nullptr) {
	operator=(std::move(other));
}

template <class TYPE>
void HashTable<TYPE>::print() {
	//std::cout << "HASHTABLE -> " << _size << '/' << _capacity << '\n';
	//for (auto i = 0u; i < _capacity; ++i) {
	//	if (_occupied.get(i)) {
	//		std::cout << i << ' ' << _buckets[i]._key << '\n';
	//	}
	//}
	//std::cout << '\n';
}

template <class TYPE>
bool HashTable<TYPE>::update(const string& key, const TYPE& value) {
	bool updated = false;

	unsigned hash = std::hash<string>{};
	hash = hash - _capacity * (hash / _capacity); // Avoid modulos


	if (_size >= _capacity / 2 + 1)) {
		grow();
	}
	if (_filled[hash] && _buckets[hash]._key == key) {

	}
	else {

	}

	// First try update
	for (auto i = 0; !updated && (i < _hop); ++i) {
		i = _hopMaps[begin].next(i);
		if ((i < _hop) && _buckets[(begin + i) % _capacity]._key == key) {
			_buckets[(begin + i) % _capacity]._value = value;
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

template<class TYPE>
template<class Key, class Value>
bool HashTable<TYPE>::add(Key&& key, Value&& value) {
	bool done = false;

	if (_size != _capacity) {
		unsigned begin = std::hash<string>{} (key) % _capacity;
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
					swap = _hopMaps[(empty - i + _capacity) % _capacity].next(0);
					if (swap < _hop && swap < i) {
						unsigned temp = (empty - i + swap + _capacity) % _capacity;
						unsigned temp2 = (empty - i + _capacity) % _capacity;
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

template <class TYPE>
bool HashTable<TYPE>::remove(const string& key) {
	bool removed = false;

	unsigned hash = std::hash<string>{} (key) % _capacity;

	// First try update
	for (auto i = 0; !removed && (i < _hop); ++i) {
		i = _hopMaps[hash].next(i);
		if ((i < _hop) && _buckets[(hash + i) % _capacity]._key == key) {
			_occupied.set((hash + i) % _capacity, false);
			_hopMaps[hash].set(i, false);
			removed = true;
			--_size;
		}
	}

	return removed;
}

template <class TYPE>
bool HashTable<TYPE>::find(const string& key, TYPE& value) {
	bool found = false;
	bool completed = false;

	unsigned hash = std::hash<string>{} (key) % _capacity;

	for (auto i = 0; !found && (i < _hop); ++i) {
		i = _hopMaps[hash].next(i);
		if ((i < _hop) && _buckets[(hash + i) % _capacity]._key == key) {
			value = _buckets[(hash + i) % _capacity]._value;
			found = true;
		}
	}

	return found;
}

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

template <class TYPE>
HashTable<TYPE>::~HashTable() {
	if (_buckets) {
		delete[] _buckets;
	}
	if (_hopMaps) {
		delete[] _hopMaps;
	}
}

