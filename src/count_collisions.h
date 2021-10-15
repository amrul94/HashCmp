#ifndef THESIS_WORK_COUNT_COLLISIONS_H
#define THESIS_WORK_COUNT_COLLISIONS_H

#include <iostream>
#include <set>
#include <string_view>
#include <vector>

struct CollisionsInfo {
    size_t count;
    std::set<std::string_view> words;
};

using CollisionsMap = std::vector<uint32_t>;

uint64_t CountCollisions(const CollisionsMap& hashes);

#endif //THESIS_WORK_COUNT_COLLISIONS_H
