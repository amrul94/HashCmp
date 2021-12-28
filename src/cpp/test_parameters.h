#ifndef THESIS_WORK_HELPER_STRUCTS_H
#define THESIS_WORK_HELPER_STRUCTS_H

#include <cmath>
#include <cstdint>
#include <thread>

#include <boost/json.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/tee.hpp>

#include "hash_wrappers.h"

namespace tests {

    namespace detail {
        struct OutputJson {
            boost::json::object obj;
            std::ofstream out;
        };
    }

    struct ReportsRoot {
        typedef boost::iostreams::tee_device<std::ostream, std::ofstream> TeeDevice;
        typedef boost::iostreams::stream<TeeDevice> TeeStream;

        explicit ReportsRoot(const std::filesystem::path& root_path);
        ~ReportsRoot();

        const std::filesystem::path root_path;

    private:
        std::ofstream log_file_;
        TeeDevice output_device_;

    public:
        TeeStream logger;
    };

    enum class TestFlag {
        NORMAL, // Честный подсчет 16 и 24-битных хешей
        BINS,   // Подсчет по бинам 32 и 64-битных хешей
        MASK    // Подсчет с 24-битной маской 32 и 64-битных хешей
    };

    std::string TestFlagToString(TestFlag mode);
    uint64_t XorFoldMask(uint64_t src, uint16_t mask_bits);

    uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift = 0);

    struct TestParameters {
        const uint16_t hash_bits{}; // Количество битов, которые выдает хеш-функция
        const uint16_t test_bits{}; // Количество битов для тестов (иногда задается маской)
        uint64_t num_keys{};       // Количество входных данных для хешей
        const size_t num_threads{}; // Количество потоков выполнения
        const TestFlag mode;        // Тип подсчета


        TestParameters(uint16_t hash_bits, size_t num_threads, TestFlag mode = TestFlag::NORMAL);
        TestParameters(uint16_t hash_bits, uint16_t test_bits, size_t num_threads, TestFlag mode);
        TestParameters(uint16_t hash_bits, uint64_t key_counts, size_t num_threads, TestFlag mode);
        TestParameters(uint16_t hash_bits, uint16_t test_bits, uint64_t key_counts, size_t num_threads, TestFlag mode);
        [[nodiscard]] static uint64_t GiveDivisor(uint16_t degree);
        virtual ~TestParameters() = default;
    };

    struct DistTestParameters : TestParameters{
        uint64_t num_buckets{};   // Количество счетчиков
        uint64_t divisor = 1;       // Делитель. Нужен, когда в одном счетчике много хешей

        DistTestParameters(uint16_t hash_bits, size_t num_threads, TestFlag mode);
        DistTestParameters(uint16_t hash_bits, uint16_t test_bits, size_t num_threads, TestFlag mode);

    private:
        static constexpr uint16_t DIVIDER_FOR_32 = 0; // было ранее 5;
        //static constexpr uint16_t DIVIDER_FOR_32 = 5; // было ранее 5;
        static constexpr uint16_t DIVIDER_FOR_48 = DIVIDER_FOR_32 + 16;
        static constexpr uint16_t DIVIDER_FOR_64 = DIVIDER_FOR_32 + 32;
        static constexpr uint64_t MAX_BINS_COUNT = static_cast<uint64_t>(UINT32_MAX) + 1;
        //static constexpr uint64_t MAX_BINS_COUNT = 134'217'728; // 2^27

    private:
        void SetParameters();
        void SetNormalMode();
        void SetBinsMode();
        void SetMaskMode();
    };

    struct GenBlocksParameters : TestParameters {
        uint32_t words_length{};

        GenBlocksParameters(uint16_t hash_bits, uint64_t num_words, TestFlag mode = TestFlag::NORMAL);
        GenBlocksParameters(uint16_t hash_bits, uint64_t num_words, uint32_t length, size_t num_threads, TestFlag mode);
        GenBlocksParameters(uint16_t hash_bits, uint16_t test_bits, uint64_t num_words, uint32_t length, size_t num_threads, TestFlag mode);
    };

    uint64_t ModifyHash(const TestParameters& tp, uint64_t hash);
}

#endif //THESIS_WORK_HELPER_STRUCTS_H
