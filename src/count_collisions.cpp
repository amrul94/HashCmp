#include "count_collisions.h"

uint64_t CountCollisions(const CollisionsMap& hashes) {
    uint64_t collisions = 0;
    for (const auto& count : hashes) {
        if (count > 1) {
            collisions += (count - 1);
        }
    }
    return collisions;
}