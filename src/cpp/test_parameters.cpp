#include "test_parameters.h"

#include <cmath>
#include <limits>

#include "log_duration.h"

namespace tests {

    ReportsRoot::ReportsRoot(const std::filesystem::path& root_path)
            : root_path(root_path)
            , logger(root_path / "Log.txt") {
    }

    std::string TestFlagToString(TestFlag mode) {
        switch (mode) {
            case TestFlag::NORMAL:
                return "Normal";
            case TestFlag::BINS:
                return "Bins";
            case TestFlag::MASK:
                return "Mask";
            default:
                assert(false);
        }
    }

    uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift) {
        const auto mask = static_cast<uint64_t>(std::pow(2, mask_bits)) - 1;
        return (src >> shift) & mask;
    }

    uint64_t XorFoldMask(uint64_t src, uint16_t mask_bits) {
        const auto mask = static_cast<uint64_t>(std::pow(2, mask_bits)) - 1;
        return (src >> mask_bits) ^ (src & mask);
    }

    TestParameters::TestParameters(uint16_t hash_bits, size_t num_threads, TestFlag mode)
            : TestParameters(hash_bits, hash_bits, 0, num_threads, mode) {
    }

    TestParameters::TestParameters(uint16_t hash_bits, uint16_t test_bits, size_t num_threads, TestFlag mode)
            : TestParameters(hash_bits, test_bits, 0, num_threads, mode) {
    }

    TestParameters::TestParameters(uint16_t hash_bits, uint64_t key_counts, size_t num_threads, TestFlag mode)
            : TestParameters(hash_bits, hash_bits, key_counts, num_threads, mode) {
    }

    TestParameters::TestParameters(uint16_t hash_bits, uint16_t test_bits, uint64_t key_counts, size_t num_threads, TestFlag mode)
            : hash_bits(hash_bits)
            , test_bits(test_bits)
            , num_keys(key_counts)
            , num_threads(num_threads)
            , mode(mode) {

    }

    uint64_t TestParameters::GiveDivisor(uint16_t degree) {
        return static_cast<uint64_t>(std::pow(2, degree));
    }

    DistTestParameters::DistTestParameters(uint16_t hash_bits, size_t num_threads, TestFlag mode)
            : TestParameters(hash_bits, num_threads, mode) {
        SetParameters();
    }

    DistTestParameters::DistTestParameters(uint16_t hash_bits, uint16_t test_bits, size_t num_threads, TestFlag mode)
            : TestParameters(hash_bits, test_bits, num_threads, mode) {
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
        num_keys = static_cast<uint64_t>(std::pow(2, hash_bits));
        num_buckets = num_keys;
    }

    void DistTestParameters::SetBinsMode() {
        num_keys = static_cast<uint64_t>(std::pow(2, 32));
        num_buckets = MAX_BINS_COUNT;
        switch (test_bits) {
            case 32:
                divisor = GiveDivisor(DIVIDER_FOR_32);
                break;
            case 48:
                divisor = GiveDivisor(DIVIDER_FOR_48);
                break;
            case 64:
                divisor = GiveDivisor(DIVIDER_FOR_64);
                break;
            default:
                assert(false);
        }
    }

    void DistTestParameters::SetMaskMode() {
        SetNormalMode();
    }

    GenBlocksParameters::GenBlocksParameters(uint16_t hash_bits, uint64_t word_counts, TestFlag mode)
            : TestParameters(hash_bits, word_counts, 1, mode)
            , words_length(0) {
    }

    GenBlocksParameters::GenBlocksParameters(uint16_t hash_bits, uint64_t word_counts, uint32_t length,
                                             size_t num_threads, TestFlag mode)
            : TestParameters(hash_bits, word_counts, num_threads, mode)
            , words_length(length){
    }

    GenBlocksParameters::GenBlocksParameters(uint16_t hash_bits, uint16_t test_bits, uint64_t word_counts, uint32_t length,
                                             size_t num_threads, TestFlag mode)
            : TestParameters(hash_bits, test_bits, word_counts, num_threads, mode)
            , words_length(length){
    }

    uint64_t ModifyHash(const TestParameters& tp, uint64_t hash) {
        switch (tp.mode) {
            case TestFlag::NORMAL:
                return hash;
            case TestFlag::MASK:
                return XorFoldMask(hash, tp.test_bits);
            case TestFlag::BINS: {
                const auto &cp = dynamic_cast<const DistTestParameters &>(tp);
                return hash / cp.divisor;
            }
            default:
                assert(false);
        }
    }
}

