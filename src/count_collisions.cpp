#include "count_collisions.h"

#include <cassert>
#include <cstring>

size_t CountCollisions(const CollisionsMap& hashes, std::ostream& out) {
    if (!out) {
        assert(false);
    }
    size_t collisions = 0;

    std::map<int64_t, int64_t> cycles_collisions;
    for (const auto& [hash, info] : hashes) {
        if (info.count > 1) {
            collisions += (info.count - 1);
            out << hash << ": ";
            int64_t prev_word_number;
            for (auto it = info.words.begin(); it != info.words.end();) {
                out << *it;
                if (it->size() <= 8) {
                    int64_t current_word_number = 0;
                    memcpy(&current_word_number, it->data(), it->size());
                    out << "[" << current_word_number << "]";
                    if (it != info.words.begin() && prev_word_number != 0) {
                        int64_t difference = std::abs(current_word_number - prev_word_number);
                        out << " -> cycle = " << difference << ' ';
                        ++cycles_collisions[difference];
                    }
                    prev_word_number = current_word_number;
                }
                if (++it != info.words.end()) {
                    out << ", ";
                }
            }
            out << std::endl;
        }
    }

    out << "\nCycles collisions:\n";
    for (auto it = cycles_collisions.begin(); it != cycles_collisions.end();) {
        out << it->first << ": " << it->second;
        if (++it != cycles_collisions.end()) {
            out << ", ";
        }

    }
    return collisions;
}