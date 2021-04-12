//
// Created by amrulla on 17.03.2021.
//

#include <fstream>
#include "hashes.h"
#include "log_duration.h"
#include "random_world_tests.h"

constexpr int WORD_COUNT = 1'000;
constexpr int MIN_WORD_SIZE = hashes::WORD_SIZE;
constexpr int MAX_WORD_SIZE = hashes::WORD_SIZE;

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

[[maybe_unused]] void RunAllRandomTests() {
    std::random_device rd;
    std::mt19937 generator{rd()};
    std::ofstream file_worlds("data/random_words");

    std::cout << "Build worlds START" << std::endl;
    const std::vector<std::string> words = GenerateWords(generator, WORD_COUNT, MIN_WORD_SIZE, MAX_WORD_SIZE);
    for (const auto& word : words) {
        file_worlds << word << " ";
    }
    std::cout << "Build worlds END\n\n";

    std::cout << "Test with 32 bits hashes START" << std::endl;
    const auto hashes_32_bits = hashes::Build32bitsHashes();
    std::ofstream file_report_32_hashes("reports/random_words_report_with_32_bits_hashes.txt");
    RunConcreteRandomTest(hashes_32_bits, words, file_report_32_hashes);
    std::cout << "Test with 32 bits hashes END\n" << std::endl;

    std::cout << "Test with 64 bits hashes START" << std::endl;
    const auto hashes_64_bits = hashes::Build64bitsHashes();
    std::ofstream file_report_64_hashes("reports/random_words_report_with_64_bits_hashes.txt");
    RunConcreteRandomTest(hashes_64_bits, words, file_report_64_hashes);
    std::cout << "Test with 64 bits hashes END\n" << std::endl;
}