#include <fstream>
#include <iostream>
#include <string>

#include "count_collisions.h"
#include "generators.h"
#include "hashes.h"
#include "log_duration.h"

constexpr int GIGABYTE = 1'073'741'824;

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

void RunTests(int word_count, int min_word_size, int max_word_size) {
    std::random_device rd;
    std::mt19937 generator{rd()};

    std::cout << "Build worlds START" << std::endl;
    const std::vector<std::string> words = GenerateWords(generator, word_count, min_word_size, max_word_size);
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

int main(int argc, const char** argv) {
    if (argc != 4) {
        cerr << "Usage: "sv << argv[0] << " <word_count> <min_word_size> <max_word_size>"sv << endl;
        return 1;
    }

    int word_count = std::stoi(argv[1]);
    int min_word_size = std::stoi(argv[2]);
    int max_word_size = std::stoi(argv[3]);

    assert(word_count * max_word_size < GIGABYTE);
    RunTests(word_count, min_word_size, max_word_size);
}

