#ifndef THESISWORK_ENGLISH_TESTS_H
#define THESISWORK_ENGLISH_TESTS_H

#include <map>
#include <vector>

#include <boost/format.hpp>
#include <boost/json.hpp>

#include "log_duration.h"
#include "test_parameters.h"

uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hashes);

std::vector<std::string> ParseWords(const std::filesystem::path& file_name);

template <typename HashStruct>
uint64_t HashTestWithEngWords(const HashStruct& hs, const std::vector<std::string>& words,
                              const TestParameters& tp, ReportsRoot& reports_root);

template <typename HashStruct>
void TestWithEnglishWords(const std::vector<HashStruct>& hashes, const std::vector<std::string>& words,
                          const TestParameters& tp, ReportsRoot& reports_root);

void RunTestWithEnglishWords(ReportsRoot& reports_root);


// =====================================================

template <typename HashStruct>
uint64_t HashTestWithEngWords(const HashStruct& hs, const std::vector<std::string>& words,
                              const TestParameters& tp, ReportsRoot& reports_root) {
    LOG_DURATION_STREAM(hs.hash_name, reports_root.logger);
    std::map<uint64_t, uint64_t> hashes;
    for (const std::string& word : words) {
        const uint64_t hash = hs.hash_function(word);
        const uint64_t modify = ModifyHash(tp, hash);
        ++hashes[modify];
    }
    return CountCollisions(hashes);
}

template <typename HashStruct>
void TestWithEnglishWords(const std::vector<HashStruct>& hashes, const std::vector<std::string>& words,
                          const TestParameters& tp, ReportsRoot& reports_root) {
    reports_root.logger << boost::format("start test with %1% bits hashes\n") % tp.hash_bits;
    const std::filesystem::path report_test_dir = "English words tests";
    const auto report_test_path = reports_root.root_path / report_test_dir;
    std::filesystem::create_directories(report_test_path);

    const std::filesystem::path report_name = std::to_string(tp.hash_bits) + " bits.json";
    const std::filesystem::path out_path = report_test_path / report_name;
    std::ofstream out(out_path);
    assert(out);

    boost::json::object obj;
    obj["Test name"] = "Test With English Words";
    obj["Bits"] = tp.hash_bits;
    obj["Number of words"] = words.size();

    boost::json::object collisions;
    for (const auto& hs : hashes) {
        collisions[hs.hash_name] = HashTestWithEngWords(hs, words, tp, reports_root);
    }
    obj["Collisions"] = collisions;
    out << obj;

    reports_root.logger << boost::format("end test with %1% bits hashes\n\n") % tp.hash_bits;
}

#endif //THESISWORK_ENGLISH_TESTS_H
