// Здесь собраны различные хеш-функции,
// имплементации которых я не нашел в виде отдельных проектов или файлов

#ifndef THESIS_WORK_OTHER_HASH_FUNCTIONS_H
#define THESIS_WORK_OTHER_HASH_FUNCTIONS_H

#include <cassert>
#include <concepts>
#include <string>

//=============================================================================

// http://www.cse.yorku.ca/~oz/hash.html
template<std::unsigned_integral UintT>
UintT DJB2Hash(std::string_view str) {
    UintT hash = 5381;
    for (uint8_t c : str) {
        hash = (hash << 5) + hash + c; /* hash * 33 + c */
    }
    return hash;
}

// https://www.programmingalgorithms.com/algorithm/sdbm-hash/cpp/
template<std::unsigned_integral UintType>
UintType SDBMHash(std::string_view str) {
    UintType hash = 0;
    for (char c : str) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

// https://www.programmingalgorithms.com/algorithm/pjw-hash/cpp/
template<std::unsigned_integral UintT>
UintT PJWHash(std::string_view str) {
    const std::unsigned_integral auto BitsInUnsignedInt = static_cast<UintT>(sizeof(UintT) * 8);
    const std::unsigned_integral auto ThreeQuarters     = static_cast<UintT>((BitsInUnsignedInt  * 3) / 4);
    const std::unsigned_integral auto OneEighth         = static_cast<UintT>(BitsInUnsignedInt / 8);
    const UintT HighBits  = static_cast<UintT>(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    UintT hash = 0;
    UintT test = 0;

    for (char c : str) {
        hash = (hash << OneEighth) + c;
        if ((test = hash & HighBits) != 0) {
            hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

// https://en.wikipedia.org/wiki/Jenkins_hash_function
template<std::unsigned_integral UintT>
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

#endif //THESIS_WORK_OTHER_HASH_FUNCTIONS_H
