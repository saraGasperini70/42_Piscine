#include "bigint.hpp"

std::string intToStr(int i) {
    std::stringstream ss;
    ss << i;
    std::string result;
    ss >> result;
    std::cout << result << std::endl;
    return (result);
}

int strToInt(std::string str) {
    int result = 0;
    for (size_t i = 0; i < str.length(); i++) {
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
        result += intToStr(res);
     }
    if (carry != 0) {
        result += intToStr(carry);
    }
    return reverse(result);
}

bigint::bigint() { this->arb = "0"; }

bigint::bigint(bigint &src) {
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