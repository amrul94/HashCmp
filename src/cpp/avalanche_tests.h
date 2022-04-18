#ifndef THESISWORK_AVALANCHE_TESTS_H
#define THESISWORK_AVALANCHE_TESTS_H

#include <bitset>
#include <concepts>
#include <compare>
#include <future>
#include <vector>
#include <thread>

#include <boost/format.hpp>
#include <boost/assert.hpp>

#include <pcg_random.hpp>

#include "concurrency.h"
#include "generators.h"
#include "hashes.h"
#include "my_assert.h"
#include "test_parameters.h"
#include "log_duration.h"

namespace tests {
    class AvalancheInfo;

    namespace out {
        OutputJson GetAvalancheTestJson(const TestParameters& tp, ReportsRoot& reports_root);
        boost::json::object AvalancheInfoToJson(const AvalancheInfo& avalanche_info);
    }

    struct DistanceAndFrequency {
        uint8_t value = 0;
        uint64_t frequency = 0;

        DistanceAndFrequency() = default;

        DistanceAndFrequency(std::integral auto other_distance)
                : value(other_distance)
                , frequency(1) {
        }

        DistanceAndFrequency operator=(std::integral auto other_distance) {
            value = other_distance;
            frequency = 1;
            return *this;
        }

        uint16_t GetValue() const { return static_cast<uint16_t>(value); };

        auto operator<=>(const DistanceAndFrequency& other) const { return value <=> other.value; };
        auto operator<=>(std::integral auto other_distance) const { return value <=> other_distance; };

        bool operator==(const DistanceAndFrequency& other) const { return value == other.value; };
        bool operator==(std::integral auto other_distance) const { return value == other_distance; };
    };

    struct HammingDistance {
        DistanceAndFrequency min {std::numeric_limits<uint8_t>::max()};     // худший случай
        DistanceAndFrequency max;                                           // лучший случай
        long double avg = 0;                                                // среднее значение

    };

    std::ostream& operator<<(std::ostream &os, const DistanceAndFrequency& distance_and_frequency);
    std::ostream& operator<<(std::ostream &os, const HammingDistance& distance);

    struct NumberAndHash {
        uint64_t number = 0;
        uint64_t hash = 0;
    };

    std::ostream& operator<<(std::ostream &os, const NumberAndHash& number_and_hash);

    struct AvalancheInfo {
        HammingDistance hamming_distance;
        NumberAndHash original_pair;        // Пара оригинальные число-хэш для худшего случая
        NumberAndHash modified_pair;        // Пара измененные число-хэш для худшего случая
    };

    std::ostream& operator<<(std::ostream &os, const AvalancheInfo& avalanche_info);

    void CompareAndChangeMinHammingDistance(AvalancheInfo& avalanche_info, const DistanceAndFrequency& dist_and_freq,
                                         const NumberAndHash& original, const NumberAndHash& modified);

    void CompareAndChangeMaxHammingDistance(AvalancheInfo& avalanche_info, const DistanceAndFrequency& dist_and_freq);

    static inline long double CalculateArithmeticMean(long double prev_avg, long double current_value, long double step) {
        long double multiplier1 = 1l / (step + 1l);
        long double multiplier2 = step * prev_avg + current_value;
        return multiplier1 * multiplier2;
    }

    template<typename HashStruct>
    void CalculateHammingDistance(AvalancheInfo& avalanche_info, const HashStruct& hs, const TestParameters& tp,
                                  uint64_t original_number, uint64_t iteration_step);


    template<typename HashStruct>
    AvalancheInfo HashAvalancheTest(const HashStruct& hs, const TestParameters& tp, ReportsRoot& reports_root);

    template<typename TestFunc, typename HashStructs>
    void AvalancheTest(TestFunc func, const HashStructs& funcs, const TestParameters& tp, ReportsRoot& reports_root);

    void RunAvalancheTests(ReportsRoot& reports_root);


// ==================================================

