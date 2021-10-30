#ifndef THESIS_WORK_CHECK_DISTRIBUTION_H
#define THESIS_WORK_CHECK_DISTRIBUTION_H

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "hashes.h"
#include "helper_structs.h"
#include "log_duration.h"

#define RUN_THREAD_CHECK_DIST(THREAD_NUMBER, LAMBDA, START, STEP)   \
    std::jthread jt##THREAD_NUMBER{LAMBDA, START, START + STEP};    \
    START += STEP;

void PrintReports(const std::vector<uint32_t>& buckets, const CheckParameters& cp, const std::string& hash_name);

template<typename HashStruct>
void CheckDistForHash(const HashStruct& hs, const CheckParameters& cp, std::ostream& log = std::cout) {
    LOG_DURATION_STREAM(hs.hash_name, log);

    std::vector<uint32_t> buckets(cp.buckets_count, 0);
    std::mutex mtx;
    auto lambda = [&hs, &buckets, &mtx, &cp, &log](uint64_t start, uint64_t end) {
        for (uint64_t number = start; number < end; ++number) { // стоит проверить на корректность
            const uint64_t hash = hs.hash_function(number);
            const uint64_t modify = ModifyHash(cp, hash);
            std::scoped_lock guard(mtx);
            ++buckets[modify];
        }
    };

    {
        LOG_DURATION_STREAM(hs.hash_name + " loops", log);
        uint64_t start = 0;
        uint64_t step = cp.key_count / 4;
        RUN_THREAD_CHECK_DIST(1, lambda, start, step);
        RUN_THREAD_CHECK_DIST(2, lambda, start, step);
        RUN_THREAD_CHECK_DIST(3, lambda, start, step);
        RUN_THREAD_CHECK_DIST(4, lambda, start, step);
    }

    log << "Hash count: " << std::accumulate(buckets.begin(),  buckets.end(), 0ull) << std::endl;

    PrintReports(buckets, cp, hs.hash_name);

}

template<typename HashStructs>
void CheckDist(const HashStructs& hash_vec, const CheckParameters& cp, std::ostream& log = std::cout) {
    log << "start " << cp.hash_bits << " bits" << std::endl;
    for (const auto& current_hash : hash_vec) {
        CheckDistForHash(current_hash, cp, log);
    }
    log << "end " << cp.hash_bits << " bits" << std::endl << std::endl;
}

void RunCheckDistNormalMode(std::ostream& log = std::cout);
void RunCheckDistBinsMode(std::ostream& log = std::cout);
void RunCheckDistMaskMode(std::ostream& log = std::cout);

void RunCheckDist();



#endif //THESIS_WORK_CHECK_DISTRIBUTION_H
