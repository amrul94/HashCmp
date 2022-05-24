#include "test_parameters.h"

#include <boost/assert.hpp>

namespace tests {

    // Конвертор флага в строку
    std::string TestFlagToString(TestFlag mode) {
        switch (mode) {
            case TestFlag::NORMAL:
                return "Normal";
            case TestFlag::BINS:
                return "Bins";
            case TestFlag::MASK:
                return "Mask";
            default: {
                BOOST_ASSERT_MSG(false, "Unknown flag");
                return std::string{};
            }
        }
    }

    // Функция свертки 64-битного хеша в хеш меньшей длины
    uint64_t XorFoldMask(uint64_t src, uint16_t mask_bits) {
        const uint64_t mask = (1ull << mask_bits) - 1;
        return (src >> mask_bits) ^ (src & mask);
    }

    // Конструктор TestParameters
    TestParameters::TestParameters(uint16_t hash_bits, uint16_t num_threads)
        : hash_bits(hash_bits)
        , num_threads(num_threads) {
    }

// Конструктор AdvancedTestParameters
    AvalancheTestParameters::AvalancheTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys)
        : TestParameters(hash_bits, num_threads)
        , num_keys(num_keys) {
    }

    // Конструктор AdvancedTestParameters
    AdvancedTestParameters::AdvancedTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys,
                                                   TestFlag mode)
       : AvalancheTestParameters(hash_bits, num_threads, num_keys)
       , mode(mode) {
    }

    // Конструктор DistTestParameters
    DistTestParameters::DistTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys,
                                           uint64_t num_buckets, TestFlag mode)
        : AdvancedTestParameters(hash_bits, num_threads, num_keys, mode)
        , num_buckets(num_buckets) {
        SetParameters();
    }

    // Методы, которые задает параметры тестирования
    // в зависимости от флага
    void DistTestParameters::SetParameters() {
        switch (mode) {
            case TestFlag::NORMAL:
                SetNormalMode();
                break;
            case TestFlag::BINS:
                SetBinsMode();
                break;
            case TestFlag::MASK:
                SetMaskMode();
                break;
            default:
                BOOST_ASSERT_MSG(false, "Unknown flag");
                break;
        }
    }

    // Задает параметры тестирования с флагом NORMAL
    void DistTestParameters::SetNormalMode() {
        num_keys = 1ull << hash_bits; // static_cast<uint64_t>(std::pow(2, hash_bits));
        num_buckets = num_keys;
    }

    // Задает параметры тестирования с флагом BINS
    void DistTestParameters::SetBinsMode() {
        const uint64_t shift = 32;
        num_keys = 1ull << shift; // static_cast<uint64_t>(std::pow(2, 32));
        num_buckets = num_max_bins;
        switch (hash_bits) {
            case bits32:
                divisor = 1ull << divider_for_32;
                break;
            case bits48:
                divisor = 1ull << divider_for_48;
                break;
            case bits64:
                divisor = 1ull << divider_for_64;
                break;
            default:
                BOOST_ASSERT_MSG(false, "Incorrect hash_bits");
                break;
        }
    }

    // Задает параметры тестирования с флагом MASK
    void DistTestParameters::SetMaskMode() {
        SetNormalMode();
    }

    // Конструктор GenBlocksParameters
    GenBlocksParameters::GenBlocksParameters(uint16_t hash_bits, uint16_t mask_bits, uint16_t num_threads,
                                             uint64_t num_words, uint16_t words_length, TestFlag mode)
            : AdvancedTestParameters(hash_bits, num_threads, num_words, mode)
            , mask_bits(mask_bits)
            , words_length(words_length) {
    }

    // Изменяет хеш в зависимости от флага тестирования
    uint64_t ModifyHash(const AdvancedTestParameters& parameters, uint64_t hash) {
        switch (parameters.mode) {
            case TestFlag::NORMAL:
                return hash;
            case TestFlag::MASK: {
                const auto &derived_parameters = dynamic_cast<const GenBlocksParameters &>(parameters);
                return XorFoldMask(hash, derived_parameters.mask_bits);
            }
            case TestFlag::BINS: {
                const auto &derived_parameters = dynamic_cast<const DistTestParameters &>(parameters);
                return hash / derived_parameters.divisor;
            }
            default: {
                BOOST_ASSERT_MSG(false, "Unknown flag");
                return -1;
            }
        }
    }
}

