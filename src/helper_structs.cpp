#include "helper_structs.h"

#include <cmath>
#include <limits>

#include "hash_wrappers.h"

TestParameters::TestParameters(uint16_t hash_bits)
        : hash_bits(hash_bits) {
}

TestParameters::TestParameters(uint16_t hash_bits, uint64_t key_counts)
        : hash_bits(hash_bits)
        , key_counts(key_counts) {
}

uint64_t TestParameters::GiveDivisor(uint16_t degree) {
    return static_cast<uint64_t>(std::pow(2, degree));
}

CheckParameters::CheckParameters(uint16_t hash_bits)
        : TestParameters(hash_bits) {
    SetParameters();
}

void CheckParameters::SetParameters() {
    switch (hash_bits) {
        case 16:
            key_counts = std::numeric_limits<uint16_t>::max() + static_cast<uint64_t>(1);
            SetBuckets_16_24();
            break;
        case 24:
            key_counts = static_cast<uint64_t>(std::numeric_limits<hfl::uint24_t>::max()) + 1;
            SetBuckets_16_24();
            break;
        case 32:
            SetBuckets_32_64();
            small_divisor = GiveDivisor(12);
            big_divisor = GiveDivisor(DIVIDER_FOR_32);
            break;
        case 64:
            SetBuckets_32_64();
            small_divisor = GiveDivisor(44);
            big_divisor = GiveDivisor(DIVIDER_FOR_64);
            break;
        default:
            break;
    }
}

void CheckParameters::SetBuckets_16_24() {
    small_divisor = big_divisor = 1;
    small_buckets_count = key_counts;
    big_buckets_count = key_counts;
}

void CheckParameters::SetBuckets_32_64() {
    key_counts = std::numeric_limits<uint32_t>::max() + static_cast<uint64_t>(1);
    small_buckets_count = SBC_32_64;
    big_buckets_count = BBC_32_64;
}

WordsParameters::WordsParameters(uint16_t hash_bits, uint64_t word_counts, uint32_t length, uint64_t mod, uint16_t test_bits)
        : TestParameters(hash_bits, word_counts)
        , words_length(length)
        , mod(mod)
        , test_bits(test_bits) {
}

