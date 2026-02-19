#include "vect2.hpp"

vect2::vect2() { this->a = 0; this->b = 0; }

vect2::vect2(int n1, int n2) {
    this->a = n1;
    this->b = n2;
}

vect2::vect2(const vect2 &src) {
    this->a = src.a;
    this->b = src.b;
}

vect2::~vect2() {}

vect2 &vect2::operator++() {
    this->a += 1;
    this->b += 1;
    return *this;
}

vect2 vect2::operator++(int) {
    vect2 temp(*this);
    this->a += 1;
    this->b += 2;
    return (temp);
}

vect2 &vect2::operator--() {
    this->a -= 1;
    this->b -= 1;
    return *this;
}

vect2 vect2::operator--(int) {
    vect2 temp(*this);
    this->a -= 1;
    this->b -= 2;
    return (temp);
}

vect2 &vect2::operator+=(const vect2 &src) {
    this->a += src.a;
    this->b += src.b;
    return *this;
}

vect2 &vect2::operator+(const vect2 &src) const {
    vect2 *temp = new vect2;
    temp->a = this->a + src.a;
    temp->b = this->b + src.a;
    return (*temp);
}

vect2 &vect2::operator-=(const vect2 &src) {
    this->a -= src.a;
    this->b -= src.b;
    return *this;
}

vect2 &vect2::operator-(const vect2 &src) {
    vect2 &temp(*this);
    temp -= src;
    return (temp);
}

vect2 &vect2::operator-() {
    this->a *= -1;
    this->b *= -1;
    return (*this);
}

vect2 &vect2::operator*=(const vect2 &src) {
    this->a *= src.a;
    this->b *= src.b;
    return *this;
}

vect2 &vect2::operator*=(int i) {
    this->a *= i;
    this->b *= i;
    return *this;
}

vect2 &vect2::operator*(const vect2 &src) {
    vect2 *temp = new vect2 (this->a * src.a, this->b * src.b);
    return (*temp);
}

vect2 vect2::operator*(int i) const {
    vect2 *temp = new vect2 (this->a * i, this->b * i);
    std::cout << this->a * i << " " << this->b * i << std::endl;
    return (*temp);
}

bool vect2::operator==(const vect2 &src) {
    return (this->a == src.a && this->b == src.b);
}

bool vect2::operator!=(const vect2 &src) {
    return (!(this->a == src.a && this->b == src.b));
}

int &vect2::operator[](int i) {
    if (i == 0)
        return this->a;
    else
        return this->b;
}

int vect2::operator[](int i) const {
    if (i == 0)
        return this->a;
    else
        return this->b;
}

vect2 &vect2::operator=(const vect2 &src) {
    if (*this != src) {
        this->a = src.a;
        this->b = src.b;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const vect2 &src) {
    std::cout << "{" << src[0] << ", " << src[1] << "}";
    return os;
}

vect2 operator*(int i, const vect2 &src) {
    vect2 temp(src * i);
    return (temp);
}