#include "check_distribution.h"
#include "words_tests.h"

#include <pcg_random.hpp>

void TempTests() {
    pcg64 gen{};
    pcg64 gen2{gen};
/*
    uint64_t count = 8'589'934'592;
    while (true) {
        try {
            vector<uint64_t> vec(count, 0);
            std::cout << "Allocate " << count  << ": OK" << std::endl;
            std::generate(std::execution::par, vec.begin(),  vec.end(), gen);
            std::cout << "Generate " << vec.size()  << ": OK" << std::endl;
            std::sort(std::execution::par, vec.begin(), vec.end());
            auto last = std::unique(std::execution::par, vec.begin(), vec.end());
            vec.erase(last, vec.end());
            std::cout << "Unique: " << vec.size() << std::endl;
            break;
        }
        catch(std::exception& e) {
            std::cerr << "Allocate " << count << ": " << e.what() << std::endl;
            count /= 2;
        }
    }
*/
    std::cout << 0xFFFFFFFF << std::endl;
    std::cout << UINT32_MAX << std::endl;

}

void RunTests() {
    std::vector test_numbers{3};

    for (int test_number : test_numbers) {
        switch (test_number) {
            case 1:
                std::cout << "start CheckDist" << std::endl;
                RunCheckDist();
                std::cout << "end CheckDist" << std::endl;
                break;
            case 2:
                std::cout << "start WordsTest 1" << std::endl;
                RunTestsWithGeneratedBlocks(16);
                std::cout << "start WordsTest 2" << std::endl;
                RunTestsWithGeneratedBlocks(FOUR_KILOBYTES);
                std::cout << "end WordsTests" << std::endl;
                break;
            case 3:

            default:
                TempTests();
                break;
        }
    }

}

int main() {
    RunTests();
}

