#ifndef THESIS_WORK_COUNT_COLLISIONS_H
#define THESIS_WORK_COUNT_COLLISIONS_H

#include <iostream>
#include <map>
#include <set>
#include <string_view>
#include <unordered_map>

struct CollisionsInfo {
    size_t count;
    std::set<std::string_view> words;
};

using CollisionsMap = std::map<size_t, CollisionsInfo>;

size_t CountCollisions(const CollisionsMap& hashes, std::ostream& out);

#endif //THESIS_WORK_COUNT_COLLISIONS_H
