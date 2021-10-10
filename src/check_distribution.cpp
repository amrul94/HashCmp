#include <iostream>
#include <bitset>

#include <thread>
#include <algorithm>
#include <numeric>
#include <fstream>

#include <chrono>
#include <ranges>

#include "hashes.h"
#include "log_duration.h"



struct CheckParameters {
    uint16_t hash_bits{};
    uint32_t small_buckets_count{};
    uint32_t big_buckets_count{};
    uint32_t max_number{};
    uint64_t small_divisor{};
    uint64_t big_divisor{};

    explicit CheckParameters(uint16_t hash_bits)
        : hash_bits(hash_bits) {
        SetParameters();
    }

private:
    static const uint16_t DIVIDER_FOR_32 = 8;
    static const uint16_t DIVIDER_FOR_64 = DIVIDER_FOR_32 + 32;
    static const uint32_t SBC_32_64 = 1'048'576;
    static const uint32_t BBC_32_64 = 16'777'216;

    [[nodiscard]] static uint64_t GiveDivisor(uint16_t degree) {
        return static_cast<uint64_t>(pow(2, degree));
    }

    void SetParameters() {
        switch (hash_bits) {
            case 16:
                small_divisor = big_divisor = 1;
                max_number = std::numeric_limits<uint16_t>::max();
                small_buckets_count = max_number;
                big_buckets_count = max_number;
                break;
            case 32:
                max_number = std::numeric_limits<uint32_t>::max();
                small_buckets_count = SBC_32_64;
                big_buckets_count = BBC_32_64;
                small_divisor = GiveDivisor(DIVIDER_FOR_32 + 4);
                big_divisor = GiveDivisor(DIVIDER_FOR_32);
                break;
            case 64:
                max_number = std::numeric_limits<uint32_t>::max();
                small_buckets_count = SBC_32_64;
                big_buckets_count = BBC_32_64;
                small_divisor = GiveDivisor(DIVIDER_FOR_64 + 4);
                big_divisor = GiveDivisor(DIVIDER_FOR_64);
            default:
                break;
        }
    }
};



string CurrentTime() {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::string ts = std::ctime(&t);
    ts.resize(ts.size() - 1);
    ts.replace(ts.find(':'), 1, ".");
    ts.replace(ts.find(':'), 1, ".");
    return ts;
}


void PrintReports(const std::vector<uint32_t>& buckets, uint64_t divisor, const std::string& hash_name,
                  uint16_t hash_bits) {
    std::string file_name = "reports/Check Dist/"s + to_string(hash_bits) + "/"s +
                            hash_name + " "s + CurrentTime() + ".txt"s;

    std::ofstream hash_out(file_name);

    for (uint64_t i = 0; i < buckets.size(); ++i) {
        hash_out << i * divisor << "\t" << buckets[i] << endl;
    }
}


template<typename HashStruct>
void CheckDistForHash(const HashStruct& hash_struct, const CheckParameters& cp) {
    LOG_DURATION_STREAM(hash_struct.hash_name, cout);

    //using HashType = decltype(hash_struct.hash_function("string"));
    //using KeyType = HashType;

    std::vector<uint32_t> small_buckets(cp.small_buckets_count, 0);
    std::vector<uint32_t> big_buckets(cp.big_buckets_count, 0);

    for (uint64_t number = 0; number <= cp.max_number; ++number) { // стоит проверить на корректность
        auto hash = hash_struct.hash_function(number);
        ++small_buckets[hash / cp.small_divisor];
        ++big_buckets[hash / cp.big_divisor];
    }

    std::cout << "Run first thread" << endl;
    //PrintReports(small_buckets, cp.small_divisor, hash_struct.hash_name, cp.hash_bits);
    jthread jts {PrintReports, small_buckets, cp.small_divisor, hash_struct.hash_name, cp.hash_bits};
    std::cout << "Sleep start" << endl;
    std::this_thread::sleep_for(std::chrono::seconds (5));
    std::cout << "Sleep end" << endl;
    if (cp.hash_bits != 16) {
        std::cout << "Run second thread" << endl;
        jthread jtb {PrintReports, big_buckets, cp.big_divisor, hash_struct.hash_name, cp.hash_bits};
        //PrintReports(big_buckets, cp.big_divisor, hash_struct.hash_name, cp.hash_bits);
    }
}

template<typename HashVector>
void CheckDist(const HashVector& hash_vec, const CheckParameters& cp) {
    std::cout << "start "  << cp.hash_bits << " bits" << endl;
    for (const auto& current_hash : hash_vec) {
        CheckDistForHash(current_hash, cp);
    }
    std::cout << "end " << cp.hash_bits << " bits" << endl << endl;
}

int main() {
    const CheckParameters cp16 {16};
    const std::vector<hfl::Hash16Struct> hashes16 = hfl::Build16bitsHashes();
    //CheckDist(hashes16, cp16);

    const CheckParameters cp32 {32};
    const std::vector<hfl::Hash32Struct> hashes32 = hfl::Build32bitsHashes();
    jthread jt32 {CheckDist<std::vector<hfl::Hash32Struct>>, hashes32, cp32};
    //CheckDist(hashes32, cp32);

    const CheckParameters cp64 {64};
    const std::vector<hfl::Hash64Struct> hashes64 = hfl::Build64bitsHashes();
    jthread jt64 {CheckDist<std::vector<hfl::Hash64Struct>>, hashes64, cp64};
    //CheckDist(hashes64, cp64);
}
