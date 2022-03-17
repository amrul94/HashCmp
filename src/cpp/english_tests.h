#ifndef THESISWORK_ENGLISH_TESTS_H
#define THESISWORK_ENGLISH_TESTS_H

#include <map>
#include <vector>

#include <boost/format.hpp>
#include <boost/json.hpp>

#include "log_duration.h"
#include "test_parameters.h"

namespace tests {
    namespace detail {
        OutputJson GetEnglishTestJson(const TestParameters& tp, size_t num_words, ReportsRoot& reports_root);
    }

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
        LOG_DURATION_STREAM(hs.name, reports_root.logger);
        std::map<uint64_t, uint64_t> hashes;
        for (const std::string& word : words) {
            const uint64_t hash = hs.hasher(word);
            const uint64_t modify = ModifyHash(tp, hash);
            ++hashes[modify];
        }
        return CountCollisions(hashes);
    }

    template <typename HashStruct>
    void TestWithEnglishWords(const std::vector<HashStruct>& hashes, const std::vector<std::string>& words,
                              const TestParameters& tp, ReportsRoot& reports_root) {
        reports_root.logger << boost::format("--- START TEST WITH %1% BITS HASHES ---\n") % tp.hash_bits;

        auto out_json = detail::GetEnglishTestJson(tp, words.size(), reports_root);
        boost::json::object collisions;
        for (const auto& hs : hashes) {
            collisions[hs.name] = HashTestWithEngWords(hs, words, tp, reports_root);
        }
        out_json.obj["Collisions"] = collisions;
        out_json.out << out_json.obj;

        reports_root.logger << boost::format("--- END TEST WITH %1% BITS HASHES ---\n\n") % tp.hash_bits;
    }

}

#endif //THESISWORK_ENGLISH_TESTS_H
