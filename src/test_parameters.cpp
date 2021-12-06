#include "test_parameters.h"

#include <cmath>
#include <limits>

#include "log_duration.h"

ReportsRoot::ReportsRoot(const std::filesystem::path& root_path)
        : root_path(root_path)
        , log(root_path / "Log.txt") {
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

TestParameters::TestParameters(uint16_t hash_bits, uint16_t test_bits, TestFlag mode)
        : TestParameters(hash_bits, test_bits, 0, mode) {
}

TestParameters::TestParameters(uint16_t hash_bits, uint16_t test_bits, uint64_t key_counts, TestFlag mode)
        : hash_bits(hash_bits)
        , test_bits(test_bits)
        , key_count(key_counts)
        , mode(mode) {
}

uint64_t TestParameters::GiveDivisor(uint16_t degree) {
    return static_cast<uint64_t>(std::pow(2, degree));
}

CheckParameters::CheckParameters(uint16_t hash_bits, uint16_t test_bits, TestFlag mode)
        : TestParameters(hash_bits, test_bits, mode) {
    SetParameters();
}

void CheckParameters::SetParameters() {
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

void CheckParameters::SetNormalMode() {
    hfl::uint24_t max_uint = 0;
    switch (test_bits) {
        case 16:
            max_uint = std::numeric_limits<uint16_t>::max();
            break;
        case 24:
            max_uint = std::numeric_limits<hfl::uint24_t>::max();
            break;
        default:
            assert(false);
    }
    key_count = static_cast<uint64_t>(max_uint) + 1;
    buckets_count = key_count;
}

void CheckParameters::SetBinsMode() {
    key_count = std::numeric_limits<uint32_t>::max() + static_cast<uint64_t>(1);
    buckets_count = MAX_BINS_COUNT;
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

void CheckParameters::SetMaskMode() {
    SetNormalMode();
}

WordsParameters::WordsParameters(uint16_t hash_bits, uint16_t test_bits, uint64_t word_counts, uint32_t length, TestFlag mode)
        : TestParameters(hash_bits, test_bits, word_counts, mode)
        , words_length(length){
}

uint64_t ModifyHash(const TestParameters& tp, uint64_t hash) {
    switch (tp.mode) {
        case TestFlag::NORMAL:
            return hash;
        case TestFlag::MASK:
            return MaskShift(hash, tp.test_bits);
        case TestFlag::BINS: {
            const auto &cp = dynamic_cast<const CheckParameters &>(tp);
            return hash / cp.divisor;
        }
        default:
            assert(false);
    }
}

