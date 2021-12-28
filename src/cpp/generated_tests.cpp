#include <random>

#include <pcg_random.hpp>

#include "generated_tests.h"
#include "hashes.h"

#define RUN_COLL_TEST_NORMAL_IMPL(GENS, BITS, POW_COUNTS, LENGTH, NUM_THREADS, MODE, ROOT)  \
    const auto word_counts##BITS = static_cast<uint64_t>(pow(2, POW_COUNTS));               \
    const GenBlocksParameters wp##BITS {BITS, word_counts##BITS, LENGTH, NUM_THREADS, MODE};    \
    const auto hashes##BITS = hfl::Build##BITS##bitsHashes(hfl::BuildFlag::ALL);            \
    TestWithGeneratedBlocks(GENS, hashes##BITS, wp##BITS, ROOT)

void RunCollTestNormal(const std::vector<pcg64>& generators, uint16_t words_length, size_t num_threads,
                       ReportsRoot& reports_root) {
    RUN_COLL_TEST_NORMAL_IMPL(generators, 16, 16, words_length, num_threads, TestFlag::NORMAL, reports_root);
    //RUN_COLL_TEST_NORMAL_IMPL(generators, 24, 24, words_length, num_threads, TestFlag::NORMAL, reports_root);
    //RUN_COLL_TEST_NORMAL_IMPL(generators, 32, 24, words_length, num_threads, TestFlag::NORMAL, reports_root);
}

#define RUN_COLL_TEST_WITH_MASK_IMPL(GENS, BITS, POW_COUNTS, LENGTH, NUM_THREADS, MODE, ROOT)       \
    const auto word_counts##BITS = static_cast<uint64_t>(pow(2, POW_COUNTS));                       \
    const GenBlocksParameters wp##BITS {BITS, 32, word_counts##BITS, LENGTH, NUM_THREADS, MODE};    \
    const auto hashes##BITS = hfl::Build##BITS##bitsHashes(hfl::BuildFlag::ALL);                    \
    TestWithGeneratedBlocks(GENS, hashes##BITS, wp##BITS, ROOT)

void RunCollTestWithMask(const std::vector<pcg64>& generators, uint16_t words_length, size_t num_threads,
                       ReportsRoot& reports_root) {
    RUN_COLL_TEST_WITH_MASK_IMPL(generators, 48, 24, words_length, num_threads, TestFlag::MASK, reports_root);
    RUN_COLL_TEST_WITH_MASK_IMPL(generators, 64, 24, words_length, num_threads, TestFlag::MASK, reports_root);
}

std::vector<pcg64> GetGenerators(uint16_t words_length) {
    const size_t hardware_threads = std::thread::hardware_concurrency();
    //const size_t num_threads = hardware_threads != 0 ? hardware_threads : 1;
    const size_t num_threads = 1;
    std::vector<pcg64> generators(num_threads);

    for (auto& generator : generators) {
        pcg_extras::seed_seq_from<std::random_device> seed_source;
        generator.seed(seed_source);
    }

    std::cout << boost::format("num_threads = %1%\n") % num_threads;
    return generators;
}

void RunTestWithGeneratedBlocks(uint16_t words_length, ReportsRoot& reports_root) {
    std::vector<pcg64> generators = GetGenerators(words_length);

    RunCollTestNormal(generators, words_length, generators.size(), reports_root);
    //RunCollTestWithMask(generators, words_length, generators.size(), reports_root);
}