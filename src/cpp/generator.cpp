#include "generator.h"

#include <random>


std::vector<pcg64> GetGenerators(uint16_t num_generators, uint64_t num_generate_numbers) {
    assert(num_generators != 0);

    pcg64 rng;
    std::vector<pcg64> generators {rng};
    generators.reserve(num_generators);
    const uint64_t step = num_generate_numbers / num_generators;

    pcg_extras::seed_seq_from<std::mt19937_64> seed_source;
    for (uint16_t i = 0; i < num_generators - 1; ++i) {
        rng.advance(step);
        generators.emplace_back(rng);
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