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


class Buckets {
public:
    explicit Buckets(const CheckParameters& cp);
    void AddHash(uint64_t hash);

    [[nodiscard]] const std::vector<uint32_t>& GetSmallBuckets() const;
    [[nodiscard]] const std::vector<uint32_t>& GetBigBuckets() const;

private:
    const CheckParameters& cp_;
    std::vector<uint32_t> small_buckets_;
    std::vector<uint32_t> big_buckets_;
    std::mutex mutex_{};
};


void PrintReports(const std::vector<uint32_t>& buckets, uint64_t divisor, const std::string& hash_name,
                  uint16_t hash_bits);

template<typename HashFunc>
void CheckDistForHashLoop(const HashFunc& hf, Buckets& buckets, uint64_t start, uint64_t end) {
    //std::cout << "start loop ("  << start << " - " << end << ")" << std::endl;
    for (uint64_t number = start; number < end; ++number) { // стоит проверить на корректность
        uint64_t hash = hf(number);
        buckets.AddHash(hash);
    }
    //std::cout << "end loop ("  << start << " - " << end << ")" << std::endl;
}


template<typename HashStruct>
void CheckDistForHash(const HashStruct& hs, const CheckParameters& cp, std::ostream& log = std::cout) {
    LOG_DURATION_STREAM(hs.hash_name, log);

    Buckets buckets{cp};
    std::mutex mtx;
    auto lambda = [&hs, &buckets, &mtx, &log](uint64_t start, uint64_t end) {
        log << "start loop ("  << start << " - " << end << ")" << std::endl;
        for (uint64_t number = start; number < end; ++number) { // стоит проверить на корректность
            uint64_t hash = hs.hash_function(number);
            {
                //std::lock_guard guard(mtx);
                //mtx.lock();
                std::scoped_lock guard(mtx);
                buckets.AddHash(hash);
                //mtx.unlock();
            }

        }
        log << "end loop ("  << start << " - " << end << ")" << std::endl;
    };

    {
        LOG_DURATION_STREAM(hs.hash_name + " loops", log);
        uint64_t start = 0;
        uint64_t step = cp.key_counts / 4;
        RUN_THREAD_CHECK_DIST(1, lambda, start, step);
        RUN_THREAD_CHECK_DIST(2, lambda, start, step);
        RUN_THREAD_CHECK_DIST(3, lambda, start, step);
        RUN_THREAD_CHECK_DIST(4, lambda, start, step);
    }


    //CheckDistForHashLoop(hs.hash_function, buckets, 0, cp.key_counts);
    const auto& small_buckets = buckets.GetSmallBuckets();
    const auto& big_buckets = buckets.GetBigBuckets();

    log << "Hash count: " << std::accumulate(small_buckets.begin(),  small_buckets.end(), 0ull) << std::endl;

    if (cp.hash_bits != 16 && cp.hash_bits != 24) {
        std::jthread jts {PrintReports, small_buckets, cp.small_divisor, hs.hash_name, cp.hash_bits};
        log << "Run first thread " << hs.hash_name << endl;
        std::this_thread::sleep_for(std::chrono::seconds (5));
        log << "Run second thread " << hs.hash_name << endl;
        std::jthread jtb {PrintReports, big_buckets, cp.big_divisor, hs.hash_name, cp.hash_bits};
    } else {
        PrintReports(small_buckets, cp.small_divisor, hs.hash_name, cp.hash_bits);
    }

}

template<typename HashStructs>
void CheckDist(const HashStructs& hash_vec, const CheckParameters& cp, std::ostream& log = std::cout) {
    log << "start "  << cp.hash_bits << " bits" << std::endl;
    for (const auto& current_hash : hash_vec) {
        CheckDistForHash(current_hash, cp, log);
    }
    log << "end " << cp.hash_bits << " bits" << std::endl << std::endl;
}

template<typename HashStructs>
void RunCheckDistImpl(const HashStructs& hash_vec, uint16_t hash_bits, std::ostream& log = std::cout) {
    const CheckParameters cp{hash_bits};
    CheckDist(hash_vec, cp, log);
}

void RunCheckDist();



#endif //THESIS_WORK_CHECK_DISTRIBUTION_H
