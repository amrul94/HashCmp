// Здесь собраны различные хеш-функции,
// имплементации которых я не нашел в виде отдельных проектов или файлов

#ifndef THESIS_WORK_OTHER_HASH_FUNCTIONS_H
#define THESIS_WORK_OTHER_HASH_FUNCTIONS_H

#include <cassert>
#include <string>

// http://www.cse.yorku.ca/~oz/hash.html
template<typename UintType>
UintType DJB2Hash(std::string_view str) {
    UintType hash = 5381;

    for (uint8_t c : str) {
        hash = (hash << 5) + hash + c; /* hash * 33 + c */
    }

    return hash;
}

// https://www.programmingalgorithms.com/algorithm/sdbm-hash/cpp/
template<typename UintType>
UintType SDBMHash(std::string_view str) {
    UintType hash = 0;

    for (char c : str)
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

// https://www.programmingalgorithms.com/algorithm/pjw-hash/cpp/
template<typename UintType>
UintType PJWHash(std::string_view str) {
    const auto BitsInUnsignedInt = static_cast<UintType>(sizeof(UintType) * 8);
    const auto ThreeQuarters     = static_cast<UintType>((BitsInUnsignedInt  * 3) / 4);
    const auto OneEighth         = static_cast<UintType>(BitsInUnsignedInt / 8);
    const UintType HighBits  = static_cast<UintType>(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    UintType hash = 0;
    UintType test = 0;

    for (char c : str) {
        hash = (hash << OneEighth) + c;
        if ((test = hash & HighBits) != 0) {
            hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

// https://en.wikipedia.org/wiki/Jenkins_hash_function
template<typename UintType>
UintType one_at_a_time_hash(const uint8_t* key, size_t length) {
    size_t i = 0;
    UintType hash = 0;
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
