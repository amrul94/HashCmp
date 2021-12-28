#include "generated_tests.h"
#include "distribution_tests.h"
#include "english_tests.h"
#include "log_duration.h"

#include <boost/format.hpp>
#include <boost/container/static_vector.hpp>

#include <pcg_random.hpp>
#include <random>
#include <algorithm>
#include <execution>
#include <set>

#include "hashes.h"

void TempTests() {
    const uint64_t num_count = 1'000;
    const uint64_t word_size = FOUR_KILOBYTES;

    {
        pcg64 rng;
        LOG_DURATION("New generator");
        uint64_t sum_hashes = 0;
        for (uint64_t number = 0; number < num_count; ++number) {
            std::string str = GenerateRandomDataBlock(rng, word_size);
            sum_hashes += hfl::city_hash_32(str);
        }
        std::cout << sum_hashes << std::endl;
    }

}



void RunTests(const std::vector<int>& test_numbers, ReportsRoot& reports_root) {

    for (int test_number : test_numbers) {
        switch (test_number) {
            case 1:
                reports_root.logger << "start DistributionTest\n\n";
                RunDistributionTests(reports_root);
                reports_root.logger << "end DistributionTest\n\n\n";
                break;
            case 2:
                reports_root.logger << "start generated blocks test (length = 16)\n\n";
                RunTestWithGeneratedBlocks(16, reports_root);
                reports_root.logger << "end generated blocks test (length = 16)\n\n\n";
                break;
            case 3:
                reports_root.logger << "start generated blocks test (length = 4096)\n\n";
                RunTestWithGeneratedBlocks(FOUR_KILOBYTES, reports_root);
                reports_root.logger << "end generated blocks test (length = 4096)\n\n\n";
            case 4:
                reports_root.logger << "start english words test\n\n";
                RunTestWithEnglishWords(reports_root);
                reports_root.logger << "end english words test\n\n\n";
                break;
            default:
                TempTests();
                break;
        }
    }
}

ReportsRoot CreateReportsRoot() {
    const auto current_path = std::filesystem::current_path();
    const std::filesystem::path reports_dir{"reports"};
    const auto reports_path = current_path / reports_dir / CurrentTime() / "cpp";
    std::filesystem::create_directories(reports_path);
    return ReportsRoot{reports_path};
}

int main() {
    const std::vector test_numbers{1, 2, 3, 4};
    ReportsRoot reports_root = CreateReportsRoot();
    RunTests(test_numbers, reports_root);
}

