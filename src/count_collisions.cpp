#include "count_collisions.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <deque>
#include <tuple>
#include <vector>

namespace {

    void FillCyclesCollisions(std::map<int64_t, int64_t>& cycles_collisions, const std::set<std::string_view>& words,
                              std::ostream& out) {
        int64_t prev_word_number;
        for (auto it = words.begin(); it != words.end();) {
            out << *it;
            if (it->size() <= 8) {
                int64_t current_word_number = 0;
                memcpy(&current_word_number, it->data(), it->size());
                out << "[" << current_word_number << "]";
                if (it != words.begin() && prev_word_number != 0) {
                    int64_t difference = std::abs(current_word_number - prev_word_number);
                    out << " -> cycle = " << difference << ' ';
                    ++cycles_collisions[difference];
                }
                prev_word_number = current_word_number;
            }
            if (++it != words.end()) {
                out << ", ";
            }
        }
    }

    void PrintInfoAboutCollisions(std::map<int64_t, int64_t>& cycles_collisions, size_t hash, const CollisionsInfo& info,
                                  std::ostream& out) {
        out << hash << ": ";
        FillCyclesCollisions(cycles_collisions, info.words, out);
        out << std::endl;
    }

    std::pair<size_t, std::map<int64_t, int64_t>>
    CountCollisionsImpl(const CollisionsMap& hashes, std::ostream& out) {
        size_t collisions = 0;
        std::map<int64_t, int64_t> cycles_collisions;
        for (const auto& [hash, info] : hashes) {
            if (info.count > 1) {
                collisions += (info.count - 1);
                PrintInfoAboutCollisions(cycles_collisions, hash, info, out);
            }
        }
        return {collisions, std::move(cycles_collisions)};
    }

    void PrintCyclesCollisions(const std::map<int64_t, int64_t>& cycles_collisions, std::ostream& out) {
        out << "\nCycles collisions:\n";
        std::vector<std::pair<uint64_t, uint64_t>> result(cycles_collisions.begin(), cycles_collisions.end());

        std::sort(result.begin(), result.end(), [](const auto& lhs, const auto& rhs) {
            return std::tie(lhs.second, rhs.first) > std::tie(rhs.second, lhs.first);
        });
        for (auto it = result.begin(); it != result.end();) {
            out << it->first << ": " << it->second;
            if (++it != result.end()) {
                out << ", ";
            }
        }
    }
}


size_t CountCollisions(const CollisionsMap& hashes, std::ostream& out) {
    if (!out) {
        assert(false);
    }

    auto [collisions, cycles_collisions] = CountCollisionsImpl(hashes, out);
    PrintCyclesCollisions(cycles_collisions, out);
    return collisions;
}