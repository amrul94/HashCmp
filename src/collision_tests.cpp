#include "collision_tests.h"

#include <pcg_random.hpp>

#include "hashes.h"

#define RUN_TEST_WITH_ENG_WORDS_IMPL(WORDS, BITS, ROOT)                         \
    const auto hashes##BITS = hfl::Build##BITS##bitsHashes();                       \
    const WordsParameters wp##BITS{BITS, BITS, WORDS.size(), 0, TestFlag::NORMAL};  \
    TestWithEnglishWords(hashes##BITS, WORDS, wp##BITS, ROOT)                   \


#define RUN_TEST_WITH_GEN_WORDS_IMPL(HASH_BITS, TEST_BITS, COUNTS, LENGTH, MODE, ROOT)                \
    auto gen##HASH_BITS = base_gen;                                                             \
    const WordsParameters wp##HASH_BITS {HASH_BITS, TEST_BITS, COUNTS, LENGTH, MODE};           \
    const auto hashes##HASH_BITS = hfl::Build##HASH_BITS##bitsHashes(hfl::BuildFlag::MASK);     \
    TestWithGeneratedBlocks(gen##HASH_BITS, hashes##HASH_BITS, wp##HASH_BITS, ROOT)


uint64_t CountCollisions(const std::vector<uint32_t>& hashes) {
    uint64_t collisions = 0;
    for (const auto& count : hashes) {
        if (count > 1) {
            collisions += (count - 1);
        }
    }
    return collisions;
}

uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hashes) {
    uint64_t collisions = 0;
    for (const auto& [hash, count] : hashes) {
        if (count > 1) {
            collisions += (count - 1);
        }
    }
    return collisions;
}

std::vector<std::string> ParseWords(const std::filesystem::path& file_name) {
    std::ifstream input(file_name);
    assert(input);

    std::vector<std::string> words;
    while (input) {
        std::string str;
        input >> str;
        words.push_back(std::move(str));
    }
    return words;
}

void RunTestWithEnglishWords(ReportsRoot& reports_root) {
    const std::filesystem::path& file_name = "data/english_words.txt";
    const std::vector<std::string> words = ParseWords(file_name);

    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 16, reports_root);
    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 24, reports_root);
    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 32, reports_root);
    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 48, reports_root);
    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 64, reports_root);
}

std::string UintToString(uint64_t src, uint64_t size) {
    std::string dest;
    dest.resize(size);
    memcpy(dest.data(), &src, size);
    return dest;
}

void RunTestWithGeneratedBlocks(uint32_t words_length, ReportsRoot& reports_root) {
    pcg64 base_gen{};

    {
        auto word_counts = static_cast<uint64_t>(pow(2, 16));
        RUN_TEST_WITH_GEN_WORDS_IMPL(16, 16, word_counts, words_length, TestFlag::NORMAL, reports_root);
        RUN_TEST_WITH_GEN_WORDS_IMPL(32, 16, word_counts, words_length, TestFlag::MASK, reports_root);
        RUN_TEST_WITH_GEN_WORDS_IMPL(64, 16, word_counts, words_length, TestFlag::MASK, reports_root);
    }
    {
        auto word_counts = static_cast<uint64_t>(pow(2, 24));
        RUN_TEST_WITH_GEN_WORDS_IMPL(24, 24, word_counts, words_length, TestFlag::NORMAL, reports_root);
        RUN_TEST_WITH_GEN_WORDS_IMPL(32, 24, word_counts, words_length, TestFlag::MASK, reports_root);
        RUN_TEST_WITH_GEN_WORDS_IMPL(64, 24, word_counts, words_length, TestFlag::MASK, reports_root);
    }
}