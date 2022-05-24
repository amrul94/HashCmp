#include "english_tests.h"

#include <boost/assert.hpp>

namespace tests {
    namespace out {
        OutputJson GetEnglishTestJson(const TestParameters& parameters, size_t num_words, out::Logger& logger) {
                const std::string test_name = "English words tests";
                const std::filesystem::path report_test_dir = test_name;
                const auto report_test_path = logger.GetLogDirPath() / report_test_dir;
                std::filesystem::create_directories(report_test_path);

                const std::filesystem::path report_name = std::to_string(parameters.hash_bits) + " bits.json";
                const std::filesystem::path out_path = report_test_path / report_name;
                std::ofstream out(out_path);
                BOOST_ASSERT_MSG(out, "Failed to create file");

                boost::json::object obj;
                obj["Test name"] = test_name;
                obj["Bits"] = parameters.hash_bits;
                obj["Number of words"] = num_words;
                return OutputJson{std::move(obj), std::move(out)};
        }
    }

    // Парсинг слов из файла
    std::vector<std::string> ParseWords(const std::filesystem::path& file_name) {
        std::ifstream input(file_name);
        BOOST_ASSERT_MSG(input, "Could not open the file");

        std::vector<std::string> words;
        while (input) {
            std::string str;
            input >> str;
            words.push_back(std::move(str));
        }
        return words;
    }

    // Запуск тестирования устойчивости к коллизиям хеш функций одной битности
    template <hfl::UnsignedIntegral UintT>
    static void RunTestWithEnglishWordsImpl(const std::vector<std::string>& words, out::Logger& logger) {
        const auto hashes = hfl::BuildHashes<UintT>();
        const TestParameters parameters{std::numeric_limits<UintT>::digits};
        TestWithEnglishWords(hashes, words, parameters, logger);
    }

    // Запуск тестирования устойчивости к коллизиям всех хеш функций
    void RunTestWithEnglishWords(out::Logger& logger) {
        out::StartAndEndLogTest start_and_end_log(logger, "ENGLISH WORDS");
        const std::filesystem::path& file_name = "data/english_words.txt";
        const std::vector<std::string> words = ParseWords(file_name);

        RunTestWithEnglishWordsImpl<uint16_t>(words, logger);
        RunTestWithEnglishWordsImpl<hfl::uint24_t>(words, logger);
        RunTestWithEnglishWordsImpl<uint32_t>(words, logger);
        RunTestWithEnglishWordsImpl<hfl::uint48_t>(words, logger);
        RunTestWithEnglishWordsImpl<uint64_t>(words, logger);
    }
}
