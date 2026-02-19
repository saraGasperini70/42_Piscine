#ifndef BIGINT_HPP
# define BIGINT_HPP

# include <string>
# include <sstream>
# include <iostream>

class bigint {
    private:
        std::string arb;
    public:
        bigint();
        bigint(bigint &src);
        bigint(int i);
        ~bigint();
        std::string getArb() const;
        bigint operator+(const bigint &src) const;
        bigint &operator+=(const bigint &src); 
};

#endif