#ifndef THESIS_WORK_WORDS_TESTS_H
#define THESIS_WORK_WORDS_TESTS_H

#include <fstream>
#include <iostream>
#include <vector>

#include "count_collisions.h"
#include "generators.h"
#include "hashes.h"
#include "helper_structs.h"
#include "log_duration.h"

constexpr int GIGABYTE = 1'073'741'824;
constexpr int KILOBYTE = 1024;
constexpr int FOUR_KILOBYTES = KILOBYTE * 4;
constexpr int EIGHT_KILOBYTES = KILOBYTE * 8;

template<typename Generator, typename HashStruct>
void WordsTestForHash(Generator& generator, const HashStruct& hs, const WordsParameters& wp, std::ostream& out) {
    LOG_DURATION_STREAM(hs.hash_name, std::cout);

    std::vector<uint32_t> hashes(wp.mod, 0);
    for (uint64_t i = 0; i < wp.key_counts; ++i) {
        std::string str = GenerateRandomWord(generator, wp.words_length);
        auto hash = hs.hash_function(str) % wp.mod;
        ++hashes[hash];
    }

    out << hs.hash_name << "\t" << CountCollisions(hashes) << std::endl;
}

template<typename Generator, typename HashStructs>
void TestWithWords(Generator gen, const HashStructs& hash_vec, const WordsParameters& wp) {
    using namespace std::literals;

    std::cout << "start "  << wp.hash_bits << " bits" << std::endl;
    std::string file_name = "reports/Words Test/"s + std::to_string(wp.hash_bits) + " bits mod "s +
            std::to_string(wp.mod) + ' ' + CurrentTime() + ".txt"s;

    std::ofstream out(file_name);
    for (const auto& current_hash : hash_vec) {
        Generator c_gen = gen;
        WordsTestForHash(c_gen, current_hash, wp, out);
    }
    std::cout << "end " << wp.hash_bits << " bits" << std::endl << std::endl;
}

void RunTestWithRandomWords();

#endif //THESIS_WORK_WORDS_TESTS_H
