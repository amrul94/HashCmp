#include "blocks_generator.h"


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