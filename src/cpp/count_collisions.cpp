#include "count_collisions.h"


// Вычисляет число коллизий
uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hash_values) {
    uint64_t collisions = 0;
    for (const auto& [hash, count] : hash_values) {
        const uint64_t tmp = (count > 1) ? (count - 1) : 0;
        collisions += tmp;
    }
    return collisions;
}