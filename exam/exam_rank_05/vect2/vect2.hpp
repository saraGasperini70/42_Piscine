#ifndef VECT2_HPP
# define VECT2_HPP

# include <iostream>

class vect2 
{
    private:
        int a;
        int b;
    public:
        // Constructors and deconstructor
        vect2();
        vect2(int n1, int n2);
        vect2(const vect2 &src);
        ~vect2();
        // Increment operators
        vect2 &operator++();
        vect2 operator++(int);
        // Decrement operators
        vect2 &operator--();
        vect2 operator--(int);
        // Math operators
        vect2 &operator+=(const vect2 &src);
        vect2 &operator-=(const vect2 &src);
        vect2 &operator*=(const vect2 &src);
        vect2 &operator*=(int);
        vect2 &operator+(const vect2 &src) const;
        vect2 &operator-(const vect2 &src);
        vect2 &operator-();
        vect2 &operator*(const vect2 &src);
        vect2 &operator*(int i);
        vect2 operator*(int i) const;
        bool operator==(const vect2 &src);
        bool operator!=(const vect2 &src);
        int &operator[](int i);
        int operator[](int i) const;
        vect2 &operator=(const vect2 &src);
};

vect2 operator*(int i, const vect2 &src);
std::ostream &operator<<(std::ostream &os, const vect2 &src);
#endif