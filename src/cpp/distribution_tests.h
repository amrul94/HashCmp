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

using Bucket = std::uint16_t;

void PrintReports(const std::vector<Bucket>& buckets, const DistTestParameters& cp, const std::string& hash_name,
                  ReportsRoot& reports_root);

template<typename HashStruct>
void HashDistTest(const HashStruct& hs, const DistTestParameters& dtp, ReportsRoot& reports_root) {
    LOG_DURATION_STREAM(hs.hash_name, std::cout);

    std::vector<Bucket> buckets(dtp.num_buckets, 0);
    const auto max_bucket = std::numeric_limits<Bucket>::max();
    /*std::cerr << boost::format("num_keys = %1%\nnum_buckets = %2%\ndivisor = %3%\n")
                                % dtp.num_keys % dtp.num_buckets % dtp.divisor;*/
    std::mutex mutex;
    auto lambda = [&mutex, &hs, &dtp, &buckets, max_bucket](uint64_t start, uint64_t end) {
        //const auto log_thread_id = boost::format("thread %1%") % std::this_thread::get_id();
        //LOG_DURATION_STREAM(log_thread_id.str(), std::cout);
        for (uint64_t number = start; number < end; ++number) {
            const uint64_t hash = hs.hash_function(number);
            const uint64_t modified = ModifyHash(dtp, hash);

            std::lock_guard guard{mutex};
            /*std::cerr << boost::format("hash: %1%\n") % hash;
            std::cerr << boost::format("modified: %1%\n") % modified;
            std::cerr << boost::format("number: %1%\n") % number;*/
            Bucket& bucket = buckets[modified];
            bucket = (bucket != max_bucket) ? (bucket + 1) : bucket;
        }
    };

    uint64_t start = 0;
    uint64_t step = dtp.num_keys / dtp.num_threads;
    std::vector<std::thread> threads(dtp.num_threads - 1);
    //uint64_t step = dtp.num_keys / 1;
    //std::vector<std::thread> threads;

    for (auto& t : threads) {
        t = std::thread{lambda, start, start + step};
        start += step;
    }
    lambda(start, dtp.num_keys);

    for (auto& t : threads) {
        t.join();
    }

    PrintReports(buckets, dtp, hs.hash_name, reports_root);

}

template<typename HashStructs>
void DistributionTest(const HashStructs& funcs, const DistTestParameters& dtp, ReportsRoot& reports_root) {
    std::cout << "start " << dtp.hash_bits << " bits" << std::endl;
    for (const auto& current_hash : funcs) {
        HashDistTest(current_hash, dtp, reports_root);
    }
    std::cout << "end " << dtp.hash_bits << " bits" << std::endl << std::endl;
}

void RunDistTestNormal(size_t num_threads, ReportsRoot& reports_root);
void RunDistTestWithBins(size_t num_threads, ReportsRoot& reports_root);

void RunDistributionTests(ReportsRoot& reports_root);

#endif //THESIS_WORK_CHECK_DISTRIBUTION_H
