#ifndef SEARCHABLE_TREE_BAG_HPP
# define SEARCHABLE_TREE_BAG

# include "tree_bag.hpp"
# include "searchable_bag.hpp"
# include <cstddef>

class searchable_tree_bag : public tree_bag, public searchable_bag {
	public:
        searchable_tree_bag();
        searchable_tree_bag(const searchable_tree_bag &src);
        searchable_tree_bag &operator=(const searchable_tree_bag &src);
        ~searchable_tree_bag();
        bool search(node *current, int value) const;
        bool has(int) const;
};
#endif
