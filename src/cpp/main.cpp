#include "generated_tests.h"
#include "distribution_tests.h"
#include "english_tests.h"
#include "log_duration.h"
#include "speed_tests.h"
#include "avalanche_tests.h"
#include "images_tests.h"

#include <pcg_random.hpp>
#include <algorithm>

#include "hashes.h"
#include "my_assert.h"

void TempTests(tests::ReportsRoot& report_root) {
    uint64_t counter = 0;

    for (int i = 0; i < 10; ++i) {
        counter += true;
        std::cout << counter << std::endl;
        counter += false;
        std::cout << counter << std::endl;
    }

}

void HashIsCorrectTest() {
    auto lambda = [](const auto& hasher, const std::vector<uint64_t>& numbers) {
        for (uint64_t number : numbers) {
            ASSERT_EQUAL_HINT(hasher.hash(number), hasher.hash(number), hasher.name + " is not correct");
        }
        std::cout << boost::format("\t\t%1% is correct\n") % hasher.name;
    };

    uint64_t count = 100000;
    pcg64 rng;
    std::vector<uint64_t> numbers;
    numbers.reserve(count);
    for (int i = 0; i < count; ++i) {
        numbers.emplace_back(rng());
    }

    const auto hashes16 = hfl::Build16bitsHashes();
    std::cout << boost::format("\t16 bits hashes (count = %1%):\n") % hashes16.size();
    for (const auto& hasher : hashes16) {
        lambda(hasher, numbers);
    }
    std::cout << std::endl;

    const auto hashes24 = hfl::Build24bitsHashes();
    std::cout << boost::format("\t24 bits hashes (count = %1%):\n") % hashes24.size();
    for (const auto& hasher : hashes24) {
        lambda(hasher, numbers);
    }
    std::cout << std::endl;

    const auto hashes32 = hfl::Build32bitsHashes();
    std::cout << boost::format("\t32 bits hashes (count = %1%):\n") % hashes32.size();
    for (const auto& hasher : hashes32) {
        lambda(hasher, numbers);
    }
    std::cout << std::endl;

    const auto hashes48 = hfl::Build48bitsHashes();
    std::cout << boost::format("\t48 bits hashes (count = %1%):\n") % hashes48.size();
    for (const auto& hasher : hashes48) {
        lambda(hasher, numbers);
    }
    std::cout << std::endl;

    const auto hashes64 = hfl::Build64bitsHashes();
    std::cout << boost::format("\t64 bits hashes (count = %1%):\n") % hashes64.size();
    for (const auto& hasher : hashes64) {
        lambda(hasher, numbers);
    }
    std::cout << std::endl;
}


void RunTests(const std::vector<int>& test_numbers, tests::ReportsRoot& reports_root) {
    LOG_DURATION_STREAM("FULL TIME", reports_root.logger);


    for (int test_number : test_numbers) {
        switch (test_number) {
            case 0:
                std::cout << "=== CHECK HASHES ===\n\n";
                HashIsCorrectTest();
                std::cout << "=== CHECK HASHES ===\n\n\n";
                break;
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
            case 7:
                reports_root.logger << "=== START IMAGES TEST ===\n\n";
                tests::RunImagesTests(reports_root);
                reports_root.logger << "=== END IMAGES TEST ===\n\n\n";
                break;
            default:
                TempTests(reports_root);
                //std::filesystem::remove_all(reports_root.root_path);
                //std::cout << reports_root.root_path << " removed\n";
                break;
        }
    }
}

tests::ReportsRoot CreateReportsRoot() {
    const auto current_path = std::filesystem::current_path();
    const std::filesystem::path reports_dir {"reports"};
    const auto reports_path = current_path / reports_dir / CurrentTime() / "cpp";
    std::filesystem::create_directories(reports_path);
    return tests::ReportsRoot{reports_path};
}

int main() {
    const std::vector test_numbers{2};
    tests::ReportsRoot reports_root = CreateReportsRoot();
    RunTests(test_numbers, reports_root);
}

