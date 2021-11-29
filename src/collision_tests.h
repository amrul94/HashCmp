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

template <typename HashStruct>
uint64_t HashTestWithEngWords(const HashStruct& hs, const std::vector<std::string>& words,
                              const WordsParameters& wp, ReportsRoot& reports_root) {
    LOG_DURATION_STREAM(hs.hash_name, reports_root.log);
    std::map<uint64_t, uint64_t> hashes;
    for (const std::string& word : words) {
        const uint64_t hash = hs.hash_function(word);
        const uint64_t modify = ModifyHash(wp, hash);
        ++hashes[modify];
    }
    return CountCollisions(hashes);
}

template <typename HashStruct>
void TestWithEnglishWords(const std::vector<HashStruct>& hashes, const std::vector<std::string>& words,
                          const WordsParameters& wp, ReportsRoot& reports_root) {
    reports_root.log << "Test with english words (" << wp.hash_bits << " bits) START" << std::endl;

    const std::filesystem::path report_test_dir = "English words tests";
    const auto report_test_path = reports_root.root_path / report_test_dir;
    std::filesystem::create_directories(report_test_path);

    const std::filesystem::path report_name = std::to_string(wp.hash_bits) + " bits.json";
    const std::filesystem::path out_path = report_test_path / report_name;
    std::ofstream out(out_path);
    assert(out);

    boost::json::object obj;
    obj["Test name"] = "Test With English Words";
    obj["Bits"] = wp.hash_bits;
    obj["Number of words"] = words.size();

    boost::json::object collisions;
    for (const auto& hs : hashes) {
        collisions[hs.hash_name] = HashTestWithEngWords(hs, words, wp, reports_root);
    }
    obj["Collisions"] = collisions;
    out << obj;

    reports_root.log << "Test with english words (" << wp.hash_bits << " bits) END" << std::endl;
}

void RunTestWithEnglishWords(ReportsRoot& reports_root);

// в будущем возможно объединю эту функцию с WriteToHash из hash_wrappers
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
    assert(word.size() == length);
    return word;
}

template<typename Generator, typename HashStruct>
auto HashTestWithGenBlocks(Generator& generator, const HashStruct& hs, const WordsParameters& wp,
                           ReportsRoot& reports_root) {
    LOG_DURATION_STREAM(hs.hash_name, reports_root.log);

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

// Возможно стоит поделить эту функции на части, так как она очень большая
template<typename Generator, typename HashStructs>
void TestWithGeneratedBlocks(Generator gen, const HashStructs& hash_vec, const WordsParameters& wp,
                             ReportsRoot& reports_root) {
    using namespace std::literals;

    reports_root.log << "start " << wp.hash_bits << " bits" << std::endl;

    const std::filesystem::path gen_tests_dir = "Generated blocks tests";
    const std::filesystem::path block_size_dir = std::to_string(wp.words_length);
    const auto block_size_path = reports_root.root_path / gen_tests_dir / block_size_dir;
    std::filesystem::create_directories(block_size_path);
    reports_root.log << "block_size_path: " << block_size_path << std::endl;

    const std::filesystem::path report_name = std::to_string(wp.hash_bits) + " bits (" + TestFlagToString(wp.mode)
                                              + " "s + std::to_string(wp.test_bits) + " bits).json"s;
    const std::filesystem::path report_path = block_size_path / report_name;
    reports_root.log << "report_path: " << report_path << std::endl;


    std::ofstream out(report_path);
    assert(out);
    boost::json::object obj;
    obj["Test name"] = "Test With Random Words";
    obj["Mode"] = TestFlagToString(wp.mode);
    obj["Bits"] = wp.hash_bits;
    obj["Mask"] = wp.test_bits;
    obj["Number of keys"] = wp.key_count;

    boost::json::object collisions;
    std::mutex local_mutex;
    auto lambda = [&] (size_t index) {
        std::unique_lock guard{local_mutex};
        reports_root.log << "index " << index << ", thread " << std::this_thread::get_id() << std::endl;
        guard.unlock();
        const auto& current_hash = hash_vec[index];
        Generator c_gen = gen;
        auto [hash_name, counters] = HashTestWithGenBlocks(c_gen, current_hash, wp, reports_root);
        //std::scoped_lock guard{local_mutex};
        guard.lock();
        collisions[hash_name] = counters;
    };

    const size_t hardware_threads = std::thread::hardware_concurrency();
    const size_t num_threads = hardware_threads != 0 ? hardware_threads : 1;

    reports_root.log << "Num threads = " << num_threads << std::endl;

    const uint64_t chunks = hash_vec.size() / num_threads;
    std::vector<std::thread> threads(num_threads - 1);
    for (uint64_t i = 0; i < chunks; ++i) {
        for (size_t j = 0; i < threads.size(); ++i) {
            threads[i] = std::thread{lambda, i * num_threads + j};
        }
        lambda(i * num_threads + threads.size());

        for (auto& t : threads) {
            t.join();
        }
    }

    uint64_t diff = hash_vec.size() - chunks * num_threads;
    for (size_t i = 0; i < diff; ++i) {
        threads[i] = std::thread{lambda, i + chunks * num_threads};
    }
    for (size_t i = 0; i < diff; ++i) {
        threads[i].join();
    }
    obj["Collisions"] = collisions;
    out << obj;

    reports_root.log << "end " << wp.hash_bits << " bits" << std::endl << std::endl;

}

void RunTestWithGeneratedBlocks(uint32_t words_length, ReportsRoot& reports_root);

#endif //THESIS_WORK_WORDS_TESTS_H
