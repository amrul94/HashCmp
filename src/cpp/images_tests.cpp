#include "images_tests.h"

#include <boost/assert.hpp>

namespace tests {
    namespace out {
        // Формирует json-файл, в который будет сохранена информация с теста хеш функции
        // на устойчивости к коллизиям
        OutputJson GetImagesTestJson(const TestParameters& parameters, out::Logger& logger) {
            const std::string test_name = "Images tests";
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
            return OutputJson{std::move(obj), std::move(out)};
        }
    }

    // Запуск тестирования устойчивости к коллизиям хеш функций одной битности
    template <hfl::UnsignedIntegral UintT>
    void RunImagesTestsImpl(uint16_t bits, uint16_t num_threads, out::Logger& logger) {
        const auto hashes = hfl::BuildHashes<UintT>();
        const TestParameters parameters{bits, num_threads};
        TestWithImages(hashes, parameters, logger);
    }

    // Запуск тестирования устойчивости к коллизиям всех хеш функций
    void RunImagesTests(out::Logger& logger) {
        out::StartAndEndLogTest start_and_end_log(logger, "IMAGES");
        const uint16_t num_threads = GetNumThreads();
        RunImagesTestsImpl<uint16_t>(bits16, num_threads, logger);
        RunImagesTestsImpl<hfl::uint24_t>(bits24, num_threads, logger);
        RunImagesTestsImpl<uint32_t>(bits32, num_threads, logger);
        RunImagesTestsImpl<hfl::uint48_t>(bits48, num_threads, logger);
        RunImagesTestsImpl<uint64_t>(bits64, num_threads, logger);
    }

}