    template<typename HashStruct>
    void CalculateHammingDistance(AvalancheInfo& avalanche_info, const HashStruct& hs, const TestParameters& tp,
                                           uint64_t original_number, uint64_t iteration_step) {
        constexpr uint8_t bit_size = 64;
        const NumberAndHash original {original_number, hs.hasher(original_number)};
        for (uint8_t bit_index = 0; bit_index < bit_size; ++bit_index) {
            std::bitset<bit_size> modified_number_bits = original_number;
            modified_number_bits.flip(bit_index);
            const uint64_t modified_number = modified_number_bits.to_ullong();
            const NumberAndHash modified {modified_number, hs.hasher(modified_number)};
            std::bitset<bit_size> xor_hashes = original.hash ^ modified.hash;
            DistanceAndFrequency hamming_distance = xor_hashes.count();

            CompareAndChangeMinHammingDistance(avalanche_info, hamming_distance, original, modified);
            CompareAndChangeMaxHammingDistance(avalanche_info, hamming_distance);
            avalanche_info.hamming_distance.avg = CalculateArithmeticMean(avalanche_info.hamming_distance.avg,
                                                                          hamming_distance.value, iteration_step);
        }
    }

    template<typename HashStruct>
    AvalancheInfo HashAvalancheTest(const HashStruct& hs, const TestParameters& tp, ReportsRoot& reports_root) {
        LOG_DURATION_STREAM("\t\ttime", reports_root.logger);
        reports_root.logger << boost::format("\n\t%1%: \n") % hs.name;

        auto generators = GetGenerators(tp.num_threads, tp.num_keys);
        std::atomic_uint16_t gen_index = 0;
        auto thread_task = [&hs, &tp, &generators, &gen_index] (uint64_t first, uint64_t last) {
            pcg64 rng = generators[gen_index++];
            AvalancheInfo avalanche_info;
            while (first < last) {
                uint64_t number = rng();
                CalculateHammingDistance(avalanche_info, hs, tp, number, ++first);
            }
            return avalanche_info;
        };

        uint16_t step = 0;
        auto merge_results = [&step](AvalancheInfo& best_result, const AvalancheInfo& current_result)  {
            CompareAndChangeMinHammingDistance(best_result, current_result.hamming_distance.min,
                                               current_result.original_pair, current_result.modified_pair);
            CompareAndChangeMaxHammingDistance(best_result, current_result.hamming_distance.max);
            best_result.hamming_distance.avg = CalculateArithmeticMean(best_result.hamming_distance.avg,
                                                                       current_result.hamming_distance.avg, ++step);
        };

        ThreadTasks<AvalancheInfo> tasks(thread_task, merge_results, tp.num_threads, tp.num_keys);
        const auto result = tasks.GetResult();

        ASSERT_EQUAL_HINT(result.original_pair.hash, static_cast<uint64_t>(hs.hasher(result.original_pair.number)),
                          hs.name + " is not correct");
        ASSERT_EQUAL_HINT(result.modified_pair.hash, static_cast<uint64_t>(hs.hasher(result.modified_pair.number)),
                          hs.name + " is not correct");

        reports_root.logger << result << std::endl;
        return result;
    }


    template<typename HashStructs>
    void AvalancheTest(const HashStructs& hashes, const TestParameters& tp, ReportsRoot& reports_root) {
        reports_root.logger << "--- START " << tp.hash_bits << " BITS TEST ---" << std::endl;

        auto out_json = out::GetAvalancheTestJson(tp, reports_root);
        boost::json::object avalanche_statistics;
        for (const auto& hs : hashes) {
            AvalancheInfo avalanche_info = HashAvalancheTest(hs, tp, reports_root);
            avalanche_statistics[hs.name] = out::AvalancheInfoToJson(avalanche_info);
        }

        out_json.obj["Avalanche effect"] = avalanche_statistics;
        out_json.out << out_json.obj;
        reports_root.logger << "\n--- END " << tp.hash_bits << " BITS TEST ---\n" << std::endl;
    }
}



#endif //THESISWORK_AVALANCHE_TESTS_H
