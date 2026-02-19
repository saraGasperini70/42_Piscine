# include "bigint.hpp"

int main() {
    bigint bg1(1337);
    bigint bg2;
    const bigint bg3(5);
    // std::cout << bg1.getArb() <<  std::endl;
    // std::cout << bg2.getArb() <<  std::endl;
    // std::cout << bg3.getArb() <<  std::endl;
    bg1 += bg3;
    // std::cout << bg1.getArb() <<  std::endl;
}