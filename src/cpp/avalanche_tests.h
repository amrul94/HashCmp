#ifndef THESISWORK_AVALANCHE_TESTS_H
#define THESISWORK_AVALANCHE_TESTS_H

#include <bitset>
#include <compare>
#include <future>
#include <vector>
#include <thread>

#include <boost/format.hpp>

#include <pcg_random.hpp>

#include "concurrency.h"
#include "generator.h"
#include "hashes.h"
#include "test_parameters.h"
#include "log_duration.h"

namespace tests {

    struct AvalancheInfo {
        AvalancheInfo() = default;

        uint16_t hamming_distance = std::numeric_limits<uint16_t>::max();
        uint64_t frequency_occurrence = 0;
        uint64_t original_number = 0;
        uint64_t modified_number = 0;
        uint64_t original_hash = 0;
        uint64_t modified_hash = 0;
    };

    namespace out {
        OutputJson GetAvalancheTestJson(const TestParameters& tp, ReportsRoot& reports_root);
        boost::json::object AvalancheInfoToJson(const AvalancheInfo& avalanche_info, ReportsRoot& reports_root);
    }



    template<typename HashStruct>
    void CalculateHammingDistance(AvalancheInfo& avalanche_info, const HashStruct& hs, const TestParameters& tp,
                                  uint64_t original_number);

    static inline void MergeResults(AvalancheInfo& best_result, AvalancheInfo current_result)  {
        if (best_result.hamming_distance == current_result.hamming_distance) {
            best_result.frequency_occurrence += current_result.frequency_occurrence;
        } else if (best_result.hamming_distance > current_result.hamming_distance) {
            best_result = current_result;
        }
    };

    template<typename HashStruct>
    AvalancheInfo HashTestWithAllNumbers(const HashStruct& hs, const TestParameters& tp, ReportsRoot& reports_root);

    template<typename HashStruct>
    AvalancheInfo HashTestWithGenerateNumbers(const HashStruct& hs, const TestParameters& tp, ReportsRoot& reports_root);

    template<typename TestFunc, typename HashStructs>
    void AvalancheTest(TestFunc func, const HashStructs& funcs, const TestParameters& tp, ReportsRoot& reports_root);

    void RunTestWithAllNumbers(size_t num_threads, ReportsRoot& reports_root);
    void RunTestWithGenerateNumbers(size_t num_threads, ReportsRoot& reports_root);
    void RunAvalancheTests(ReportsRoot& reports_root);


// ==================================================

    template<typename HashStruct>
    void CalculateHammingDistance(AvalancheInfo& avalanche_info, const HashStruct& hs, const TestParameters& tp,
                                           uint64_t original_number) {
        const auto original_hash = static_cast<uint64_t>(hs.hasher(original_number));
        for (uint16_t bit = 0; bit < tp.hash_bits; ++bit) {
            std::bitset<64> modified_number_bits = original_number;
            modified_number_bits.flip(bit);
            uint64_t modified_number = modified_number_bits.to_ullong();
            const auto modified_hash = static_cast<uint64_t>(hs.hasher(modified_number));

            std::bitset<64> xor_hashes = original_hash ^ modified_hash;
            uint16_t current_hamming_distance = xor_hashes.count();

            if (avalanche_info.hamming_distance == current_hamming_distance) {
                ++avalanche_info.frequency_occurrence;
            } else if (avalanche_info.hamming_distance > current_hamming_distance) {
                avalanche_info.hamming_distance = current_hamming_distance;
                avalanche_info.frequency_occurrence = 1;
                avalanche_info.original_number = original_number;
                avalanche_info.modified_number = modified_number;
                avalanche_info.original_hash = original_hash;
                avalanche_info.modified_hash = modified_hash;
            }
        }
    }

    template<typename HashStruct>
    AvalancheInfo HashTestWithAllNumbers(const HashStruct& hs, const TestParameters& tp, ReportsRoot& reports_root) {
        LOG_DURATION_STREAM("\t\ttime", reports_root.logger);

        reports_root.logger << boost::format("\t%1%: \n") % hs.name;

        auto lambda = [&hs, &tp](uint64_t first, uint64_t last) {
            AvalancheInfo avalanche_info;
            for (uint64_t number = first; number < last; ++number) {
                CalculateHammingDistance(avalanche_info, hs, tp, number);
            }
            return avalanche_info;
        };

        ThreadTasks<AvalancheInfo> tasks(lambda, MergeResults, tp.num_threads, tp.num_keys);
        return tasks.GetResult();
    }

    template<typename HashStruct>
    AvalancheInfo HashTestWithGenerateNumbers(const HashStruct& hs, const TestParameters& tp, ReportsRoot& reports_root) {
        LOG_DURATION_STREAM("\t\ttime", reports_root.logger);
        reports_root.logger << boost::format("\t%1%: \n") % hs.name;

        auto generators = GetGenerators(tp.num_threads);
        std::atomic_uint16_t gen_index = 0;
        auto lambda = [&hs, &tp, &generators, &gen_index] (uint64_t first, uint64_t last) {
            pcg64 rng = generators[gen_index++];
            AvalancheInfo avalanche_info;
            for (; first < last; ++first) {
                uint64_t number = rng();
                CalculateHammingDistance(avalanche_info, hs, tp, number);
            }
            return avalanche_info;
        };

        ThreadTasks<AvalancheInfo> tasks(lambda, MergeResults, tp.num_threads, tp.num_keys);
        return tasks.GetResult();
    }


    template<typename TestFunc, typename HashStructs>
    void AvalancheTest(TestFunc func, const HashStructs& hashes, const TestParameters& tp, ReportsRoot& reports_root) {
        reports_root.logger << "--- START " << tp.hash_bits << " BITS TEST ---\n";

        auto out_json = out::GetAvalancheTestJson(tp, reports_root);
        boost::json::object avalanche_statistics;
        for (const auto& hs : hashes) {
            AvalancheInfo avalanche_info = func(hs, tp, reports_root);
            avalanche_statistics[hs.name] = out::AvalancheInfoToJson(avalanche_info, reports_root);
        }

        out_json.obj["Avalanche effect"] = avalanche_statistics;
        out_json.out << out_json.obj;
        reports_root.logger << "--- END " << tp.hash_bits << " BITS TEST ---\n\n";
    }
}



#endif //THESISWORK_AVALANCHE_TESTS_H
