#ifndef THESIS_WORK_CHECK_DISTRIBUTION_H
#define THESIS_WORK_CHECK_DISTRIBUTION_H

#include <atomic>
#include <iostream>
#include <execution>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <boost/format.hpp>

#include "hashes.h"
#include "test_parameters.h"
#include "log_duration.h"

namespace tests {

    using Bucket = std::uint16_t;

    void PrintReports(const std::vector<Bucket>& buckets, const DistTestParameters& dtp, const std::string& hash_name,
                      ReportsRoot& reports_root);

    template<typename HashStruct>
    void HashDistTest(const HashStruct& hs, const DistTestParameters& dtp, ReportsRoot& reports_root);

    template<typename HashStructs>
    void DistributionTest(const HashStructs& funcs, const DistTestParameters& dtp, ReportsRoot& reports_root);

    void RunDistTestNormal(size_t num_threads, ReportsRoot& reports_root);
    void RunDistTestWithBins(size_t num_threads, ReportsRoot& reports_root);

    void RunDistributionTests(ReportsRoot& reports_root);


// ==================================================

    template<typename HashStruct>
    void HashDistTest(const HashStruct& hs, const DistTestParameters& dtp, ReportsRoot& reports_root) {
        LOG_DURATION_STREAM(hs.name, reports_root.logger);

        // Выделить buckets, max_bucket и мьютекс в отдельный класс (например, Distributor).
        // При этом добавление нового элемента вынести в метод класса AddHash
        std::vector<Bucket> buckets(dtp.num_buckets, 0);
        const auto max_bucket = std::numeric_limits<Bucket>::max();

        std::mutex mutex;
        auto lambda = [&mutex, &hs, &dtp, &buckets, max_bucket](uint64_t start, uint64_t end) {
            // Создать строку длины sizeof(number), записывать числа в эту строку
            // и хешировать эту строку. Это должно избавить от лишних выделений памяти
            for (uint64_t number = start; number < end; ++number) {
                const uint64_t hash = hs.hasher(number);
                const uint64_t modified = ModifyHash(dtp, hash);
                std::lock_guard guard{mutex};
                Bucket& bucket = buckets[modified];
                const uint64_t tmp = (bucket != max_bucket) ? 1 : 0;
                bucket += tmp;
            }
        };

        uint64_t start = 0;
        uint64_t step = dtp.num_keys / dtp.num_threads;
        std::vector<std::thread> threads(dtp.num_threads - 1);

        for (auto& t : threads) {
            t = std::thread{lambda, start, start + step};
            start += step;
        }
        lambda(start, dtp.num_keys);

        for (auto& t : threads) {
            t.join();
        }

        PrintReports(buckets, dtp, hs.name, reports_root);

    }

    template<typename HashStructs>
    void DistributionTest(const HashStructs& funcs, const DistTestParameters& dtp, ReportsRoot& reports_root) {
        reports_root.logger << "start " << dtp.hash_bits << " bits" << std::endl;
        for (const auto& current_hash : funcs) {
            HashDistTest(current_hash, dtp, reports_root);
        }
        reports_root.logger << "end " << dtp.hash_bits << " bits" << std::endl << std::endl;
    }
}

#endif //THESIS_WORK_CHECK_DISTRIBUTION_H
