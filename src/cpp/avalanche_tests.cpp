#include "avalanche_tests.h"

#include <boost/format.hpp>

namespace tests {

    namespace out {
        OutputJson GetAvalancheTestJson(const TestParameters& tp, ReportsRoot& reports_root) {
            const std::string test_name = "Avalanche effect tests";
            const std::filesystem::path report_test_dir = test_name;
            const auto report_test_path = reports_root.root_path / report_test_dir;
            std::filesystem::create_directories(report_test_path);

            const std::filesystem::path report_name = std::to_string(tp.hash_bits) + " bits.json";
            const std::filesystem::path out_path = report_test_path / report_name;
            std::ofstream out(out_path);
            assert(out);

            boost::json::object obj;
            obj["Test name"] = test_name;
            obj["Bits"] = tp.hash_bits;
            return OutputJson{std::move(obj), std::move(out)};
        }

        boost::json::object AvalancheInfoToJson(const AvalancheInfo& avalanche_info, ReportsRoot& reports_root) {
            boost::json::object avalanche_statistics;

            /*reports_root.logger << boost::format("\t\thamming_distance:\t%1%\n") % avalanche_info.hamming_distance;
            reports_root.logger << boost::format("\t\tfrequency_occurrence: %1%\n") % avalanche_info.frequency_occurrence;
            reports_root.logger << boost::format("\t\toriginal_number:\t%1% = %2%\n")
                % std::bitset<64>(avalanche_info.original_number) % avalanche_info.original_number;
            reports_root.logger << boost::format("\t\tmodified_number:\t%1% = %2%\n")
                % std::bitset<64>(avalanche_info.modified_number) % avalanche_info.modified_number;
            reports_root.logger << boost::format("\t\toriginal_hash:\t\t%1% = %2%\n")
                % std::bitset<64>(avalanche_info.original_hash) % avalanche_info.original_hash;
            reports_root.logger << boost::format("\t\tmodified_hash:\t\t%1% = %2%\n")
                % std::bitset<64>(avalanche_info.modified_hash) % avalanche_info.modified_hash;*/

            reports_root.logger << boost::format("\t\thamming_distance: %1%\n") % avalanche_info.hamming_distance;
            reports_root.logger << boost::format("\t\tfrequency_occurrence: %1%\n") % avalanche_info.frequency_occurrence;
            reports_root.logger << boost::format("\t\toriginal_number: %1%\n") % avalanche_info.original_number;
            reports_root.logger << boost::format("\t\tmodified_number: %1%\n") % avalanche_info.modified_number;
            reports_root.logger << boost::format("\t\toriginal_hash: %1%\n") % avalanche_info.original_hash;
            reports_root.logger << boost::format("\t\tmodified_hash: %1%\n") % avalanche_info.modified_hash;

            avalanche_statistics["Hamming distance"] = avalanche_info.hamming_distance;
            avalanche_statistics["Frequency occurrence"] = avalanche_info.frequency_occurrence;
            avalanche_statistics["Original number"] = avalanche_info.original_number;
            avalanche_statistics["Modified number"] = avalanche_info.modified_number;
            avalanche_statistics["Original hash"] = avalanche_info.original_hash;
            avalanche_statistics["Modified hash"] = avalanche_info.modified_hash;

            return avalanche_statistics;
        }
    }



    #define RUN_AVALANCHE_TEST_IMPL(FUNC, NUM_KEYS, BITS, NUM_THREADS, ROOT) \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes();   \
        const TestParameters tp##BITS{BITS, NUM_KEYS, NUM_THREADS, TestFlag::NORMAL};     \
        AvalancheTest(FUNC, hashes##BITS, tp##BITS, ROOT)

    void RunTestWithAllNumbers(size_t num_threads, ReportsRoot& reports_root) {
        auto lambda = [](const auto& hs, const TestParameters& tp, ReportsRoot& reports_root) {
            return HashTestWithAllNumbers(hs, tp, reports_root);
        };

        uint64_t num_keys = std::numeric_limits<uint16_t>::max() + 1ull;
        RUN_AVALANCHE_TEST_IMPL(lambda, num_keys, 16, num_threads, reports_root);

        num_keys = static_cast<uint64_t>(std::numeric_limits<hfl::uint24_t>::max()) + 1ull;
        //RUN_AVALANCHE_TEST_IMPL(lambda, num_keys, 24, num_threads, reports_root);

        num_keys = std::numeric_limits<uint32_t>::max() + 1ull;
        //RUN_AVALANCHE_TEST_IMPL(lambda, num_keys, 32, num_threads, reports_root);
    }

    void RunTestWithGenerateNumbers(size_t num_threads, ReportsRoot& reports_root) {
        auto lambda = [](const auto& hs, const TestParameters& tp, ReportsRoot& reports_root) {
            return HashTestWithGenerateNumbers(hs, tp, reports_root);
        };

        const uint64_t num_keys = std::numeric_limits<uint32_t>::max() + 1ull;
        RUN_AVALANCHE_TEST_IMPL(lambda, num_keys, 48, num_threads, reports_root);
        RUN_AVALANCHE_TEST_IMPL(lambda, num_keys, 64, num_threads, reports_root);
    }

    void RunAvalancheTests(ReportsRoot& reports_root) {
        const size_t hardware_threads = std::thread::hardware_concurrency();
        const size_t num_threads = hardware_threads != 0 ? hardware_threads : 1;
        reports_root.logger << boost::format("\tnum_threads = %1%\n\n") % num_threads;
        RunTestWithAllNumbers(num_threads, reports_root);
        //RunTestWithGenerateNumbers(num_threads, reports_root);
    }
}