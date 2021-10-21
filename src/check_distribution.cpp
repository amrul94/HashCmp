#include "check_distribution.h"

Buckets::Buckets(const CheckParameters& cp)
    : cp_(cp)
    , small_buckets_(cp.small_buckets_count, 0)
    , big_buckets_(cp.big_buckets_count, 0) {
}

void Buckets::AddHash(uint64_t hash) {
    //std::lock_guard<std::mutex> guard(mutex_);
    //std::scoped_lock<std::mutex> guard(mutex_);
    ++small_buckets_[hash / cp_.small_divisor];
    ++big_buckets_[hash / cp_.big_divisor];
}

const std::vector<uint32_t>& Buckets::GetSmallBuckets() const {
    return small_buckets_;
}

const std::vector<uint32_t>& Buckets::GetBigBuckets() const {
    return big_buckets_;
}


void PrintReports(const std::vector<uint32_t>& buckets, uint64_t divisor, const std::string& hash_name,
                  uint16_t hash_bits) {
    using namespace std::literals;
    std::string file_name = "reports/Check Dist/"s + std::to_string(hash_bits) + "/"s +
                            hash_name + " "s + CurrentTime() + ".txt"s;

    std::ofstream hash_out(file_name);

    for (uint64_t i = 0; i < buckets.size(); ++i) {
        hash_out << i * divisor << '\t' << buckets[i] << '\n';
    }
}

void RunCheckDist() {
    std::ofstream log("reports/Log Check Dist " + CurrentTime());

    const auto hashes16 = hfl::Build16bitsHashes();
    RunCheckDistImpl(hashes16, 16, log);

    const auto hashes24 = hfl::Build24bitsHashes();
    RunCheckDistImpl(hashes24, 24, log);

    const auto hashes32 = hfl::Build32bitsHashes();
    //RunCheckDistImpl(hashes32, 32, log);

    const auto hashes64 = hfl::Build64bitsHashes();
    //RunCheckDistImpl(hashes64, 64, log);

}