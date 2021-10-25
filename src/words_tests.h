#ifndef THESIS_WORK_WORDS_TESTS_H
#define THESIS_WORK_WORDS_TESTS_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>


#include <boost/json.hpp>

#include "count_collisions.h"
#include "generators.h"
#include "hashes.h"
#include "helper_structs.h"
#include "log_duration.h"

constexpr int GIGABYTE = 1'073'741'824;
constexpr int KILOBYTE = 1024;
constexpr int FOUR_KILOBYTES = KILOBYTE * 4;
constexpr int EIGHT_KILOBYTES = KILOBYTE * 8;

inline std::mutex wtm{};

template<typename Generator, typename HashStruct>
auto WordsTestForHash(Generator& generator, const HashStruct& hs, const WordsParameters& wp) {
    LOG_DURATION_STREAM(hs.hash_name, std::cout);

    std::vector<uint32_t> hashes(wp.mod, 0);


    boost::json::object counters;
    auto word_counts = static_cast<uint64_t>(pow(2, wp.test_bits / 2));
    uint64_t i = 0;
    for (; word_counts <= wp.key_counts; word_counts *= 2) {
        for (; i < word_counts; ++i) {
            std::string str = GenerateRandomWord(generator, wp.words_length);

            size_t hash = 0;
            if (wp.hash_bits == 24) {
                hash = (size_t)hs.hash_function(str);
            } else {
                hash = (size_t)hs.hash_function(str) % wp.mod;
            }
            ++hashes[hash];

        }
        std::string index = std::to_string(word_counts);
        counters[index] = CountCollisions(hashes);
        //std::cout << "word_counts " << word_counts << " -> " << counters[index] << std::endl;
    }

    auto hash_name = static_cast<boost::json::string>(hs.hash_name);
    if (wp.hash_bits > 24) {
        hash_name += " (mod " + std::to_string(wp.mod) + ")";
    }
    boost::json::object result;
    result[hash_name] = counters;
    return std::pair{std::move(hash_name), std::move(counters)};
}

template<typename Generator, typename HashStructs>
void TestWithWords(Generator gen, const HashStructs& hash_vec, const WordsParameters& wp) {
    using namespace std::literals;

    {
        std::scoped_lock guard{wtm};
        std::cout << "start "  << wp.hash_bits << " bits" << std::endl;
    }

    std::string file_name = "reports/Words Test/"s + std::to_string(wp.hash_bits) + " bits mod "s +
            std::to_string(wp.mod) + ' ' + CurrentTime() + ".json"s;

    std::ofstream out(file_name);
    boost::json::object obj;
    obj["Test name"] = "Test With Random Words";
    obj["Bits"] = wp.test_bits;

    //auto& collisions = obj["Collisions"];

    boost::json::object collisions;
    /*
    for (const auto& current_hash : hash_vec) {
        Generator c_gen = gen;
        auto [hash_name, counters] = WordsTestForHash(c_gen, current_hash, wp);
        collisions[hash_name] = counters;
    }*/
    std::mutex local_mutex;
    auto lambda = [&local_mutex, &wp, &collisions, &gen, &hash_vec] (size_t index) {
        const auto& current_hash = hash_vec[index];
        std::cout << "thread " << index << " start with " << current_hash.hash_name << endl;
        Generator c_gen = gen;
        auto [hash_name, counters] = WordsTestForHash(c_gen, current_hash, wp);
        std::scoped_lock guard{local_mutex};
        collisions[hash_name] = counters;
        std::cout << "thread " << index << " end with " << current_hash.hash_name << endl;
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

    {
        std::scoped_lock guard{wtm};
        std::cout << "end " << wp.hash_bits << " bits" << std::endl << std::endl;
    }

}

void RunTestsWithGeneratedBlocks(uint32_t words_length);

#endif //THESIS_WORK_WORDS_TESTS_H
