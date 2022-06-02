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

constexpr uint16_t LENGTH_OF_LITTLE_BLOCK = 16;
constexpr uint16_t LENGTH_OF_BIG_BLOCK = 4048;
constexpr uint64_t NUM_SPEED_TEST_WORDS = 2'000'000;

// Проверяет корректность хеша. Хеширование двух одинаковых чисел должно дать два одинаковых хеша
template<hfl::UnsignedIntegral UintT>
void CheckHash(const hfl::Hash<UintT>& hash, const std::vector<uint64_t>& numbers, tests::out::Logger& logger) {
    for (uint64_t number : numbers) {
        std::string hint = hash.GetName() + " is not correct";
        ASSERT_EQUAL_HINT(hash(number), hash(number), hint);

    }
    logger << boost::format("\t\t%1% is correct\n") % hash.GetName();
}

// Проверяет корректность работы всех хешей одной битности
template<hfl::UnsignedIntegral UintT>
void CheckHashes(const std::vector<uint64_t>& numbers, tests::out::Logger& logger) {
    const auto hashes = hfl::BuildHashes<UintT>();
    std::cout << boost::format("\t%1% bits hashes (size = %2%):\n") % std::numeric_limits<UintT>::digits
        % hashes.size();
    for (const auto& hasher : hashes) {
        CheckHash(hasher, numbers, logger);
    }
    logger << std::endl;
}

// Запускает проверку корректности работы всех хешей
void RunCheckHashes(tests::out::Logger& logger) {
    tests::out::StartAndEndLogTest start_and_end_log{logger, "CHECK HASHES"};

    const uint64_t size = 100000;
    pcg64 rng;
    std::vector<uint64_t> numbers;
    numbers.reserve(size);
    for (int i = 0; i < size; ++i) {
        numbers.emplace_back(rng());
    }

    CheckHashes<uint16_t>(numbers, logger);
    CheckHashes<hfl::uint24_t>(numbers, logger);
    CheckHashes<uint32_t>(numbers, logger);
    CheckHashes<hfl::uint48_t>(numbers, logger);
    CheckHashes<uint64_t>(numbers, logger);
}

enum class TestType {
    AVALANCHE_TESTS = 1,
    DISTRIBUTION_TESTS = 2,
    TESTS_WITH_LITTLE_GENERATED_BLOCKS = 3,
    TESTS_WITH_BIG_GENERATED_BLOCKS = 4,
    TESTS_WITH_ENGLISH_WORDS = 5,
    TESTS_WITH_IMAGES = 6,
    SPEED_TESTS = 7
};

// Запускает выбранные тесты
void RunTests(const std::vector<int>& test_numbers, tests::out::Logger& logger) {
    tests::out::LogDuration log_duration("FULL TIME", logger);

    // Перед запуском тестов проверяется корректность работы всех хешей
    RunCheckHashes(logger);

    // Последовательно запускаются все тесты
    for (int test_number : test_numbers) {
        TestType test_type{test_number};
        switch (test_type) {
            case TestType::AVALANCHE_TESTS:
                tests::RunAvalancheTests(logger);
                break;
            case TestType::DISTRIBUTION_TESTS:
                tests::RunDistributionTests(logger);
                break;
            case TestType::TESTS_WITH_LITTLE_GENERATED_BLOCKS:
                tests::RunTestWithGeneratedBlocks(LENGTH_OF_LITTLE_BLOCK, logger);
                break;
            case TestType::TESTS_WITH_BIG_GENERATED_BLOCKS:
                tests::RunTestWithGeneratedBlocks(LENGTH_OF_BIG_BLOCK, logger);
                break;
            case TestType::TESTS_WITH_ENGLISH_WORDS:
                tests::RunTestWithEnglishWords(logger);
                break;
            case TestType::TESTS_WITH_IMAGES:
                tests::RunImagesTests(logger);
                break;
            case TestType::SPEED_TESTS:
                tests::RunSpeedTests(NUM_SPEED_TEST_WORDS, LENGTH_OF_BIG_BLOCK, logger);
                break;
            default:
                break;
        }
    }
}

int main() {
    const std::vector test_numbers{7};
    auto logger = tests::out::CreateLogger();
    RunTests(test_numbers, logger);
}

