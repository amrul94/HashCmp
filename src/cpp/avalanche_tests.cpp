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

        boost::json::object AvalancheInfoToJson(const AvalancheInfo& avalanche_info) {
            boost::json::object avalanche_statistics;

            boost::json::object worst_case;
            worst_case["Distance"] = avalanche_info.hamming_distance.min.value;
            worst_case["Frequency"] = avalanche_info.hamming_distance.min.frequency;
            avalanche_statistics["Worst case"] = worst_case;

            boost::json::object best_case;
            best_case["Distance"] = avalanche_info.hamming_distance.max.value;
            best_case["Frequency"] = avalanche_info.hamming_distance.max.frequency;
            avalanche_statistics["Best case"] = best_case;

            boost::json::object original_pair;
            original_pair["Number"] = avalanche_info.original_pair.number;
            original_pair["Hash"] = avalanche_info.original_pair.hash;
            avalanche_statistics["Original pair"] = original_pair;

            boost::json::object modified_pair;
            modified_pair["Number"] = avalanche_info.modified_pair.number;
            modified_pair["Hash"] = avalanche_info.modified_pair.hash;
            avalanche_statistics["Modified pair"] = modified_pair;

            return avalanche_statistics;
        }
    }

    std::ostream& operator<<(std::ostream &os, const DistanceAndFrequency& distance_and_frequency) {
        return os  << "value = " << distance_and_frequency.GetValue()
                   << ", frequency = " << distance_and_frequency.frequency;
    }

    std::ostream& operator<<(std::ostream &os, const HammingDistance& distance) {
        return os  << "\t\t\tminimum: " << distance.min
                   << "\n\t\t\tmaximum: " << distance.max
                   << "\n\t\t\taverage: " << distance.avg;
    }

    std::ostream& operator<<(std::ostream &os, const NumberAndHash& number_and_hash) {
        return os  << "number = " << number_and_hash.number << ", hash = " << number_and_hash.hash;
    }

    std::ostream& operator<<(std::ostream &os, const AvalancheInfo& avalanche_info) {
        return os  << "\t\thamming value:\n" << avalanche_info.hamming_distance
                   << "\n\t\toriginal: " << avalanche_info.original_pair
                   << "\n\t\tmodified: " << avalanche_info.modified_pair;
    }


    void CompareAndChangeMinHammingDistance(AvalancheInfo& avalanche_info, const DistanceAndFrequency& dist_and_freq,
                                            const NumberAndHash& original, const NumberAndHash& modified) {
        if (avalanche_info.hamming_distance.min == dist_and_freq) {
            avalanche_info.hamming_distance.min.frequency += dist_and_freq.frequency;
        } else if (avalanche_info.hamming_distance.min > dist_and_freq) {
            avalanche_info.hamming_distance.min = dist_and_freq;
            avalanche_info.original_pair = original;
            avalanche_info.modified_pair = modified;
        }
    }

    void CompareAndChangeMaxHammingDistance(AvalancheInfo& avalanche_info, const DistanceAndFrequency& dist_and_freq) {
        if (avalanche_info.hamming_distance.max == dist_and_freq) {
            avalanche_info.hamming_distance.max.frequency += dist_and_freq.frequency;
        } else if (avalanche_info.hamming_distance.max < dist_and_freq) {
            avalanche_info.hamming_distance.max = dist_and_freq;
        }
    }

    #define RUN_AVALANCHE_TEST_IMPL(NUM_KEYS, BITS, NUM_THREADS, ROOT)                  \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes();                       \
        const TestParameters tp##BITS{BITS, NUM_KEYS, NUM_THREADS, TestFlag::NORMAL};   \
        AvalancheTest(hashes##BITS, tp##BITS, ROOT)

    void RunAvalancheTests(ReportsRoot& reports_root) {
        const size_t hardware_threads = std::thread::hardware_concurrency();
        const size_t num_threads = hardware_threads != 0 ? hardware_threads : 1;
        reports_root.logger << boost::format("\tnum_threads = %1%\n\n") % num_threads;

        const uint64_t num_keys = std::numeric_limits<uint32_t>::max() + 1ull;
        RUN_AVALANCHE_TEST_IMPL(num_keys, 16, num_threads, reports_root);
        RUN_AVALANCHE_TEST_IMPL(num_keys, 32, num_threads, reports_root);
        RUN_AVALANCHE_TEST_IMPL(num_keys, 64, num_threads, reports_root);
    }
}