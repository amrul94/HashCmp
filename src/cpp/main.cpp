#include "generated_tests.h"
#include "distribution_tests.h"
#include "english_tests.h"
#include "log_duration.h"
#include "speed_tests.h"
#include "avalanche_tests.h"

#include <boost/format.hpp>
#include <boost/container/static_vector.hpp>

#include <pcg_random.hpp>
#include <random>
#include <algorithm>
#include <execution>
#include <set>
#include <unordered_set>

#include "hashes.h"

void LocalSpeedTest(tests::ReportsRoot& report_root) {
    for (int i = 0; i < 10; ++i) {
        std::cout << hfl::buz_hash_64(i) << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << hfl::buz_hash_64(i) << std::endl;
    }
}

void TempTests(tests::ReportsRoot& report_root) {
    std::vector<std::string> strings = {"key", "key2", "key3", "key4", "key5"};
    LocalSpeedTest(report_root);
}

void RunTests(const std::vector<int>& test_numbers, tests::ReportsRoot& reports_root) {
    LOG_DURATION_STREAM("FULL TIME", reports_root.logger);
    for (int test_number : test_numbers) {
        switch (test_number) {
            case 1:
                reports_root.logger << "=== START DISTRIBUTION TEST ===\n\n";
                tests::RunDistributionTests(reports_root);
                reports_root.logger << "=== END DISTRIBUTION TEST ===\n\n\n";
                break;
            case 2:
                reports_root.logger << "=== START GENERATED BLOCKS TEST (length = 16) ===\n\n";
                tests::RunTestWithGeneratedBlocks(16, reports_root);
                reports_root.logger << "=== END GENERATED BLOCKS TEST (length = 16) ===\n\n\n";
                break;
            case 3:
                reports_root.logger << "=== START GENERATED BLOCKS TEST (length = 4096) ===\n\n";
                tests::RunTestWithGeneratedBlocks(FOUR_KILOBYTES, reports_root);
                reports_root.logger << "=== END GENERATED BLOCKS TEST (length = 4096) ===\n\n\n";
                break;
            case 4:
                reports_root.logger << "=== START ENGLISH WORDS TEST ===\n\n";
                tests::RunTestWithEnglishWords(reports_root);
                reports_root.logger << "=== END ENGLISH WORDS TEST ===\n\n\n";
                break;
            case 5:
                reports_root.logger << "=== START SPEED TEST ===\n\n";
                tests::RunSpeedTests(2'000'000, FOUR_KILOBYTES, reports_root);
                reports_root.logger << "=== END SPEED TEST ===\n\n\n";
                break;
            case 6:
                reports_root.logger << "=== START AVALANCHE TEST ===\n\n";
                tests::RunAvalancheTests(reports_root);
                reports_root.logger << "=== END AVALANCHE TEST ===\n\n\n";
                break;
            default:
                TempTests(reports_root);
                break;
        }
    }
}

tests::ReportsRoot CreateReportsRoot() {
    const auto current_path = std::filesystem::current_path();
    const std::filesystem::path reports_dir{"reports"};
    const auto reports_path = current_path / reports_dir / CurrentTime() / "cpp";
    std::filesystem::create_directories(reports_path);
    return tests::ReportsRoot{reports_path};
}

int main() {
    const std::vector test_numbers{6};
    tests::ReportsRoot reports_root = CreateReportsRoot();
    RunTests(test_numbers, reports_root);
}

