#ifndef THESIS_WORK_AVALANCHE_TESTS_H
#define THESIS_WORK_AVALANCHE_TESTS_H

#include <bitset>
#include <concepts>
#include <compare>
#include <future>
#include <vector>
#include <thread>

#include <boost/assert.hpp>
#include <boost/format.hpp>

#include <pcg_random.hpp>

#include "concurrency.h"
#include "generators.h"
#include "hashes.h"
#include "my_assert.h"
#include "test_parameters.h"
#include "timers.h"
#include "output.h"

namespace tests {
    class AvalancheInfo;

    namespace out {
        OutputJson GetAvalancheTestJson(const AvalancheTestParameters& parameters, out::Logger& loger);
        boost::json::object AvalancheInfoToJson(const AvalancheInfo& avalanche_info);
    }

    struct DistanceAndFrequency {
        uint16_t value = 0;
        uint64_t frequency = 0;

        DistanceAndFrequency() = default;

        explicit DistanceAndFrequency(std::integral auto other_distance)
            : value(other_distance)
            , frequency(1) {
        }

        DistanceAndFrequency operator=(std::integral auto other_distance) {
            value = other_distance;
            frequency = 1;
            return *this;
        }

        auto operator<=>(const DistanceAndFrequency& other) const { return value <=> other.value; };
        auto operator<=>(std::integral auto other_distance) const { return value <=> other_distance; };

        bool operator==(const DistanceAndFrequency& other) const { return value == other.value; };
        bool operator==(std::integral auto other_distance) const { return value == other_distance; };
    };

    struct HammingDistance {
        DistanceAndFrequency min {std::numeric_limits<uint8_t>::max()};     // худший случай
        DistanceAndFrequency max {0};                                       // лучший случай
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

    template<hfl::UnsignedIntegral UintT>
    void CalculateHammingDistance(AvalancheInfo& avalanche_info, const hfl::Hash<UintT>& hash,
                                  const AvalancheTestParameters& tp, uint64_t original_number, uint64_t iteration_step);

    template<hfl::UnsignedIntegral UintT>
    AvalancheInfo HashAvalancheTest(const hfl::Hash<UintT>& hash, const AvalancheTestParameters& tp,
                                    out::Logger& logger);

    template<hfl::UnsignedIntegral UintT>
    void AvalancheTest(const std::vector<hfl::Hash<UintT>>& hashes, const AvalancheTestParameters& tp,
                       out::Logger& logger);

    void RunAvalancheTests(out::Logger& loger);


// ==================================================

    template<hfl::UnsignedIntegral UintT>
    void CalculateHammingDistance(AvalancheInfo& avalanche_info, const hfl::Hash<UintT>& hash,
                                  const AvalancheTestParameters& tp, uint64_t original_number,
                                  uint64_t iteration_step) {
        constexpr uint8_t bit_size = 64;
        const NumberAndHash original {original_number, hash(original_number)};
        for (uint8_t bit_index = 0; bit_index < bit_size; ++bit_index) {
            std::bitset<bit_size> modified_number_bits = original_number;
            modified_number_bits.flip(bit_index);
            const uint64_t modified_number = modified_number_bits.to_ullong();
            const NumberAndHash modified {modified_number, hash(modified_number)};
            std::bitset<bit_size> xor_hashes = original.hash ^ modified.hash;
            DistanceAndFrequency hamming_distance{xor_hashes.count()};

            CompareAndChangeMinHammingDistance(avalanche_info, hamming_distance, original, modified);
            CompareAndChangeMaxHammingDistance(avalanche_info, hamming_distance);
            avalanche_info.hamming_distance.avg = CalculateArithmeticMean(avalanche_info.hamming_distance.avg,
                                                                          hamming_distance.value, iteration_step);
        }
    }

    template<hfl::UnsignedIntegral UintT>
    AvalancheInfo HashAvalancheTest(const hfl::Hash<UintT>& hash, const AvalancheTestParameters& tp,
                                    out::Logger& logger) {
        out::LogDuration log_duration("\t\ttime", logger);
        logger << boost::format("\t%1%: \n") % hash.GetName();

        auto generators = GetGenerators(tp.num_threads, tp.num_keys);
        std::atomic_uint16_t gen_index = 0;
        auto thread_task = [&hash, &tp, &generators, &gen_index] (uint64_t first, uint64_t last) {
            pcg64 rng = generators[gen_index++];
            AvalancheInfo avalanche_info;
            while (first < last) {
                uint64_t number = rng();
                CalculateHammingDistance(avalanche_info, hash, tp, number, ++first);
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

        BOOST_ASSERT_MSG(result.original_pair.hash == hash(result.original_pair.number),
                         "The hash of the original number is not correct");
        BOOST_ASSERT_MSG(result.modified_pair.hash == hash(result.modified_pair.number),
                         "The hash of the original number is not correct");

        logger << result << std::endl;
        return result;
    }


    template<hfl::UnsignedIntegral UintT>
    void AvalancheTest(const std::vector<hfl::Hash<UintT>>& hashes, const AvalancheTestParameters& tp,
                       out::Logger& logger) {
        out::StartAndEndLogBitsTest log(logger, tp.hash_bits);

        auto out_json = out::GetAvalancheTestJson(tp, logger);
        boost::json::object avalanche_statistics;
        for (const auto& hash : hashes) {
            AvalancheInfo avalanche_info = HashAvalancheTest(hash, tp, logger);
            avalanche_statistics[hash.GetName()] = out::AvalancheInfoToJson(avalanche_info);
        }

        out_json.obj["Avalanche effect"] = avalanche_statistics;
        out_json.out << out_json.obj;
    }
}

#endif //THESIS_WORK_AVALANCHE_TESTS_H
