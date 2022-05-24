#include "count_collisions.h"

uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hashes) {
    uint64_t collisions = 0;
    for (const auto& [hash, count] : hashes) {
        const uint64_t tmp = (count > 1) ? (count - 1) : 0;
        collisions += tmp;
    }
    return collisions;
}