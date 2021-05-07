#include "generators.h"

#include <cassert>
#include <cstring>

std::string GenerateRandomWord(std::mt19937& generator, int min_length, int max_length) {
    const int length = std::uniform_int_distribution(min_length, max_length)(generator);
    std::string word;
    word.reserve(length);
    for (int i = 0; i < length; ++i) {
        word.push_back(std::uniform_int_distribution('a', 'z')(generator));
    }
    return word;
}

std::vector<std::string> GenerateRandomWords(std::mt19937& generator, int word_count, int min_length, int max_length) {
    std::vector<std::string> words;
    words.reserve(word_count);
    for (int i = 0; i < word_count; ++i) {
        words.push_back(GenerateRandomWord(generator, min_length, max_length));
    }
    sort(words.begin(), words.end());
    words.erase(unique(words.begin(), words.end()), words.end());
    return words;
}

std::vector<std::string> GenerateSimilarWord(char ch, int length) {
    std::vector<std::string> result;
    result.reserve(256);
    std::string str(length, ch);
    char c = std::numeric_limits<char>::min();
    for (int i = 0; c < std::numeric_limits<char>::max(); ++i, ++c) {
        result.push_back(str + c);
    }
    return result;
}

std::vector<std::string> PJWCrash(char ch, int length) {
    std::vector<std::string> result = GenerateSimilarWord(ch, length);
    uint32_t max_world = std::numeric_limits<uint32_t>::max();
    uint64_t crash_num = 10'000'000'000;
    std::string crash_str(reinterpret_cast<char*>(&crash_num), 64);
    for (std::string& str : result) {
        str += crash_str;
    }
    return result;
}

std::vector<std::string> CrashDJB2(int word_count, uint64_t crash_word) {
    std::vector<std::string> words;
    uint64_t number = 0;
    for (int i = 0; i < word_count; ++i, number += crash_word) {
        words.emplace_back(reinterpret_cast<char*>(&number), 8);
    }
    return words;
}