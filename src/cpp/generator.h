#ifndef THESISWORK_GENERATORS_H
#define THESISWORK_GENERATORS_H

#include <string>
#include <vector>

#include <pcg_random.hpp>

std::vector<pcg64> GetGenerators(uint16_t num_generators, uint64_t num_generate_numbers);

// в будущем возможно объединю эту функцию с WriteToHash из hash_wrappers
std::string UintToString(uint64_t src, uint64_t size);
std::string GenerateRandomDataBlock(pcg64& rng, uint32_t length);
std::vector<std::string> GenerateRandomDataBlocks(pcg64& rng, uint64_t num_blocks, uint32_t block_length);

#endif //THESISWORK_GENERATORS_H
