#ifndef THESIS_WORK_TEST_PARAMETERS_H
#define THESIS_WORK_TEST_PARAMETERS_H

#include <cmath>
#include <cstdint>
#include <thread>

#include "hash_wrappers.h"
#include "timers.h"

namespace tests {
    // Константы, обозначающие число бит
    static inline constexpr uint16_t bits16 = 16;
    static inline constexpr uint16_t bits24 = 24;
    static inline constexpr uint16_t bits32 = 32;
    static inline constexpr uint16_t bits48 = 48;
    static inline constexpr uint16_t bits64 = 64;

    // Флаги тестирования
    enum class TestFlag {
        NORMAL, // Честный подсчет 16 и 24-битных хешей
        BINS,   // Подсчет по бинам 32 и 64-битных хешей
        MASK    // Подсчет с 24-битной маской 32 и 64-битных хешей
    };

    // Конвертор флага в строку
    std::string TestFlagToString(TestFlag mode);

    // Функция свертки 64-битного хеша в хеш меньшей длины
    uint64_t XorFoldMask(uint64_t src, uint16_t mask_bits);

    // Структура, которая задает параметры тестов
    struct TestParameters {
        uint16_t hash_bits;     // Количество битов, которые выдает хеш-функция
        uint16_t num_threads;   // Количество потоков выполнения

        // Конструкторы
        TestParameters() = delete;
        TestParameters(const TestParameters& parameters) = default;
        TestParameters(TestParameters&& parameters) = default;
        explicit TestParameters(uint16_t hash_bits, uint16_t num_threads = 1);

        // Операторы присваивания
        TestParameters& operator=(const TestParameters& parameters) = default;
        TestParameters& operator=(TestParameters&& parameters) = default;

        // Деструктор виртуальный, так как структура является базовой для других
        virtual ~TestParameters() = default;
    };

    // Структура, которая задает параметры тестов на лавинный эффект
    struct AvalancheTestParameters : public TestParameters {
        uint64_t num_keys{};        // Количество входных данных для хешей

        AvalancheTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys);
    };

    // Структура, которая задает расширенные параметры тестов
    struct AdvancedTestParameters : public AvalancheTestParameters {
        const TestFlag mode{};      // Тип подсчета

        AdvancedTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys,
                               TestFlag mode = TestFlag::NORMAL);
    };


    // Структура, которая задает параметры тестов на исследование
    // распределительных свойств хеш-функций
    struct DistTestParameters : AdvancedTestParameters {
        uint64_t num_buckets;       // Количество счетчиков
        uint64_t divisor = 1;       // Делитель. Нужен, когда в одном счетчике много хешей

        DistTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys, uint64_t num_buckets,
                           TestFlag mode);

    private:
        static constexpr uint16_t divider_for_32 = 0;
        static constexpr uint16_t divider_for_48 = divider_for_32 + 16;
        static constexpr uint16_t divider_for_64 = divider_for_32 + 32;
        static constexpr uint64_t num_max_bins = 1ull << 32;

    private:
        // Методы, которые задают параметры тестирования
        void SetParameters();
        void SetNormalMode();
        void SetBinsMode();
        void SetMaskMode();
    };

    // Структура, которая задает параметры тестов со случайными данными
    // на исследование устойчивости к коллизиям хеш-функций
    struct GenBlocksParameters : AdvancedTestParameters {
        const uint16_t mask_bits{};
        uint16_t words_length{};

        GenBlocksParameters(uint16_t hash_bits, uint16_t test_bits, uint16_t num_threads, uint64_t num_words,
                            uint16_t words_length, TestFlag mode);
    };

    // Изменяет хеш в зависимости от флага тестирования
    uint64_t ModifyHash(const AdvancedTestParameters& parameters, uint64_t hash);
}

#endif //THESIS_WORK_TEST_PARAMETERS_H
