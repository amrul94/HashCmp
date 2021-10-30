
#include <bitset>
#include <cassert>
#include <iostream>
#include <random>

uint16_t Mod(uint64_t source) {
    return static_cast<uint16_t>(source);
}

uint64_t Mask(uint64_t source) {
    const static uint16_t mask = -1;
    return source & mask;
}

uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift = 0) {
    const auto mask = static_cast<uint64_t>(std::pow(2, mask_bits)) - 1;
    return (src >> shift) & mask;
}


int main() {
    /*std::random_device rd;
    std::mt19937_64 gen{rd()};
    std::string str = "string";
    const uint64_t number = UINT64_MAX;
    std::bitset<64> orig{number};
    std::cout << orig << std::endl;
    for (uint16_t i = 1; i <= 64; ++i) {
        std::bitset<64> mod{MaskShift(number, i)};
        std::cout << "bits " << i << ": " << std::endl;
        std::cout << orig << std::endl;
        std::cout << mod << std::endl;
        std::cout << std::endl;
    }*/

    std::cout << std::numeric_limits<uint64_t>::max() << std::endl;
    std::cout << std::numeric_limits<double>::max()  << std::endl;
}

