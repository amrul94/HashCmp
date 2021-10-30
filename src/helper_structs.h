#ifndef THESIS_WORK_HELPER_STRUCTS_H
#define THESIS_WORK_HELPER_STRUCTS_H

#include <cmath>
#include <cstdint>

#include "hash_wrappers.h"

enum class ModeFlag {
    NORMAL, // Честный подсчет 16 и 24-битных хешей
    BINS,   // Подсчет по бинам 32 и 64-битных хешей
    MASK    // Подсчет с 24-битной маской 32 и 64-битных хешей
};

std::string ModeToString(ModeFlag mode);

uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift = 0);

struct TestParameters {
    uint16_t hash_bits{};   // Количество битов, которые выдает хеш-функция
    uint16_t test_bits{};   // Количество битов для тестов (иногда задается маской)
    uint64_t key_count{};   // Количество входных данных для хешей
    ModeFlag mode;              // Тип подсчета

    TestParameters(uint16_t hash_bits, uint16_t test_bits, ModeFlag mode);
    TestParameters(uint16_t hash_bits, uint16_t test_bits, uint64_t key_counts, ModeFlag mode);
    [[nodiscard]] static uint64_t GiveDivisor(uint16_t degree);
    virtual ~TestParameters() = default;
};

struct CheckParameters : TestParameters{
    uint32_t buckets_count{};   // Количество счетчиков
    uint64_t divisor = 1;       // Делитель. Нужен, когда в одном счетчике много хешей

    explicit CheckParameters(uint16_t hash_bits, uint16_t test_bits, ModeFlag mode);

private:
    static constexpr uint16_t DIVIDER_FOR_32 = 5; // попробовать 5
    static constexpr uint16_t DIVIDER_FOR_64 = DIVIDER_FOR_32 + 32;
    static constexpr uint32_t MAX_BINS_COUNT = 134'217'728; // 2^27
    static constexpr hfl::uint24_t MASK_24 = -1;

private:

    void SetParameters();
    void SetNormalMode();
    void SetBinsMode();
    void SetMaskMode();
};

struct WordsParameters : TestParameters {
    uint32_t words_length{};

    explicit WordsParameters(uint16_t hash_bits, uint16_t test_bits, uint64_t word_counts, uint32_t length, ModeFlag mode);
};

uint64_t ModifyHash(const TestParameters& tp, uint64_t hash);






#endif //THESIS_WORK_HELPER_STRUCTS_H
