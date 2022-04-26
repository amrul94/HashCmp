#include "english_tests.h"
#include "hashes.h"

namespace tests {
    namespace out {
        OutputJson GetEnglishTestJson(const TestParameters& tp, size_t num_words, out::Logger& logger) {
                const std::string test_name = "English words tests";
                const std::filesystem::path report_test_dir = test_name;
                const auto report_test_path = logger.GetLogDirPath() / report_test_dir;
                std::filesystem::create_directories(report_test_path);

                const std::filesystem::path report_name = std::to_string(tp.hash_bits) + " bits.json";
                const std::filesystem::path out_path = report_test_path / report_name;
                std::ofstream out(out_path);
                assert(out);

                boost::json::object obj;
                obj["Test name"] = test_name;
                obj["Bits"] = tp.hash_bits;
                obj["Number of words"] = num_words;
                return OutputJson{std::move(obj), std::move(out)};
        }
    }

    uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hashes) {
        uint64_t collisions = 0;
        for (const auto& [hash, count] : hashes) {
            const uint64_t tmp = (count > 1) ? (count - 1) : 0;
            collisions += tmp;
        }
        return collisions;
    }

    std::vector<std::string> ParseWords(const std::filesystem::path& file_name) {
        std::ifstream input(file_name);
        assert(input);

        std::vector<std::string> words;
        while (input) {
            std::string str;
            input >> str;
            words.push_back(std::move(str));
        }
        return words;
    }

    #define RUN_TEST_WITH_ENG_WORDS_IMPL(WORDS, BITS, ROOT)             \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes();       \
        const TestParameters tp##BITS{BITS};                            \
        TestWithEnglishWords(hashes##BITS, WORDS, tp##BITS, ROOT)       \


    void RunTestWithEnglishWords(out::Logger& logger) {
        out::StartAndEndLogTest start_and_end_log(logger, "ENGLISH WORDS");
        const std::filesystem::path& file_name = "data/english_words.txt";
        const std::vector<std::string> words = ParseWords(file_name);

        RUN_TEST_WITH_ENG_WORDS_IMPL(words, 16, logger);
        RUN_TEST_WITH_ENG_WORDS_IMPL(words, 24, logger);
        RUN_TEST_WITH_ENG_WORDS_IMPL(words, 32, logger);
        RUN_TEST_WITH_ENG_WORDS_IMPL(words, 48, logger);
        RUN_TEST_WITH_ENG_WORDS_IMPL(words, 64, logger);
    }
}
