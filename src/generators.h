#ifndef THESIS_WORK_GENERATORS_H
#define THESIS_WORK_GENERATORS_H

#include <array>
#include <random>
#include <string>
#include <vector>



#include <iostream>

std::string UintToString(uint64_t src, uint64_t size);

template<typename Generator>
std::string GenerateRandomWord(Generator& generator, uint64_t length) {
    std::string word;
    word.reserve(length);
    uint64_t max_unt64 = std::numeric_limits<uint64_t>::max();
    std::uniform_int_distribution<uint64_t> ud(0, max_unt64);
    uint64_t chunks = length / sizeof(uint64_t);
    for (uint64_t i = 0; i < chunks; ++i) {
        word += UintToString(ud(generator), sizeof(uint64_t));
    }
    uint64_t diff = length - chunks * sizeof(uint64_t);
    word += UintToString(ud(generator), diff);
    return word;
}

// Старая версия
std::string GenerateRandomWord(std::mt19937& generator, uint32_t min_length, uint32_t max_length);

std::vector<std::string> GenerateRandomWords(std::mt19937& generator, int word_count, int min_length, int max_length);

std::vector<std::string> GenerateSimilarWord(char ch, int length);

std::vector<std::string> PJWCrash(char ch, int length);

std::vector<std::string> CrashDJB2(int word_count, uint64_t crash_word);

#endif //THESIS_WORK_GENERATORS_H
