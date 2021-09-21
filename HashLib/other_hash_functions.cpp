#include "other_hash_functions.h"

uint32_t SDBMHash32(std::string_view str) {
    uint32_t hash = 0;

    for (char c : str) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

uint64_t SDBMHash64(std::string_view str) {
    uint64_t hash = 0;
    for (char c : str) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

uint32_t DJB2Hash32(std::string_view str) {
    uint32_t hash = 5381;
    for (auto c : str) {
        hash = (hash << 5) + hash + c; /* hash * 33 + c */
    }
    return hash;
}
uint64_t DJB2Hash64(std::string_view str) {
    uint64_t hash = 5381;
    for (auto c : str) {
        hash = (hash << 5) + hash + c; /* hash * 33 + c */
    }
    return hash;
}

uint32_t PJWHash32(std::string_view str) {
    const auto BitsInUnsignedInt = (uint32_t) (sizeof(uint32_t) * 8);
    const auto ThreeQuarters = (uint32_t) ((BitsInUnsignedInt * 3) / 4);
    const auto OneEighth = (uint32_t) (BitsInUnsignedInt / 8);
    const uint32_t HighBits = (uint32_t)(0xFFFFFFFF) << (int32_t)(BitsInUnsignedInt - OneEighth);
    uint32_t hash = 0;
    uint32_t test = 0;

    for (char i : str) {
        hash = (hash << (int32_t)OneEighth) + i;
        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> (int32_t)ThreeQuarters)) & (~HighBits));
        }
    }
    return hash;
}

uint64_t PJWHash64(std::string_view str) {
    const auto BitsInUnsignedInt = (uint64_t) (sizeof(uint64_t) * 8);
    const auto ThreeQuarters = (uint64_t) ((BitsInUnsignedInt * 3) / 4);
    const auto OneEighth = (uint64_t) (BitsInUnsignedInt / 8);
    const uint64_t HighBits = (uint64_t)(0xFFFFFFFF) << (int64_t)(BitsInUnsignedInt - OneEighth);
    uint64_t hash = 0;
    uint64_t test = 0;

    for (char i : str) {
        hash = (hash << (int64_t)OneEighth) + i;
        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> (int64_t)ThreeQuarters)) & (~HighBits));
        }
    }
    return hash;
}

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
    size_t i = 0;
    uint32_t hash = 0;
    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}