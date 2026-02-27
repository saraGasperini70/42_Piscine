#include "searchable_tree_bag.hpp"

searchable_tree_bag::searchable_tree_bag() {}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag &src) : tree_bag(src) {}

searchable_tree_bag &searchable_tree_bag::operator=(const searchable_tree_bag &src) {
	if (this != &src) {
		tree_bag::operator=(src);
	}
	return (*this);
}

searchable_tree_bag::~searchable_tree_bag() {}

bool searchable_tree_bag::search(node *current, int value) const {
	if (current == NULL)
		return false;
	if (current->value == value)
		return true;
	if (current->value > value && current->l)
		return search(current->l, value);
	if (current->value < value && current->r)
          return search(current->r, value);
	return false;
}

bool searchable_tree_bag::has(int value) const {
	return search(tree, value);
}
