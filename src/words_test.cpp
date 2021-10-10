#include <iostream>
#include <map>
#include <thread>

#include "count_collisions.h"
#include "generators.h"
#include "log_duration.h"
#include "hashes.h"

constexpr int GIGABYTE = 1'073'741'824;
constexpr int KILOBYTE = 1024;
constexpr int FOUR_KILOBYTES = KILOBYTE * 4;
constexpr int EIGHT_KILOBYTES = KILOBYTE * 8;


struct WordsParameters {
    uint16_t hash_bits{};
    uint64_t word_counts{};
    uint32_t min_length{};
    uint32_t max_length{};



    explicit WordsParameters(uint16_t hash_bits, uint64_t word_counts, uint32_t min_length, uint32_t max_length)
            : hash_bits(hash_bits)
            , word_counts(word_counts)
            , min_length(min_length)
            , max_length(max_length) {
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


void PrintReports(const std::vector<uint32_t>& buckets, int divisor, const std::string& hash_name,
                  int hash_bits) {
    std::string file_name = "reports/Check Dist/"s + to_string(hash_bits) + "/"s +
                            hash_name + " "s + CurrentTime() + ".txt"s;

    std::ofstream hash_out(file_name);

    for (size_t i = 0; i < buckets.size(); ++i) {
        hash_out << i * divisor << "\t" << buckets[i] << endl;
    }
}


template<typename HashStruct>
void WordsTestForHash(const HashStruct& hash_struct, const WordsParameters& wp, ostream& out, std::mt19937 generator) {
    LOG_DURATION_STREAM(hash_struct.hash_name, cout);



    std::map<uint64_t, uint32_t> hashes;

    for (uint64_t i = 0; i < wp.word_counts; ++i) {
        std::string str = GenerateRandomWord(generator, wp.min_length, wp.max_length);
        auto hash = hash_struct.hash_function(str);
        ++hashes[hash];
    }



    out << hash_struct.hash_name << "\t" << CountCollisions(hashes)<< endl;
}

template<typename HashVector>
void TestsWithWords(const HashVector& hash_vec, const WordsParameters& wp, std::mt19937 generator) {
    std::cout << "start "  << wp.hash_bits << " bits" << endl;
    string file_name = "reports/Words Test/"s + to_string(wp.hash_bits) + " bits "s + CurrentTime() + ".txt"s;
    std::ofstream out(file_name);
    for (const auto& current_hash : hash_vec) {
        std::mt19937 copy_gen = generator;
        //std::cout << "generator" << " -> " << GenerateRandomWord(copy_gen, wp.min_length, wp.max_length) << endl;
        WordsTestForHash(current_hash, wp, out, copy_gen);
    }
    std::cout << "end " << wp.hash_bits << " bits" << endl << endl;
}

int main() {
    std::random_device rd;
    std::mt19937 base_generator{rd()};
    uint64_t word_counts = std::numeric_limits<uint32_t>::max() + 1ull;
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
