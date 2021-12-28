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

#include "blocks_generator.h"
#include "test_parameters.h"
#include "log_duration.h"

constexpr int KILOBYTE = 1024;
constexpr int FOUR_KILOBYTES = KILOBYTE * 4;

template<typename HashStruct>
auto HashTestWithGenBlocks(std::vector<pcg64>& generators, const HashStruct& hs, const GenBlocksParameters& gbp,
                           ReportsRoot& reports_root);

template<typename HashStructs>
void TestWithGeneratedBlocks(const std::vector<pcg64>& generators, const HashStructs& hash_vec,
                             const GenBlocksParameters& gbp, ReportsRoot& reports_root);


void RunCollTestNormal(const std::vector<pcg64>& generators, uint16_t words_length, size_t num_threads,
                       ReportsRoot& reports_root);

void RunCollTestWithMask(const std::vector<pcg64>& generators, uint16_t words_length, size_t num_threads,
                         ReportsRoot& reports_root);

std::vector<pcg64> GetGenerators(uint16_t words_length);
void RunTestWithGeneratedBlocks(uint16_t words_length, ReportsRoot& reports_root);


// ===============================================================================

template<typename HashStruct>
auto HashTestWithGenBlocks(std::vector<pcg64>& generators, const HashStruct& hs, const GenBlocksParameters& gbp,
                           ReportsRoot& reports_root) {
    LOG_DURATION_STREAM(hs.hash_name, reports_root.logger);

    // Выделить coll_flags, collisions, num_collisions и мьютекс (в будущем) в отдельный класс (например, Counters).
    // При этом подсчет коллизий вынести в метод класса AddHash
    const auto num_hashes = static_cast<uint64_t>(std::pow(2, gbp.test_bits));
    std::deque<bool> coll_flags(num_hashes, false);
    boost::json::object collisions;
    uint64_t num_collisions = 0;

    auto num_words = static_cast<uint64_t>(pow(2, gbp.test_bits / 2));
    for (uint64_t i = 0; num_words <= gbp.num_keys; num_words *= 2) {
        for (; i < num_words; ++i) {
            std::string str = GenerateRandomDataBlock(generators.back(), gbp.words_length);
            const uint64_t hash = hs.hash_function(str);
            const uint64_t modified = ModifyHash(gbp, hash);
            bool& coll_flag = coll_flags[modified];
            const uint64_t tmp = coll_flag ? 1 : 0;
            num_collisions += tmp;
            coll_flag = true;
        }

        std::string index = std::to_string(num_words);
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

    reports_root.logger << "start " << gbp.hash_bits << " bits" << std::endl;

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

    reports_root.logger << "end " << gbp.hash_bits << " bits" << std::endl << std::endl;
}

#endif //THESIS_WORK_WORDS_TESTS_H