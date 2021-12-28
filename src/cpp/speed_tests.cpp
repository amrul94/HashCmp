#include <random>

#include "hashes.h"
#include "speed_tests.h"

namespace tests {
    namespace detail {
        OutputJson GetSpeedTestJson(uint16_t hash_bits, size_t num_words, ReportsRoot& reports_root) {
            const std::filesystem::path report_test_dir = "Speed tests";
            const auto report_test_path = reports_root.root_path / report_test_dir;
            std::filesystem::create_directories(report_test_path);

            const std::filesystem::path report_name = std::to_string(hash_bits) + " bits.json";
            const std::filesystem::path out_path = report_test_path / report_name;
            std::ofstream out(out_path);

            boost::json::object obj;
            obj["Test name"] = "Speed Tests";
            obj["Bits"] = hash_bits;
            obj["Number of words"] = num_words;
            return OutputJson{std::move(obj), std::move(out)};
        }
    }

    #define RUN_SPEED_TESTS_IMPL(BITS, WORDS, ROOT)                                     \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes(hfl::BuildFlag::ALL);    \
        SpeedTests(hashes##BITS, WORDS, ROOT)


    void RunSpeedTests(uint64_t num_blocks, uint32_t block_length, ReportsRoot& reports_root) {
        pcg_extras::seed_seq_from<std::random_device> seed_source;
        pcg64 rng{seed_source};
        const auto random_blocks = GenerateRandomDataBlocks(rng, num_blocks, block_length);

        RUN_SPEED_TESTS_IMPL(16, random_blocks, reports_root);
        RUN_SPEED_TESTS_IMPL(24, random_blocks, reports_root);
        RUN_SPEED_TESTS_IMPL(32, random_blocks, reports_root);
        RUN_SPEED_TESTS_IMPL(48, random_blocks, reports_root);
        RUN_SPEED_TESTS_IMPL(64, random_blocks, reports_root);

    }
}




