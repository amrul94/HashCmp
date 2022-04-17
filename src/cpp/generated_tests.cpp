#include <random>

#include <pcg_random.hpp>

#include "generated_tests.h"
#include "hashes.h"

namespace tests {
    namespace out {
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

    #define RUN_COLL_TEST_NORMAL_IMPL(BITS, POW_COUNTS, LENGTH, NUM_THREADS, MODE, ROOT)                        \
        const auto word_counts##BITS = static_cast<uint64_t>(pow(2, POW_COUNTS));   /*2 << POW_COUNT*/          \
        const GenBlocksParameters wp##BITS {BITS, word_counts##BITS, LENGTH, NUM_THREADS, MODE};                \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes(hfl::BuildFlag::ALL);                            \
        std::vector<pcg64> generators##BITS = GetGenerators(NUM_THREADS, ((LENGTH) * word_counts##BITS) / 8);   \
        TestWithGeneratedBlocks(generators##BITS, hashes##BITS, wp##BITS, ROOT)

    void RunCollTestNormal(uint16_t words_length, size_t num_threads, ReportsRoot& reports_root) {
        //RUN_COLL_TEST_NORMAL_IMPL(generator, 16, 16, words_length, num_threads, TestFlag::NORMAL, reports_root);
        //RUN_COLL_TEST_NORMAL_IMPL(generator, 24, 24, words_length, num_threads, TestFlag::NORMAL, reports_root);
        //RUN_COLL_TEST_NORMAL_IMPL(32, 24, words_length, num_threads, TestFlag::NORMAL, reports_root);
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

    void RunTestWithGeneratedBlocks(uint16_t words_length, ReportsRoot& reports_root) {
        const size_t num_threads = GetNumThreads();
        RunCollTestNormal(words_length, num_threads, reports_root);
        //RunCollTestWithMask(generator, words_length, num_threads, reports_root);
    }
}