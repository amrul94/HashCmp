#ifndef THESIS_WORK_WORDS_TESTS_H
#define THESIS_WORK_WORDS_TESTS_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <boost/json.hpp>

#include "test_parameters.h"
#include "log_duration.h"

constexpr int KILOBYTE = 1024;
constexpr int FOUR_KILOBYTES = KILOBYTE * 4;

uint64_t CountCollisions(const std::vector<uint32_t>& hashes);
uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hashes);

std::vector<std::string> ParseWords(const std::filesystem::path& file_name);

// в будущем объединю эту функцию с WriteToHash из hash_wrappers
std::string UintToString(uint64_t src, uint64_t size);

template<typename Generator>
std::string GenerateRandomDataBlock(Generator& generator, uint64_t length) {
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

template<typename Generator, typename HashStruct>
auto HashTestWithGenBlocks(Generator& generator, const HashStruct& hs, const WordsParameters& wp) {
    LOG_DURATION_STREAM(hs.hash_name, std::cout);

    std::vector<uint32_t> hashes(wp.key_count, 0);

    boost::json::object counters;
    auto word_counts = static_cast<uint64_t>(pow(2, wp.test_bits / 2));
    uint64_t i = 0;
    for (; word_counts <= wp.key_count; word_counts *= 2) {
        for (; i < word_counts; ++i) {
            std::string str = GenerateRandomDataBlock(generator, wp.words_length);
            const uint64_t hash = hs.hash_function(str);
            const uint64_t modify = ModifyHash(wp, hash);
            ++hashes[modify];
        }
        std::string index = std::to_string(word_counts);
        counters[index] = CountCollisions(hashes);
    }

    auto hash_name = static_cast<boost::json::string>(hs.hash_name);
    if (wp.test_bits > 24) {
        hash_name += " (mask " + std::to_string(wp.test_bits) + " bits)";
    }
    boost::json::object result;
    result[hash_name] = counters;
    return std::pair{std::move(hash_name), std::move(counters)};
}

template <typename HashStruct>
uint64_t HashTestWithEngWords(const HashStruct& hs, const std::vector<std::string>& words,
                          const WordsParameters& wp, std::ostream& log) {
    LOG_DURATION_STREAM(hs.hash_name, log);
    std::map<uint64_t, uint64_t> hashes;
    for (const std::string& word : words) {
        const uint64_t hash = hs.hash_function(word);
        const uint64_t modify = ModifyHash(wp, hash);
        ++hashes[modify];
    }
    return CountCollisions(hashes);
}

template<typename Generator, typename HashStructs>
void TestWithGeneratedBlocks(Generator gen, const HashStructs& hash_vec, const WordsParameters& wp) {
    using namespace std::literals;

    std::cout << "start " << wp.hash_bits << " bits" << std::endl;

    std::string file_name = "reports/gen_words/"s + std::to_string(wp.hash_bits) + " bits (" + TestFlagToString(wp.mode)
                            + " "s + std::to_string(wp.test_bits) + " bits) " + CurrentTime() + ".json"s;


    std::ofstream out(file_name);
    assert(out);
    boost::json::object obj;
    obj["Test name"] = "Test With Random Words";
    obj["Mode"] = TestFlagToString(wp.mode);
    obj["Bits"] = wp.hash_bits;
    obj["Mask"] = wp.test_bits;
    obj["Number of keys"] = wp.key_count;

    boost::json::object collisions;
    std::mutex local_mutex;
    auto lambda = [&local_mutex, &wp, &collisions, &gen, &hash_vec] (size_t index) {
        const auto& current_hash = hash_vec[index];
        Generator c_gen = gen;
        auto [hash_name, counters] = HashTestWithGenBlocks(c_gen, current_hash, wp);
        std::scoped_lock guard{local_mutex};
        collisions[hash_name] = counters;
    };

    uint64_t chunks = hash_vec.size() / 4;
    for (uint64_t i = 0; i < chunks; ++i) {
        std::jthread jt1{lambda, i * 4};
        std::jthread jt2{lambda, i * 4 + 1};
        std::jthread jt3{lambda, i * 4 + 2};
        std::jthread jt4{lambda, i * 4 + 3};
    }
    
    for (size_t i = chunks * 4; i < hash_vec.size(); ++i) {
        lambda(i);
    }
    obj["Collisions"] = collisions;
    out << obj;

    std::cout << "end " << wp.hash_bits << " bits" << std::endl << std::endl;

}

template <typename HashStruct>
void TestWithEnglishWords(const std::vector<HashStruct>& hashes, const std::vector<std::string>& words,
                          const WordsParameters& wp, std::ostream& log = std::cout) {
    log << "Test with english words (" << wp.hash_bits << " bits) START" << std::endl;

    std::string file_name = "reports/eng_words/" + std::to_string(wp.hash_bits) + " bits " + CurrentTime() + ".json";


    std::ofstream out(file_name);
    assert(out);
    boost::json::object obj;
    obj["Test name"] = "Test With English Words";
    obj["Bits"] = wp.hash_bits;
    obj["Number of words"] = words.size();

    boost::json::object collisions;
    for (const auto& hs : hashes) {
        collisions[hs.hash_name] = HashTestWithEngWords(hs, words, wp, log);
    }
    obj["Collisions"] = collisions;
    out << obj;

    log << "Test with english words (" << wp.hash_bits << " bits) END" << std::endl;
}

void RunTestWithGeneratedBlocks(uint32_t words_length);

void RunTestWithEnglishWords();

#endif //THESIS_WORK_WORDS_TESTS_H
