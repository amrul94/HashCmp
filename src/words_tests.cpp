#include "words_tests.h"

#include <pcg_random.hpp>

#include "hashes.h"

#define RUN_TEST_WITH_RANDOM_WORDS_IMPL(BITS, COUNTS, LENGTH, MOD, TB)                          \
    auto gen##BITS = base_gen;                                                                  \
    const WordsParameters wp##BITS {BITS, COUNTS, LENGTH, MOD, TB};                             \
    const auto hashes##BITS = hfl::Build##BITS##bitsHashes(hfl::BuildFlag::RandomWordsTest);    \
    TestWithWords(gen##BITS, hashes##BITS, wp##BITS);


void RunTestsWithGeneratedBlocks(uint32_t words_length) {
    //std::random_device rd;
    pcg64 base_gen{};


    // Возможно здесь стоит сделать цикл
    {/*
        auto mod16 = static_cast<uint64_t>(pow(2, 16));
        auto word_counts = static_cast<uint64_t>(pow(2, 16));
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(16, word_counts, words_length, mod16, 16);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(32, word_counts, words_length, mod16, 16);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(64, word_counts, words_length, mod16, 16);
    */}
    {
        auto mod24 = static_cast<uint64_t>(pow(2, 24));
        auto word_counts = static_cast<uint64_t>(pow(2, 24));
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(24, word_counts, words_length, mod24, 24);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(32, word_counts, words_length, mod24, 24);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(64, word_counts, words_length, mod24, 24);
    }
}
