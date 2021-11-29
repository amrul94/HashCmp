#ifndef THESIS_WORK_CHECK_DISTRIBUTION_H
#define THESIS_WORK_CHECK_DISTRIBUTION_H

#include <iostream>
#include <execution>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "hashes.h"
#include "test_parameters.h"
#include "log_duration.h"

void PrintReports(const std::vector<uint32_t>& buckets, const CheckParameters& cp, const std::string& hash_name,
                  ReportsRoot& reports_root);

template<typename HashStruct>
void HashDistTest(const HashStruct& hs, const CheckParameters& cp, ReportsRoot& reports_root) {
    LOG_DURATION_STREAM(hs.hash_name, reports_root.log);

    std::vector<uint32_t> buckets(cp.buckets_count, 0);
    std::mutex local_mutex;
    auto lambda = [&](uint64_t start, uint64_t end) {
        std::unique_lock guard{local_mutex};
        reports_root.log << "thread " << std::this_thread::get_id() << std::endl;
        guard.unlock();
        for (uint64_t number = start; number < end; ++number) {
            const uint64_t hash = hs.hash_function(number);
            const uint64_t modify = ModifyHash(cp, hash);
            //std::scoped_lock guard(local_mutex);
            guard.lock();
            ++buckets[modify];
            guard.unlock();
        }
    };

    const size_t hardware_threads = std::thread::hardware_concurrency();
    const size_t num_threads = hardware_threads != 0 ? hardware_threads : 1;

    reports_root.log << "Num threads = " << num_threads << std::endl;

    uint64_t start = 0;
    uint64_t step = cp.key_count / num_threads;
    std::vector<std::thread> threads(num_threads - 1);

    for (auto& t : threads) {
        t = std::thread{lambda, start, start + step};
        start += step;
    }
    lambda(start, start + step);

    for (auto& t : threads) {
        t.join();
    }

    reports_root.log << "Hash count: " << std::reduce(std::execution::par, buckets.begin(),  buckets.end(), 0ull)
                     << std::endl;

    PrintReports(buckets, cp, hs.hash_name, reports_root);

}

template<typename HashStructs>
void DistributionTest(const HashStructs& funcs, const CheckParameters& cp, ReportsRoot& reports_root) {
    reports_root.log << "start " << cp.hash_bits << " bits" << std::endl;
    for (const auto& current_hash : funcs) {
        HashDistTest(current_hash, cp, reports_root);
    }
    reports_root.log << "end " << cp.hash_bits << " bits" << std::endl << std::endl;
}

void RunDistTestNormal(ReportsRoot& reports_root);
void RunDistTestWithBins(ReportsRoot& reports_root);
void RunDistTestWithMask(ReportsRoot& reports_root);

void RunDistributionTests(ReportsRoot& reports_root);

#endif //THESIS_WORK_CHECK_DISTRIBUTION_H
