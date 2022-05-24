#include "generated_tests.h"

namespace tests {
    namespace out {
        // Формирует json-файл, в который будет сохранена информация с теста хеш функции
        // на устойчивости к коллизиям
        OutputJson GetGenTestJson(const GenBlocksParameters& gbp, out::Logger& logger) {
            using namespace std::literals;
            const std::filesystem::path gen_tests_dir = "Generated blocks tests";
            const std::filesystem::path block_size_dir = std::to_string(gbp.words_length);
            const auto block_size_path = logger.GetLogDirPath() / gen_tests_dir / block_size_dir;
            std::filesystem::create_directories(block_size_path);

            const std::filesystem::path report_name = std::to_string(gbp.hash_bits) + " bits (" + TestFlagToString(gbp.mode)
                                                      + " "s + std::to_string(gbp.mask_bits) + " bits).json"s;
            const std::filesystem::path report_path = block_size_path / report_name;

            std::ofstream out(report_path);
            assert(out);
            boost::json::object obj;
            obj["Test name"] = "Test With Random Words";
            obj["Mode"] = TestFlagToString(gbp.mode);
            obj["Bits"] = gbp.hash_bits;
            obj["Mask"] = gbp.mask_bits;
            obj["Number of keys"] = gbp.num_keys;
            return OutputJson{std::move(obj), std::move(out)};
        }
    }

    // Запуск тестирования устойчивости к коллизиям хеш функций одной битности
    template<hfl::UnsignedIntegral UintT>
    void RunTestWithGeneratedBlocksImpl(uint16_t hash_bits, uint16_t test_bits, uint16_t shift, uint16_t num_threads,
                                        uint16_t words_length, TestFlag mode, out::Logger& logger) {
        const uint64_t num_words = (1ull << shift);
        const GenBlocksParameters parameters {hash_bits, test_bits, num_threads, num_words, words_length, mode};
        const auto hashes = hfl::BuildHashes<UintT>();
        TestWithGeneratedBlocks(hashes, parameters, logger);
    }

    // Запуск тестирования устойчивости к коллизиям всех хеш функций
    void RunTestWithGeneratedBlocks(uint16_t words_length, out::Logger& logger) {
        using hfl::uint24_t;
        using hfl::uint48_t;

        const std::string test_name = "GENERATED BLOCKS (length = " + std::to_string(words_length) + ")";
        out::StartAndEndLogTest start_and_end_log(logger, test_name);
        const uint16_t num_threads = GetNumThreads();

        RunTestWithGeneratedBlocksImpl<uint16_t>(bits16, bits16, bits16, num_threads, words_length,
                                                 TestFlag::NORMAL, logger);
        RunTestWithGeneratedBlocksImpl<uint24_t>(bits24, bits24, bits24, num_threads, words_length,
                                                 TestFlag::NORMAL, logger);
        RunTestWithGeneratedBlocksImpl<uint32_t>(bits32, bits32, bits24, num_threads, words_length,
                                                 TestFlag::NORMAL, logger);
        RunTestWithGeneratedBlocksImpl<uint48_t>(bits48, bits32, bits24, num_threads, words_length,
                                                 TestFlag::MASK, logger);
        RunTestWithGeneratedBlocksImpl<uint64_t>(bits64, bits32, bits24, num_threads, words_length,
                                                 TestFlag::MASK, logger);
    }
}