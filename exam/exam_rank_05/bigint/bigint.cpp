#include "bigint.hpp"

std::string intToStr(int i) {
    std::stringstream ss;
    ss << i;
    std::string result;
    ss >> result;
    // std::cout << result << std::endl;
    return (result);
}

int strToInt(std::string str) {
    int result = 0;
    for (size_t i = 0; i < str.length(); i++) {
        if (str.length() == 1)
            result = (str[i] - '0');
        else
            result += (str[i] - '0') * 10;
    }
    return (result);
}

std::string reverse(std::string str) { //This is working
    std::string reversed;
    for (size_t i = str.length(); i > 0; --i){
        reversed += str[i - 1];
    } return reversed;
}

std::string addition(const std::string str1, const std::string str2) {
    std::string rev1 = reverse(str1);
    std::string rev2 = reverse(str2);
    std::string result;
    if (rev1.length() > rev2.length()) 
        rev2.insert(rev2.end(), rev1.length() - rev2.length(), '0');
    else if (rev2.length() > rev1.length())
        rev1.insert(rev1.end(), rev2.length() - rev1.length(), '0');

    int carry = 0;
    int res = 0;
    for (size_t i = 0; i < rev1.length(); i++) {
        res = (rev1[i] - '0') + (rev2[i] - '0') + carry;
        // std::cout << "(" << rev1[i] - '0' << " + " << rev2[i] - '0' << ")" << std::endl;
        // std::cout << "(" << rev1[i] << " + " << rev2[i] << ")" << std::endl;
        if (res > 9) {
            carry = res / 10;
            res %= 10;
        }
        else 
            carry = 0;
        result += intToStr(res);
     }
    if (carry != 0) {
        result += intToStr(carry);
    }
    return reverse(result);
}

bigint::bigint() { this->arb = "0"; }

bigint::bigint(const bigint &src) {
    this->arb = src.arb;
}

bigint::bigint(int i) {
    this->arb = intToStr(i);
}

bigint::~bigint() {}

std::string bigint::getArb() const {
    return this->arb;
}

bigint &bigint::operator+=(const bigint &src) {
    this->arb = addition(this->arb, src.arb);
    return *this;
}

bigint bigint::operator+(const bigint &src) const {
    bigint temp(src);
    temp += *this;
    return temp;
}

bigint bigint::operator++() {
    *this += bigint(1);
    return *this;
}

bigint &bigint::operator++(int) {
    bigint *temp = this;
    *(this) = *(this) + bigint(1);
    return *temp;
}

bigint bigint::operator<<(unsigned int i) const {
    bigint temp = * this;
    for (size_t j = 0; j < i; j++) {
        temp.arb.push_back('0');
    }
    return (temp);
}

bigint bigint::operator<<=(unsigned int i) {
    *(this) = *(this) << i;
    return (*this);
}

bigint bigint::operator<<=(const bigint i) {
    *(this) <<= strToInt(i.arb);
    return *(this);
}

bigint bigint::operator>>(unsigned int i) const {
    bigint temp = * this;
    if (i > temp.arb.length()) {
        std::cout << "i: " << i << "and temp.arb.length:" << temp.arb.length() << std::endl;
        temp.arb = '0';
    }
    else {
        std::cout << "i: " << i << "and temp.arb.length:" << temp.arb.length() << std::endl;
        temp.arb.erase(temp.arb.length() - i, i);
    }
    return temp;
}

bigint bigint::operator>>=(unsigned int i) {
    *(this) = *(this) >> i;
    return *(this);
}
        
bigint bigint::operator>>=(const bigint i) {
    *(this) = *(this) >> strToInt(i.arb);
    return *(this);
}

std::ostream &operator<<(std::ostream &os, const bigint &src) {
    os << src.getArb();
    return os;
}