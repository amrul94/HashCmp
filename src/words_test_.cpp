#include <iostream>
#include <map>
#include <thread>

#include "count_collisions.h"
#include "generators.h"
#include "log_duration.h"
#include "hashes.h"

#include <boost/multiprecision/cpp_int.hpp>


#include <cstdint>
using uint24_t = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<24, 24,
                                                    boost::multiprecision::unsigned_magnitude,
                                                    boost::multiprecision::unchecked, void>>;
#define __INT24_TYPE__

//using uint24_t = boost::multiprecision::cpp_int_backend<24, 24, boost::multiprecision::cpp_integer_type::signed_magnitude.




struct WordsParameters {
    uint16_t hash_bits{};
    uint64_t words_count{};
    uint32_t min_length{};
    uint32_t max_length{};



    explicit WordsParameters(uint16_t hash_bits, uint64_t word_counts, uint32_t min_length, uint32_t max_length)
            : hash_bits(hash_bits)
            , words_count(word_counts)
            , min_length(min_length)
            , max_length(max_length) {
    }
};


/*
string CurrentTime() {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::string ts = std::ctime(&t);
    ts.resize(ts.size() - 1);
    ts.replace(ts.find(':'), 1, ".");
    ts.replace(ts.find(':'), 1, ".");
    return ts;
}*/


void PrintReports(const std::vector<uint32_t>& buckets, int divisor, const std::string& hash_name,
                  int hash_bits) {
    std::string file_name = "reports/Check Dist/"s + to_string(hash_bits) + "/"s +
                            hash_name + " "s + CurrentTime() + ".txt"s;

    std::ofstream hash_out(file_name);

    for (size_t i = 0; i < buckets.size(); ++i) {
        hash_out << i * divisor << "\t" << buckets[i] << endl;
    }
}



int main() {
    uint24_t u24 = 3;
    cout << u24 << endl;
    cout << numeric_limits<uint24_t>::max() << endl;

    std::random_device rd;
    std::mt19937 base_generator{rd()};
    auto word_counts = static_cast<uint64_t>(pow(2, 16));
    uint32_t min_length = FOUR_KILOBYTES;
    uint32_t max_length = min_length;

    std::mt19937 generator16 = base_generator;
    const WordsParameters wp16 {16, word_counts, min_length, max_length};
    const std::vector<hfl::Hash16Struct> hashes16 = hfl::Build16bitsHashes();
    jthread jt16 {TestsWithWords<std::vector<hfl::Hash16Struct>>, hashes16, wp16, generator16};
    //std::cout << "generator" << " -> " << GenerateRandomWord(generator16, min_length, max_length) << endl;
    //CheckDist(hashes16, cp16);

    std::mt19937 generator32 = base_generator;
    const WordsParameters wp32 {32, word_counts, min_length, max_length};
    const std::vector<hfl::Hash32Struct> hashes32 = hfl::Build32bitsHashes();
    jthread jt32 {TestsWithWords<std::vector<hfl::Hash32Struct>>, hashes32, wp32, generator32};
    //std::cout << "generator" << " -> " << GenerateRandomWord(generator32, min_length, max_length) << endl;
    //CheckDist(hashes32, cp32);

    std::mt19937 generator64 = base_generator;
    const WordsParameters wp64 {64, word_counts, min_length, max_length};
    const std::vector<hfl::Hash64Struct> hashes64 = hfl::Build64bitsHashes();
    jthread jt64 {TestsWithWords<std::vector<hfl::Hash64Struct>>, hashes64, wp64, generator64};
    //std::cout << "generator" << " -> " << GenerateRandomWord(generator64, min_length, max_length) << endl;
    //CheckDist(hashes64, cp64);

}
