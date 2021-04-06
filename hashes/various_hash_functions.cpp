#include "various_hash_functions.h"

uint32_t SDBMHash32(const std::string& str) {
    uint32_t hash = 0;

    for (char c : str) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

uint64_t SDBMHash64(const std::string& str) {
    uint64_t hash = 0;
    for (char c : str) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}