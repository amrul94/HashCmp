#ifndef THESIS_WORK_HELPER_STRUCTS_H
#define THESIS_WORK_HELPER_STRUCTS_H

#include <cmath>
#include <cstdint>

struct TestParameters {
    uint16_t hash_bits{};
    uint64_t key_counts{};

    explicit TestParameters(uint16_t hash_bits);
    TestParameters(uint16_t hash_bits, uint64_t key_counts);
    [[nodiscard]] static uint64_t GiveDivisor(uint16_t degree);
    virtual ~TestParameters() = default;
};

struct CheckParameters : TestParameters{
    uint32_t small_buckets_count{};
    uint32_t big_buckets_count{};
    uint64_t small_divisor{};
    uint64_t big_divisor{};

    explicit CheckParameters(uint16_t hash_bits);

private:
    static const uint16_t DIVIDER_FOR_32 = 5; // попробовать 5
    static const uint16_t DIVIDER_FOR_64 = DIVIDER_FOR_32 + 32;
    static const uint32_t SBC_32_64 = 1'048'576;
    static const uint32_t BBC_32_64 = 134'217'728;

    void SetParameters();
    void SetBuckets_16_24();
    void SetBuckets_32_64();
};

struct WordsParameters : TestParameters {
    uint32_t words_length{};
    uint64_t mod{};
    uint16_t test_bits{};

    explicit WordsParameters(uint16_t hash_bits, uint64_t word_counts, uint32_t length, uint64_t mod, uint16_t test_bits);
};




#endif //THESIS_WORK_HELPER_STRUCTS_H
