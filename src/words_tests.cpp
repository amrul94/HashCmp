#include "words_tests.h"

#include <thread>

#include "hashes.h"

#define RUN_TEST_WITH_RANDOM_WORDS_IMPL(BITS, COUNTS, LENGTH, MOD)                                                      \
    auto gen##BITS = base_gen;                                                                                          \
    const WordsParameters wp##BITS {BITS, COUNTS, LENGTH, MOD};                                                         \
    const auto hashes##BITS = hfl::Build##BITS##bitsHashes();                                                           \
    std::jthread jt##BITS {TestWithWords<decltype(base_gen), decltype(hashes##BITS)>, gen##BITS, hashes##BITS, wp##BITS};


WordsParameters::WordsParameters(uint16_t hash_bits, uint64_t word_counts, uint32_t length, uint64_t mod)
    : hash_bits(hash_bits)
    , words_count(word_counts)
    , words_length(length)
    , mod(mod) {
}

void RunTestWithRandomWords() {
    std::random_device rd;
    std::mt19937_64 base_gen{rd()};
    auto word_counts = static_cast<uint64_t>(pow(2, 16));

    // Возможно здесь стоит сделать цикл
    {
        auto mod16 = static_cast<uint64_t>(pow(2, 16));
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(16, word_counts, FOUR_KILOBYTES, mod16);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(32, word_counts, FOUR_KILOBYTES, mod16);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(64, word_counts, FOUR_KILOBYTES, mod16);
    }
    {
        auto mod24 = static_cast<uint64_t>(pow(2, 24));
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(32, word_counts, FOUR_KILOBYTES, mod24);
        RUN_TEST_WITH_RANDOM_WORDS_IMPL(64, word_counts, FOUR_KILOBYTES, mod24);
    }
}
