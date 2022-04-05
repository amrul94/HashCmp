// Здесь собраны различные хеш-функции,
// имплементации которых я не нашел в виде отдельных проектов или файлов

#ifndef THESIS_WORK_OTHER_HASH_FUNCTIONS_H
#define THESIS_WORK_OTHER_HASH_FUNCTIONS_H

#include <cassert>
#include <concepts>
#include <string>

//=============================================================================

// https://github.com/rurban/smhasher/blob/master/Hashes.cpp
static inline uint32_t FNV32a(const char *key, int len, uint32_t seed) {
    uint32_t h = seed;
    const auto  *data = (const uint8_t *)key;

    h ^= UINT32_C(2166136261);
    for (int i = 0; i < len; i++) {
        h ^= data[i];
        h *= 16777619;
    }
    return h;
}

static inline uint64_t FNV64a(const char *key, int len, uint64_t seed) {
    uint64_t  h = seed;
    auto  *data = (uint8_t *)key;
    const uint8_t *const end = &data[len];

    h ^= UINT64_C(0xcbf29ce484222325);
    while (data < end) {
        h ^= *data++;
        h *= UINT64_C(0x100000001b3);
    }
    return h;
}

template<typename UintT>
static inline UintT one_at_a_time_hash(const uint8_t* key, size_t length) {
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
static inline UintT DJB2Hash(const char *message, size_t length) {
    UintT hash = 5381;
    for (size_t i = 0; i < length; ++i) {
        hash = hash * 33 + message[i];
    }
    return hash;
}

// https://www.programmingalgorithms.com/algorithm/sdbm-hash/cpp/
template<typename UintType>
static inline UintType SDBMHash(const char *message, size_t length) {
    UintType hash = 0;
    for (size_t i = 0; i < length; ++i) {
        hash = message[i] + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

// https://www.programmingalgorithms.com/algorithm/pjw-hash/cpp/
template<typename UintT, uint8_t BitsInUnsignedInt = sizeof(UintT) * 8>
static inline UintT PJWHash(const char *message, size_t length) {
    const uint8_t ThreeQuarters     = (BitsInUnsignedInt  * 3) / 4;
    const uint8_t OneEighth         = BitsInUnsignedInt / 8;
    const auto MaxUintT = std::numeric_limits<UintT>::max();
    const auto HighBits  = MaxUintT << (BitsInUnsignedInt - OneEighth);
    UintT hash = 0;
    UintT test{};

    for (size_t i = 0; i < length; ++i) {
        hash = (hash << OneEighth) + message[i];
        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

#endif //THESIS_WORK_OTHER_HASH_FUNCTIONS_H
