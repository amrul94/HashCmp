#ifndef THESIS_WORK_HELPER_STRUCTS_H
#define THESIS_WORK_HELPER_STRUCTS_H

#include <cmath>
#include <cstdint>
#include <thread>

#include "hash_wrappers.h"
#include "timers.h"

namespace tests {
    enum class TestFlag {
        NORMAL, // Честный подсчет 16 и 24-битных хешей
        BINS,   // Подсчет по бинам 32 и 64-битных хешей
        MASK    // Подсчет с 24-битной маской 32 и 64-битных хешей
    };

    std::string TestFlagToString(TestFlag mode);
    uint64_t XorFoldMask(uint64_t src, uint16_t mask_bits);

    uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift = 0);

    struct TestParameters {
        uint16_t hash_bits;     // Количество битов, которые выдает хеш-функция
        uint16_t num_threads;   // Количество потоков выполнения


        TestParameters() = delete;
        TestParameters(const TestParameters& parameters) = default;
        TestParameters(TestParameters&& parameters) = default;

        TestParameters& operator=(const TestParameters& parameters) = default;
        TestParameters& operator=(TestParameters&& parameters) = default;

        explicit TestParameters(uint16_t hash_bits, uint16_t num_threads = 1);

        [[nodiscard]] static uint64_t GiveDivisor(uint16_t degree);
        virtual ~TestParameters() = default;
    };

    struct AvalancheTestParameters : public TestParameters {
        uint64_t num_keys{};        // Количество входных данных для хешей

        AvalancheTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys);
    };

    struct AdvancedTestParameters : public AvalancheTestParameters {
        const TestFlag mode{};      // Тип подсчета

        AdvancedTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys,
                               TestFlag mode = TestFlag::NORMAL);
    };



    struct DistTestParameters : AdvancedTestParameters {
        uint64_t num_buckets;       // Количество счетчиков
        uint64_t divisor = 1;       // Делитель. Нужен, когда в одном счетчике много хешей

        DistTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys, uint64_t num_buckets,
                           TestFlag mode);

    private:
        static constexpr uint16_t divider_for_32 = 0;
        static constexpr uint16_t divider_for_48 = divider_for_32 + 16;
        static constexpr uint16_t divider_for_64 = divider_for_32 + 32;
        static constexpr uint64_t MAX_BINS_COUNT = 1ull << 32; // static_cast<uint64_t>(UINT32_MAX) + 1;

    private:
        void SetParameters();
        void SetNormalMode();
        void SetBinsMode();
        void SetMaskMode();
    };

    struct GenBlocksParameters : AdvancedTestParameters {
        const uint16_t mask_bits{};
        uint16_t words_length{};

        GenBlocksParameters(uint16_t hash_bits, uint16_t test_bits, uint16_t num_threads, uint64_t num_words,
                            uint16_t words_length, TestFlag mode);
    };

    uint64_t ModifyHash(const AdvancedTestParameters& tp, uint64_t hash);
}

#endif //THESIS_WORK_HELPER_STRUCTS_H
