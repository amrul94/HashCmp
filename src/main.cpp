#include "distribution_tests.h"
#include "collision_tests.h"

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
                std::cout << "start DistributionTest" << std::endl;
                RunDistributionTests();
                std::cout << "end DistributionTest" << std::endl;
                break;
            case 2:
                std::cout << "start WordsTest 1" << std::endl;
                RunTestWithGeneratedBlocks(16);
                std::cout << "start WordsTest 2" << std::endl;
                RunTestWithGeneratedBlocks(FOUR_KILOBYTES);
                std::cout << "end WordsTests 1 and 2" << std::endl;
                break;
            case 3:
                std::cout << "start WordsTest 3" << std::endl;
                RunTestWithEnglishWords();
                std::cout << "end WordsTests 3" << std::endl;
                break;
            default:
                TempTests();
                break;
        }
    }

}

int main() {
    RunTests();
}

