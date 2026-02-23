#ifndef BIGINT_HPP
# define BIGINT_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <ostream>

class bigint {
    private:
        std::string arb;
    public:
        bigint();
        bigint(const bigint &src);
        bigint(int i);
        ~bigint(); 
        std::string getArb() const;
        bigint operator+(const bigint &src) const;
        bigint &operator+=(const bigint &src); 
        bigint operator++();
        bigint &operator++(int);
        bigint operator<<(unsigned int i) const;
        bigint operator<<=(unsigned int i);
        bigint operator<<=(const bigint i);
        bigint operator>>(unsigned int i) const;
        bigint operator>>=(unsigned int i);
        bigint operator>>=(const bigint i);
};

std::ostream &operator<<(std::ostream &os, const bigint &src);

#endif