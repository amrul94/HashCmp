#include "generators.h"

std::string GenerateWord(std::mt19937& generator, int min_length, int max_length) {
    const int length = std::uniform_int_distribution(min_length, max_length)(generator);
    std::string word;
    word.reserve(length);
    for (int i = 0; i < length; ++i) {
        word.push_back(std::uniform_int_distribution('a', 'z')(generator));
    }
    return word;
}

std::vector<std::string> GenerateWords(std::mt19937& generator, int word_count, int min_length, int max_length) {
    std::vector<std::string> words;
    words.reserve(word_count);
    for (int i = 0; i < word_count; ++i) {
        words.push_back(GenerateWord(generator, min_length, max_length));
    }
    sort(words.begin(), words.end());
    words.erase(unique(words.begin(), words.end()), words.end());
    return words;
}