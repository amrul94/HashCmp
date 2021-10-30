#include "words_tests.h"

#include <pcg_random.hpp>

#include "hashes.h"

#define RUN_TEST_WITH_RANDOM_WORDS_IMPL(HASH_BITS, TEST_BITS, COUNTS, LENGTH, MODE)             \
    auto gen##HASH_BITS = base_gen;                                                             \
    const WordsParameters wp##HASH_BITS {HASH_BITS, TEST_BITS, COUNTS, LENGTH, MODE};           \
    const auto hashes##HASH_BITS = hfl::Build##HASH_BITS##bitsHashes(hfl::BuildFlag::MASK);     \
    TestWithWords(gen##HASH_BITS, hashes##HASH_BITS, wp##HASH_BITS);


void RunTestsWithGeneratedBlocks(uint32_t words_length) {
    pcg64 base_gen{};

    {
        auto word_counts = static_cast<uint64_t>(pow(2, 16));
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(16, 16, word_counts, words_length, ModeFlag::NORMAL);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(32, 16, word_counts, words_length, ModeFlag::MASK);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(64, 16, word_counts, words_length, ModeFlag::MASK);
    }
    {
        auto word_counts = static_cast<uint64_t>(pow(2, 24));
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(24, 24, word_counts, words_length, ModeFlag::NORMAL);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(32, 24, word_counts, words_length, ModeFlag::MASK);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(64, 24, word_counts, words_length, ModeFlag::MASK);
    }
}
