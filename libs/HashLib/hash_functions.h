// Здесь собраны различные хеш-функции,
// имплементации которых я не нашел в виде отдельных проектов или файлов

#ifndef THESIS_WORK_OTHER_HASH_FUNCTIONS_H
#define THESIS_WORK_OTHER_HASH_FUNCTIONS_H

#include <cassert>
#include <concepts>
#include <string>

//=============================================================================

// https://github.com/rurban/smhasher/blob/master/Hashes.cpp
uint32_t FNV32a(const void *key, int len, uint32_t seed);
uint64_t FNV64a(const char *key, int len, uint64_t seed);

template<typename UintT>
UintT one_at_a_time_hash(const uint8_t* key, size_t length) {
    size_t i = 0;
    UintT hash = 0;
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


// http://www.cse.yorku.ca/~oz/hash.html
template<typename UintT>
UintT DJB2Hash(std::string_view str) {
    UintT hash = 5381;
    for (uint8_t c : str) {
        hash = hash * 33 + c;
    }
    return hash;
}

// https://www.programmingalgorithms.com/algorithm/sdbm-hash/cpp/
template<typename UintType>
UintType SDBMHash(std::string_view str) {
    UintType hash = 0;
    for (char c : str) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

// https://www.programmingalgorithms.com/algorithm/pjw-hash/cpp/
template<typename UintT, uint8_t BitsInUnsignedInt = sizeof(UintT) * 8>
UintT PJWHash(std::string_view str) {
    const uint8_t ThreeQuarters     = (BitsInUnsignedInt  * 3) / 4;
    const uint8_t OneEighth         = BitsInUnsignedInt / 8;
    const auto MaxUintT = std::numeric_limits<UintT>::max();
    const auto HighBits  = MaxUintT << (BitsInUnsignedInt - OneEighth);
    UintT hash = 0;
    UintT test{};

    for (char c : str) {
        hash = (hash << OneEighth) + c;
        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

#endif //THESIS_WORK_OTHER_HASH_FUNCTIONS_H
