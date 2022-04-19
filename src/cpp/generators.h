#ifndef THESISWORK_GENERATORS_H
#define THESISWORK_GENERATORS_H

#include <string>
#include <vector>

#include <pcg_random.hpp>

template<std::unsigned_integral UintT, size_t num_seeds, typename Generator>
static inline std::array<UintT, num_seeds> GenerateSeedsImpl(Generator& generator) {
    std::array<UintT, num_seeds> seeds{};
    for (UintT& seed : seeds) {
        seed = generator();
    }
    return seeds;
}

template<size_t num_seeds>
static inline std::array<uint8_t, num_seeds> Generate8BitsSeeds() {
    pcg8_oneseq_once_insecure rng;
    return GenerateSeedsImpl<uint8_t, num_seeds>(rng);
}

template<size_t num_seeds>
static inline std::array<uint32_t, num_seeds> Generate32BitsSeeds() {
    pcg32 rng;
    return GenerateSeedsImpl<uint32_t, num_seeds>(rng);
}

template<size_t num_seeds>
static inline std::array<uint64_t, num_seeds>  Generate64BitsSeeds() {
    pcg64 rng;
    return GenerateSeedsImpl<uint64_t, num_seeds>(rng);
}

inline constexpr uint64_t NUM_64_BITS_SEEDS = 4;

const auto [SEED_8_1, SEED_8_2, SEED_8_3, SEED_8_4,
            SEED_8_5, SEED_8_6, SEED_8_7, SEED_8_8,
            SEED_8_9, SEED_8_10, SEED_8_11, SEED_8_12,
            SEED_8_13, SEED_8_14, SEED_8_15, SEED_8_16] = Generate8BitsSeeds<16>();

const auto [SEED_32] = Generate32BitsSeeds<1>();
const auto [SEED_64_1, SEED_64_2, SEED_64_3, SEED_64_4] = Generate64BitsSeeds<NUM_64_BITS_SEEDS>();

std::vector<pcg64> GetGenerators(uint16_t num_generators, uint64_t num_generate_numbers);

std::string GenerateRandomDataBlock(pcg64& rng, uint32_t length);
std::vector<std::string> GenerateRandomDataBlocks(pcg64& rng, uint64_t num_blocks, uint32_t block_length);

#endif //THESISWORK_GENERATORS_H
