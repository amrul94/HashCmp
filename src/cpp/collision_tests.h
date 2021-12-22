#ifndef THESIS_WORK_WORDS_TESTS_H
#define THESIS_WORK_WORDS_TESTS_H

#include <execution>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <boost/format.hpp>
#include <boost/json.hpp>

#include <pcg_random.hpp>

#include "test_parameters.h"
#include "log_duration.h"

constexpr int KILOBYTE = 1024;
constexpr int FOUR_KILOBYTES = KILOBYTE * 4;

uint64_t CountCollisions(const std::vector<std::uint32_t>& hashes);
uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hashes);

std::vector<std::string> ParseWords(const std::filesystem::path& file_name);

template <typename HashStruct>
uint64_t HashTestWithEngWords(const HashStruct& hs, const std::vector<std::string>& words,
                              const TestParameters& tp, ReportsRoot& reports_root) {
    LOG_DURATION_STREAM(hs.hash_name, std::cout);
    std::map<uint64_t, uint64_t> hashes;
    for (const std::string& word : words) {
        const uint64_t hash = hs.hash_function(word);
        const uint64_t modify = ModifyHash(tp, hash);
        ++hashes[modify];
    }
    return CountCollisions(hashes);
}

template <typename HashStruct>
void TestWithEnglishWords(const std::vector<HashStruct>& hashes, const std::vector<std::string>& words,
                          const TestParameters& tp, ReportsRoot& reports_root) {
    std::cout << boost::format("start test with %1% bits hashes\n") % tp.hash_bits;
    const std::filesystem::path report_test_dir = "English words tests";
    const auto report_test_path = reports_root.root_path / report_test_dir;
    std::filesystem::create_directories(report_test_path);

    const std::filesystem::path report_name = std::to_string(tp.hash_bits) + " bits.json";
    const std::filesystem::path out_path = report_test_path / report_name;
    std::ofstream out(out_path);
    assert(out);

    boost::json::object obj;
    obj["Test name"] = "Test With English Words";
    obj["Bits"] = tp.hash_bits;
    obj["Number of words"] = words.size();

    boost::json::object collisions;
    for (const auto& hs : hashes) {
        collisions[hs.hash_name] = HashTestWithEngWords(hs, words, tp, reports_root);
    }
    obj["Collisions"] = collisions;
    out << obj;

    std::cout << boost::format("end test with %1% bits hashes\n\n") % tp.hash_bits;
}

void RunTestWithEnglishWords(ReportsRoot& reports_root);

// в будущем возможно объединю эту функцию с WriteToHash из hash_wrappers
std::string UintToString(uint64_t src, uint64_t size);
std::string GenerateRandomDataBlockSeq(pcg64& generator, uint32_t length);
std::string GenerateRandomDataBlockPar(std::vector<pcg64>& generators, uint32_t length);

template<typename HashStruct>
auto HashTestWithGenBlocks(std::vector<pcg64>& generators, const HashStruct& hs, const GenBlocksParameters& gbp,
                           ReportsRoot& reports_root) {
    LOG_DURATION_STREAM(hs.hash_name, std::cout);

    const auto num_hashes = static_cast<uint64_t>(std::pow(2, gbp.test_bits));
    std::deque<bool> coll_flags(num_hashes, false);

    boost::json::object collisions;

    uint64_t num_collisions = 0;

    /*std::cerr << boost::format("num_keys = %1%\nhash_bits = %2%\nnum_hashes = %3%\n")
                 % gbp.num_keys % gbp.hash_bits % num_hashes;*/

    auto num_words = static_cast<uint64_t>(pow(2, gbp.test_bits / 2));
    for (uint64_t i = 0; num_words <= gbp.num_keys; num_words *= 2) {
        //std::cerr << boost::format("num_words: %1%\n") % num_words;
        for (; i < num_words; ++i) {
            std::string str = GenerateRandomDataBlockPar(generators, gbp.words_length);
            const uint64_t hash = hs.hash_function(str);
            const uint64_t modified = ModifyHash(gbp, hash);
            //std::cerr << boost::format("string: %1%\n") % str;
            //std::cerr << boost::format("hash: %1%\n") % hash;
            //std::cerr << boost::format("modified: %1%\n\n") % modified;
            bool& coll_flag = coll_flags[modified];
            if (coll_flag) {
                ++num_collisions;
            }
            coll_flag = true;
        }

        std::string index = std::to_string(num_words);
        //std::cerr << boost::format("index: %1%\n") % index;
        collisions[index] = num_collisions;
    }

    auto hash_name = static_cast<boost::json::string>(hs.hash_name);
    if (gbp.mode == TestFlag::MASK) {
        hash_name += " (mask " + std::to_string(gbp.test_bits) + " bits)";
    }
    boost::json::object result;
    result[hash_name] = collisions;
    return std::pair{std::move(hash_name), std::move(collisions)};
}

// Возможно стоит поделить эту функции на части, так как она очень большая
template<typename HashStructs>
void TestWithGeneratedBlocks(const std::vector<pcg64>& generators, const HashStructs& hash_vec,
                             const GenBlocksParameters& gbp, ReportsRoot& reports_root) {
    using namespace std::literals;

    std::cout << "start " << gbp.hash_bits << " bits" << std::endl;

    const std::filesystem::path gen_tests_dir = "Generated blocks tests";
    const std::filesystem::path block_size_dir = std::to_string(gbp.words_length);
    const auto block_size_path = reports_root.root_path / gen_tests_dir / block_size_dir;
    std::filesystem::create_directories(block_size_path);

    const std::filesystem::path report_name = std::to_string(gbp.hash_bits) + " bits (" + TestFlagToString(gbp.mode)
                                              + " "s + std::to_string(gbp.test_bits) + " bits).json"s;
    const std::filesystem::path report_path = block_size_path / report_name;


    std::ofstream out(report_path);
    assert(out);
    boost::json::object obj;
    obj["Test name"] = "Test With Random Words";
    obj["Mode"] = TestFlagToString(gbp.mode);
    obj["Bits"] = gbp.hash_bits;
    obj["Mask"] = gbp.test_bits;
    obj["Number of keys"] = gbp.num_keys;

    boost::json::object collisions;
    std::mutex local_mutex;

    for (const auto& current_hash : hash_vec) {
        std::vector<pcg64> copy_gens = generators;
        auto [hash_name, counters] = HashTestWithGenBlocks(copy_gens, current_hash, gbp, reports_root);
        collisions[hash_name] = std::move(counters);
    }

    obj["Collisions"] = collisions;
    out << obj;

    std::cout << "end " << gbp.hash_bits << " bits" << std::endl << std::endl;
}


void RunCollTestNormal(const std::vector<pcg64>& generators, uint16_t words_length, size_t num_threads,
                       ReportsRoot& reports_root);

void RunCollTestWithMask(const std::vector<pcg64>& generators, uint16_t words_length, size_t num_threads,
                         ReportsRoot& reports_root);

std::vector<pcg64> GetGenerators(uint16_t words_length);
void RunTestWithGeneratedBlocks(uint16_t words_length, ReportsRoot& reports_root);

#endif //THESIS_WORK_WORDS_TESTS_H
