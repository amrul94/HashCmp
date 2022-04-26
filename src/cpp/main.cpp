#include "generated_tests.h"
#include "distribution_tests.h"
#include "english_tests.h"
#include "timers.h"
#include "speed_tests.h"
#include "avalanche_tests.h"
#include "images_tests.h"

#include <pcg_random.hpp>
#include <algorithm>

#include "hash_wrappers.h"
#include "hashes.h"
#include "my_assert.h"
#include "output.h"

void print(std::integral auto i) {
    std::cout << "Integral: " << i << '\n';
}

void print(auto x) {
    std::cout << "Non-integral: " << x << '\n';
}

void TempTests(tests::out::Logger& logger) {
    std::cout << std::boolalpha;
    print(true ); static_assert( std::integral<bool> );
    print( 'o' ); static_assert( std::integral<char> );
    print( 007 ); static_assert( std::integral<int> );
    print( hfl::uint24_t{365} ); static_assert( !std::integral<hfl::uint24_t> );
    print(hfl::uint48_t{365}); static_assert( !std::integral<hfl::uint48_t> );
    std::cout << boost::is_integral<hfl::uint48_t>::value << std::endl;
}

void HashIsCorrectTest(tests::out::Logger& logger) {
    tests::out::StartAndEndLogTest start_and_end_log{logger, "CHECK HASHES"};

    auto lambda = [](const auto& hasher, const std::vector<uint64_t>& numbers) {
        for (uint64_t number : numbers) {
            ASSERT_EQUAL_HINT(hasher(number), hasher(number), hasher.GetName() + " is not correct");
        }
        std::cout << boost::format("\t\t%1% is correct\n") % hasher.GetName();
    };

    const uint64_t count = 100000;
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


void RunTests(const std::vector<int>& test_numbers, tests::out::Logger& logger) {
    tests::out::LogDuration log_duration("FULL TIME", logger);

    for (int test_number : test_numbers) {
        switch (test_number) {
            case 0:
                HashIsCorrectTest(logger);
                break;
            case 1:
                tests::RunDistributionTests(logger);
                break;
            case 2:
                tests::RunTestWithGeneratedBlocks(16, logger);
                break;
            case 3:
                tests::RunTestWithGeneratedBlocks(FOUR_KILOBYTES, logger);
                break;
            case 4:
                tests::RunTestWithEnglishWords(logger);
                break;
            case 5:
                tests::RunSpeedTests(2'000'000, FOUR_KILOBYTES, logger);
                break;
            case 6:
                tests::RunAvalancheTests(logger);
                break;
            case 7:
                tests::RunImagesTests(logger);
                break;
            default:
                TempTests(logger);
                //std::filesystem::remove_all(logger.GetLogDirPath());
                //std::cout << logger.GetLogDirPath() << " removed\n";
                break;
        }
    }
}



int main() {
    const std::vector test_numbers{6};
    auto Logger = tests::out::CreateLogger();
    RunTests(test_numbers, Logger);
}

