#include <random>

#include <pcg_random.hpp>

#include "generated_tests.h"
#include "hashes.h"

namespace tests {
    namespace detail {
        OutputJson GetGenTestJson(const GenBlocksParameters& gbp, ReportsRoot& reports_root) {
            using namespace std::literals;
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
            return OutputJson{std::move(obj), std::move(out)};
        }
    }

    #define RUN_COLL_TEST_NORMAL_IMPL(GEN, BITS, POW_COUNTS, LENGTH, NUM_THREADS, MODE, ROOT)      \
        const auto word_counts##BITS = static_cast<uint64_t>(pow(2, POW_COUNTS));                   \
        const GenBlocksParameters wp##BITS {BITS, word_counts##BITS, LENGTH, NUM_THREADS, MODE};    \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes(hfl::BuildFlag::ALL);                \
        TestWithGeneratedBlocks(GEN, hashes##BITS, wp##BITS, ROOT)

    void RunCollTestNormal(pcg64& generator, uint16_t words_length, size_t num_threads, ReportsRoot& reports_root) {
        RUN_COLL_TEST_NORMAL_IMPL(generator, 16, 16, words_length, num_threads, TestFlag::NORMAL, reports_root);
        RUN_COLL_TEST_NORMAL_IMPL(generator, 24, 24, words_length, num_threads, TestFlag::NORMAL, reports_root);
        RUN_COLL_TEST_NORMAL_IMPL(generator, 32, 24, words_length, num_threads, TestFlag::NORMAL, reports_root);
    }

    #define RUN_COLL_TEST_WITH_MASK_IMPL(GEN, BITS, POW_COUNTS, LENGTH, NUM_THREADS, MODE, ROOT)       \
        const auto word_counts##BITS = static_cast<uint64_t>(pow(2, POW_COUNTS));                       \
        const GenBlocksParameters wp##BITS {BITS, 32, word_counts##BITS, LENGTH, NUM_THREADS, MODE};    \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes(hfl::BuildFlag::ALL);                    \
        TestWithGeneratedBlocks(GEN, hashes##BITS, wp##BITS, ROOT)

    void RunCollTestWithMask(pcg64& generator, uint16_t words_length, size_t num_threads, ReportsRoot& reports_root) {
        RUN_COLL_TEST_WITH_MASK_IMPL(generator, 48, 24, words_length, num_threads, TestFlag::MASK, reports_root);
        RUN_COLL_TEST_WITH_MASK_IMPL(generator, 64, 24, words_length, num_threads, TestFlag::MASK, reports_root);
    }

    std::vector<pcg64> GetGenerators(uint16_t words_length) {
        const size_t hardware_threads = std::thread::hardware_concurrency();
        const size_t num_threads = hardware_threads != 0 ? hardware_threads : 1;
        std::vector<pcg64> generators(num_threads);

        for (auto& generator : generators) {
            pcg_extras::seed_seq_from<std::random_device> seed_source;
            generator.seed(seed_source);
        }

        std::cout << boost::format("\tnum_threads = %1%\n") % num_threads;
        return generators;
    }

    void RunTestWithGeneratedBlocks(uint16_t words_length, ReportsRoot& reports_root) {
        pcg64 generator;

        RunCollTestNormal(generator, words_length, 1, reports_root);
        RunCollTestWithMask(generator, words_length, 1, reports_root);
    }
}