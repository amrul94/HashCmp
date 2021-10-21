#include "check_distribution.h"
#include "words_tests.h"

#include <pcg_random.hpp>

#include <random>

void TempTests() {
    /*pcg64 gen{};
    std::uniform_int_distribution<uint64_t> ud{};
    for (int i = 0; i < 100; ++i) {
        std::cout << ud(gen) << std::endl;
    }*/

    std::string str = "string";
    uint64_t number = numeric_limits<uint64_t>::max();
    std::cout << hfl::fast_hash_16(number) << std::endl;
    std::cout << hfl::fast_hash_24(number) << std::endl;
    std::cout << hfl::fast_hash_32(number) << std::endl;
    std::cout << hfl::fast_hash_64(number) << std::endl;

}

void RunTests() {
    int test_number = 1;

    while(true) {
        switch (test_number) {
            case 1:
                RunCheckDist();
                return;
            case 2:
                RunTestWithRandomWords();
                return;
            default:
                TempTests();
                return;
        }
    }

}

int main() {
    RunTests();
}

