#ifndef SET_HPP
# define SET_HPP

# include "searchable_bag.hpp"
# include <cstddef>

class set {
	private:
		searchable_bag &bag;
	public:
        set(searchable_bag &s_bag);
        set(const set &src);
		set &operator=(const set &src);
		~set();
		void insert(int);
		void insert(int *, int);
		void clear();
		const searchable_bag &get_bag() const;
		bool has(int) const;
		void print() const;
};

#endif
