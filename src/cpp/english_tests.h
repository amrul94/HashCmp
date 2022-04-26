#ifndef THESIS_WORK_ENGLISH_TESTS_H
#define THESIS_WORK_ENGLISH_TESTS_H

#include <map>
#include <vector>

#include <boost/format.hpp>
#include <boost/json.hpp>

#include "timers.h"
#include "output.h"
#include "test_parameters.h"
#include "hashes.h"

namespace tests {
    namespace out {
        OutputJson GetEnglishTestJson(const TestParameters& tp, size_t num_words, out::Logger& logger);
    }

    uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hashes);

    std::vector<std::string> ParseWords(const std::filesystem::path& file_name);

    template <hfl::UnsignedIntegral UintT>
    uint64_t HashTestWithEngWords(const hfl::Hash<UintT>& hasher, const std::vector<std::string>& words,
                                  const TestParameters& tp, out::Logger& logger);

    template <hfl::UnsignedIntegral UintT>
    void TestWithEnglishWords(const std::vector<hfl::Hash<UintT>>& hashes, const std::vector<std::string>& words,
                              const TestParameters& tp, out::Logger& logger);

    void RunTestWithEnglishWords(out::Logger& logger);


// =====================================================

    template <hfl::UnsignedIntegral UintT>
    uint64_t HashTestWithEngWords(const hfl::Hash<UintT>& hash, const std::vector<std::string>& words,
                                  const TestParameters& tp, out::Logger& logger) {
        out::LogDuration log_duration("\t\ttime", logger);
        logger << '\t' + hash.GetName() + ':' << std::endl;

        std::map<uint64_t, uint64_t> hash_values;
        for (const std::string& word : words) {
            const uint64_t hash_value = hash(word);
            ++hash_values[hash_value];
        }
        const uint32_t num_collisions = CountCollisions(hash_values);
        std::cout << "\t\tcollisions: " << num_collisions << std::endl;
        return num_collisions;
    }

    template <hfl::UnsignedIntegral UintT>
    void TestWithEnglishWords(const std::vector<hfl::Hash<UintT>>& hashes, const std::vector<std::string>& words,
                              const TestParameters& tp, out::Logger& logger) {
        out::StartAndEndLogBitsTest printer(logger, tp.hash_bits);

        auto out_json = out::GetEnglishTestJson(tp, words.size(), logger);
        boost::json::object collisions;
        for (const auto& hash : hashes) {
            collisions[hash.GetName()] = HashTestWithEngWords(hash, words, tp, logger);
        }
        out_json.obj["Collisions"] = collisions;
        out_json.out << out_json.obj;
    }

}

#endif //THESIS_WORK_ENGLISH_TESTS_H
