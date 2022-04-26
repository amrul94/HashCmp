#include "test_parameters.h"

#include <cmath>
#include <limits>

#include "timers.h"

namespace tests {

    std::string TestFlagToString(TestFlag mode) {
        switch (mode) {
            case TestFlag::NORMAL:
                return "Normal";
            case TestFlag::BINS:
                return "Bins";
            case TestFlag::MASK:
                return "Mask";
            default: {
                assert(false);
                return std::string{};
            }
        }
    }

    uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift) {
        const uint64_t mask = (1ull << mask_bits) - 1;
        return (src >> shift) & mask;
    }

    uint64_t XorFoldMask(uint64_t src, uint16_t mask_bits) {
        const uint64_t mask = (1ull << mask_bits) - 1;
        return (src >> mask_bits) ^ (src & mask);
    }

    TestParameters::TestParameters(uint16_t hash_bits, uint16_t num_threads)
        : hash_bits(hash_bits)
        , num_threads(num_threads) {
    }

    uint64_t TestParameters::GiveDivisor(uint16_t degree) {
        return 1ull << degree; // static_cast<uint64_t>(std::pow(2, degree));
    }

    AvalancheTestParameters::AvalancheTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys)
        : TestParameters(hash_bits, num_threads)
        , num_keys(num_keys) {
    }

    AdvancedTestParameters::AdvancedTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys,
                                                   TestFlag mode)
       : AvalancheTestParameters(hash_bits, num_threads, num_keys)
       , mode(mode) {
    }

    DistTestParameters::DistTestParameters(uint16_t hash_bits, uint16_t num_threads, uint64_t num_keys,
                                           uint64_t num_buckets, TestFlag mode)
        : AdvancedTestParameters(hash_bits, num_threads, num_keys, mode)
        , num_buckets(num_buckets) {
        SetParameters();
    }

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
                break;
        }
    }

    void DistTestParameters::SetNormalMode() {
        num_keys = 1ull << hash_bits; // static_cast<uint64_t>(std::pow(2, hash_bits));
        num_buckets = num_keys;
    }

    void DistTestParameters::SetBinsMode() {
        const uint64_t shift = 32;
        num_keys = 1ull << shift; // static_cast<uint64_t>(std::pow(2, 32));
        num_buckets = MAX_BINS_COUNT;
        switch (hash_bits) {
            case 32:
                divisor = 1ull << divider_for_32;
                break;
            case 48:
                divisor = 1ull << divider_for_48; // GiveDivisor(divider_for_48);
                break;
            case 64:
                divisor = 1ull << divider_for_64; // GiveDivisor(divider_for_64);
                break;
            default:
                assert(false);
        }
    }

    void DistTestParameters::SetMaskMode() {
        SetNormalMode();
    }

    GenBlocksParameters::GenBlocksParameters(uint16_t hash_bits, uint16_t mask_bits, uint16_t num_threads,
                                             uint64_t num_words, uint16_t words_length, TestFlag mode)
            : AdvancedTestParameters(hash_bits, num_threads, num_words, mode)
            , mask_bits(mask_bits)
            , words_length(words_length) {
    }

    uint64_t ModifyHash(const AdvancedTestParameters& tp, uint64_t hash) {
        switch (tp.mode) {
            case TestFlag::NORMAL:
                return hash;
            case TestFlag::MASK: {
                const auto &gbp = dynamic_cast<const GenBlocksParameters &>(tp);
                return XorFoldMask(hash, gbp.mask_bits);
            }
            case TestFlag::BINS: {
                const auto &cp = dynamic_cast<const DistTestParameters &>(tp);
                return hash / cp.divisor;
            }
            default: {
                assert(false);
                return -1;
            }
        }
    }
}

