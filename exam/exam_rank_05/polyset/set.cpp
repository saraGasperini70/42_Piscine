#include "set.hpp"

set::set(searchable_bag &s_bag) : bag(s_bag) {}

set::set(const set &src) : bag(src.bag) {}

set &set::operator=(const set &src) {
	if (this != &src) {
		this->bag = src.bag;
	}
	return (*this);
}

set::~set() {}

void set::insert(int value) {
	if (this->bag.has(value))
		return;
	this->bag.insert(value);
}
void set::insert(int *values, int size) {
  for (int i = 0; i < size; i++) {
    if (!this->bag.has(values[i]))
		insert(values[i]);
	}
}
void set::clear() { this->bag.clear(); }

bool set::has(int value) const { return this->bag.has(value); }

void set::print() const { this->bag.print(); }

const searchable_bag &set::get_bag() const { return this->bag; }
