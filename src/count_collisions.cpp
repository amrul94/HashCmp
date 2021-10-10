#include "count_collisions.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <deque>
#include <tuple>
#include <vector>

uint64_t CountCollisions(const CollisionsMap& hashes) {
    uint64_t collisions = 0;
    for (const auto& [hash, count] : hashes) {
        if (count > 1) {
            collisions += (count - 1);
        }
    }
    return collisions;
}