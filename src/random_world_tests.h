//
// Created by amrulla on 17.03.2021.
//

#ifndef THESIS_WORK_RANDOM_WORLD_TESTS_H
#define THESIS_WORK_RANDOM_WORLD_TESTS_H

#include <functional>
#include <random>
#include <string>

#include "tests.h"


std::string GenerateWord(std::mt19937& generator, int min_length, int max_length);

std::vector<std::string> GenerateWords(std::mt19937& generator, int word_count, int min_length, int max_length);

template <typename HashStruct>
void TestWithRandomWords(const HashStruct& hash_struct, const std::vector<std::string>& random_words,
                         std::ostream& out) {
    out << std::endl;
    LOG_DURATION_STREAM(hash_struct.hash_name, out);
    std::unordered_map<size_t, size_t> hashes;
    for (const std::string& word : random_words) {
        ++hashes[hash_struct.hash_function(word)];
    }

    out << hash_struct.hash_name << " collisions = " << CountCollisions(hashes) << std::endl;
}

template <typename HashStruct>
void RunConcreteRandomTest(const std::vector<HashStruct>& hashes, const std::vector<std::string>& words,
                           std::ostream& out) {
    out << "words: " << words.size() << std::endl;
    for (const auto& hash_struct : hashes) {
        LOG_DURATION_STREAM(hash_struct.hash_name, std::cout);
        TestWithRandomWords(hash_struct, words, out);
    }
}

[[maybe_unused]] void RunAllRandomTests();

#endif //THESIS_WORK_RANDOM_WORLD_TESTS_H
