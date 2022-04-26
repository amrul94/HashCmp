#ifndef THESIS_WORK_CHECK_DISTRIBUTION_H
#define THESIS_WORK_CHECK_DISTRIBUTION_H

#include <atomic>
#include <iostream>
#include <execution>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <concepts>

#include <boost/format.hpp>

#include "concurrency.h"
#include "hashes.h"
#include "test_parameters.h"
#include "timers.h"
#include "output.h"

namespace tests {
    using Bucket = std::uint16_t;

    namespace out {
        void SaveReport(const std::vector<std::atomic<Bucket>>& buckets, const DistTestParameters& parameters,
                        const std::string& hash_name, out::Logger& logger);
    }

    class DistributionHashes {
    public:
        static constexpr Bucket max_bucket = std::numeric_limits<Bucket>::max();

        explicit DistributionHashes(size_t num_buckets);
        void AddHash(uint64_t hash_value);
        [[nodiscard]] const std::vector<std::atomic<Bucket>>& GetBuckets() const;

    private:
        std::vector<std::atomic<Bucket>> buckets_;
    };



    template<hfl::UnsignedIntegral UintT>
    void HashDistributionTest(const hfl::Hash<UintT>& hash, const DistTestParameters& parameters, out::Logger& logger);

    template<hfl::UnsignedIntegral UintT>
    void DistributionTest(const std::vector<hfl::Hash<UintT>>& hashes, const DistTestParameters& parameters,
                          out::Logger& logger);

    void RunDistributionTests(out::Logger& logger);

// ==================================================

    template<hfl::UnsignedIntegral UintT>
    void HashDistributionTest(const hfl::Hash<UintT>& hash, const DistTestParameters& parameters, out::Logger& logger) {
        out::LogDuration log_duration("\t\ttime", logger);
        logger << boost::format("\t%1%: \n") % hash.GetName();

        DistributionHashes distribution_hashes{parameters.num_buckets};
        auto lambda = [&hash, &parameters, &distribution_hashes](uint64_t start, uint64_t end) {
            for (uint64_t number = start; number < end; ++number) {
                const uint64_t hash_value = hash(number);
                const uint64_t modified_value = ModifyHash(parameters, hash_value);
                distribution_hashes.AddHash(modified_value);
            }
        };

        ThreadTasks<void> thread_tasks(lambda, parameters.num_threads, parameters.num_keys);
        out::SaveReport(distribution_hashes.GetBuckets(), parameters, hash.GetName(), logger);
    }

    template<hfl::UnsignedIntegral UintT>
    void DistributionTest(const std::vector<hfl::Hash<UintT>>& hashes, const DistTestParameters& parameters,
                          out::Logger& logger) {
        out::StartAndEndLogBitsTest log(logger, parameters.hash_bits);
        for (const auto& hash : hashes) {
            HashDistributionTest(hash, parameters, logger);
        }
    }
}

#endif //THESIS_WORK_CHECK_DISTRIBUTION_H
