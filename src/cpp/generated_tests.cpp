#include <random>

#include <pcg_random.hpp>

#include "generated_tests.h"
#include "hashes.h"

namespace tests {
    namespace out {
        OutputJson GetGenTestJson(const GenBlocksParameters& gbp, out::Logger& logger) {
            using namespace std::literals;
            const std::filesystem::path gen_tests_dir = "Generated blocks tests";
            const std::filesystem::path block_size_dir = std::to_string(gbp.words_length);
            const auto block_size_path = logger.GetLogDirPath() / gen_tests_dir / block_size_dir;
            std::filesystem::create_directories(block_size_path);

            const std::filesystem::path report_name = std::to_string(gbp.hash_bits) + " bits (" + TestFlagToString(gbp.mode)
                                                      + " "s + std::to_string(gbp.mask_bits) + " bits).json"s;
            const std::filesystem::path report_path = block_size_path / report_name;

            std::ofstream out(report_path);
            assert(out);
            boost::json::object obj;
            obj["Test name"] = "Test With Random Words";
            obj["Mode"] = TestFlagToString(gbp.mode);
            obj["Bits"] = gbp.hash_bits;
            obj["Mask"] = gbp.mask_bits;
            obj["Number of keys"] = gbp.num_keys;
            return OutputJson{std::move(obj), std::move(out)};
        }
    }

    #define RUN_COLL_TEST_NORMAL_IMPL(BITS, POW_COUNTS, LENGTH, NUM_THREADS, MODE, ROOT)                        \
        const auto word_counts##BITS = (1 << (POW_COUNTS)); /*static_cast<uint64_t>(pow(2, POW_COUNTS));   */ \
        const GenBlocksParameters wp##BITS {BITS, BITS, NUM_THREADS, word_counts##BITS, LENGTH, MODE};     \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes();                            \
        TestWithGeneratedBlocks(hashes##BITS, wp##BITS, ROOT)

    void RunCollTestNormal(uint16_t words_length, uint16_t num_threads, out::Logger& logger) {
        RUN_COLL_TEST_NORMAL_IMPL(16, 16, words_length, num_threads, TestFlag::NORMAL, logger);
        RUN_COLL_TEST_NORMAL_IMPL(24, 24, words_length, num_threads, TestFlag::NORMAL, logger);
        RUN_COLL_TEST_NORMAL_IMPL(32, 24, words_length, num_threads, TestFlag::NORMAL, logger);
    }

    #define RUN_COLL_TEST_WITH_MASK_IMPL(BITS, POW_COUNTS, LENGTH, NUM_THREADS, MODE, ROOT)       \
        const auto word_counts##BITS = (1 << (POW_COUNTS)); /*static_cast<uint64_t>(pow(2, POW_COUNTS));*/\
        const GenBlocksParameters wp##BITS {BITS, 32, NUM_THREADS, word_counts##BITS, LENGTH, MODE};    \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes();                    \
        TestWithGeneratedBlocks(hashes##BITS, wp##BITS, ROOT)

    void RunCollTestWithMask(uint16_t words_length, uint16_t num_threads, out::Logger& logger) {
        RUN_COLL_TEST_WITH_MASK_IMPL(48, 24, words_length, num_threads, TestFlag::MASK, logger);
        RUN_COLL_TEST_WITH_MASK_IMPL(64, 24, words_length, num_threads, TestFlag::MASK, logger);
    }

    void RunTestWithGeneratedBlocks(uint16_t words_length, out::Logger& logger) {
        out::StartAndEndLogTest start_and_end_log(logger,
                                                  "GENERATED BLOCKS (length = " + std::to_string(words_length) + ")");
        const uint16_t num_threads = GetNumThreads();
        RunCollTestNormal(words_length, num_threads, logger);
        RunCollTestWithMask(words_length, num_threads, logger);
    }
}