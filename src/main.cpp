#include "check_distribution.h"
#include "words_tests.h"

#include <pcg_random.hpp>

#include <random>

void TempTests() {
    pcg64 gen{};
    pcg64 gen2{gen};
    std::uniform_int_distribution<uint64_t> ud{};
    for (int i = 0; i < 100; ++i) {
        std::cout << ud(gen) << std::endl;
    }

    std::cout << std::endl;
    std::uniform_int_distribution<uint64_t> ud2{};
    for (int i = 0; i < 100; ++i) {
        std::cout << gen2() << std::endl;
    }

}

void RunTests() {
    int test_number = 2;

    while(true) {
        switch (test_number) {
            case 1:
                RunCheckDist();
                return;
            case 2:
                //RunTestsWithGeneratedBlocks(16);
                RunTestsWithGeneratedBlocks(FOUR_KILOBYTES);
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

