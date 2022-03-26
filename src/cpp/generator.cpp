#include "generator.h"

#include <random>

// PCG default constant
static const uint64_t PRIME64_0 = 0xcafef00dd15ea5e5ULL;

// CityHash64 constants
static const uint64_t PRIME64_1 = 0xc3a5c85c97cb3127ULL;
static const uint64_t PRIME64_2 = 0xb492b66fbe98f273ULL;
static const uint64_t PRIME64_3 = 0x9ae16a3b2f90404fULL;

// Fast-Hash64 constant
static const uint64_t PRIME64_4 = 0x880355f21e6d1965ULL;

// MurmurHash64 constant
static const uint64_t PRIME64_5 = 0xc6a4a7935bd1e995ULL;

// MX3 Hash constant
static const uint64_t PRIME64_6 = 0xbea225f9eb34556dULL;

// SpookyHash64 constant
static const uint64_t PRIME64_7 = 0xDEADBEEFDEADBEEFULL;

// T1HA constants
static const uint64_t PRIME64_8 = 0xEC99BF0D8372CAABULL;
static const uint64_t PRIME64_9 = 0x82434FE90EDCEF39ULL;
static const uint64_t PRIME64_10 = 0xD4F06DB99D67BE4BULL;
static const uint64_t PRIME64_11 = 0xBD9CACC22C6E9571ULL;
static const uint64_t PRIME64_12 = 0x9C06FAF4D023E3ABULL;
static const uint64_t PRIME64_13 = 0xC060724A8424F345ULL;
static const uint64_t PRIME64_14 = 0xCB5AF53AE3AAAC31ULL;

// WyHash64 constants
static const uint64_t PRIME64_15 = 0xa0761d6478bd642fULL;
static const uint64_t PRIME64_16 = 0xe7037ed1a0b428dbULL;
static const uint64_t PRIME64_17 = 0x8ebc6af09c88c6e3ULL;
static const uint64_t PRIME64_18 = 0x589965cc75374cc3ULL;

// xxHash64 constants
static const uint64_t PRIME64_19 = 0x9E3779B185EBCA87ULL;
static const uint64_t PRIME64_20 = 0xC2B2AE3D27D4EB4FULL;
static const uint64_t PRIME64_21 = 0x165667B19E3779F9ULL;
static const uint64_t PRIME64_22 = 0x85EBCA77C2B2AE63ULL;
static const uint64_t PRIME64_23 = 0x27D4EB2F165667C5ULL;


std::vector<pcg64> GetGenerators(size_t count_generators) {
    assert(count_generators != 0);

    std::vector<pcg64> generators;
    generators.reserve(count_generators);

    static const std::vector<uint64_t> primes {
        PRIME64_0, PRIME64_1, PRIME64_2, PRIME64_3, PRIME64_4, PRIME64_5,
        PRIME64_6, PRIME64_7, PRIME64_8, PRIME64_9, PRIME64_10, PRIME64_11,
        PRIME64_12, PRIME64_13, PRIME64_14, PRIME64_15, PRIME64_16, PRIME64_17,
        PRIME64_18, PRIME64_19, PRIME64_20, PRIME64_21, PRIME64_22, PRIME64_23
    };

    size_t count_primes = count_generators < primes.size() ? count_generators : primes.size();
    for (size_t i = 0; i < count_primes; ++i) {
        generators.emplace_back(primes[i]);
    }

    pcg_extras::seed_seq_from<std::mt19937_64> seed_source;
    for (size_t i = count_primes; i < count_generators; ++i) {
        generators.emplace_back();
        generators[i].seed(seed_source);
    }
    return generators;
}

std::string UintToString(uint64_t src, uint64_t size) {
    std::string dest;
    dest.resize(size);
    std::memcpy(dest.data(), &src, size);
    return dest;
}

std::string GenerateRandomDataBlock(pcg64& rng, uint32_t length) {
    std::string word;
    word.resize(length);

    uint64_t chunk_size = sizeof(uint64_t);
    uint64_t num_chunks = length / chunk_size;
    uint64_t all_chunks_size = num_chunks * chunk_size;
    for (uint64_t i = 0; i < num_chunks; ++i) {
        uint64_t src = rng();
        uint64_t step = i * chunk_size;
        std::memcpy(word.data() + step, &src, chunk_size);
    }

    uint64_t src = rng();
    uint64_t diff = length - all_chunks_size;
    std::memcpy(word.data() + all_chunks_size, &src, diff);

    assert(word.size() == length);
    return word;
}

std::vector<std::string> GenerateRandomDataBlocks(pcg64& rng, uint64_t num_blocks, uint32_t block_length) {
    std::vector<std::string> generated_blocks;
    generated_blocks.reserve(num_blocks);

    for (uint64_t i = 0; i < num_blocks; ++i) {
        std::string str = GenerateRandomDataBlock(rng, block_length);
        generated_blocks.push_back(std::move(str));
    }

    return generated_blocks;
}