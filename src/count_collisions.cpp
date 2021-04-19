#include "count_collisions.h"

size_t CountCollisions(const std::unordered_map<size_t, size_t>& hashes) {
    size_t collisions = 0;
    for (const auto [hash, count] : hashes) {
        if (count > 1) {
            collisions += (count - 1);
        }
    }
    return collisions;
}