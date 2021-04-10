#ifndef THESIS_WORK_HASHES_H
#define THESIS_WORK_HASHES_H

#include <utility>

#include "hash_wrappers.h"

namespace hashes {

    struct HashStruct {
        template<typename HashType>
        HashStruct(std::string hash_name, HashType hash_function)
            : hash_name(std::move(hash_name))
            , hash_function(hash_function) {
        }

        std::string hash_name;
        std::function<size_t(const std::string&)> hash_function;
    };



    // Hashes

    inline const BuzHash32Wrapper buz_hash_32;
    inline const BuzHash64Wrapper buz_hash_64;

    inline constinit CityHash32Wrapper city_hash_32;
    inline constinit CityHash64Wrapper city_hash_64;
    inline constinit CityHash64WithSeedWrapper city_hash_64_with_seed;
    inline constinit CityHash64WithSeedsWrapper city_hash_64_with_seeds;

    inline constinit DJB2Hash32Wrapper djb2_hash_32;
    inline constinit DJB2Hash64Wrapper djb2_hash_64;

    inline constinit FarmHash32Wrapper farm_hash_32;
    inline constinit FarmHash32WithSeedWrapper farm_hash_32_with_seed;
    inline constinit FarmHash64Wrapper farm_hash_64;
    inline constinit FarmHash64WithSeedWrapper farm_hash_64_with_seed;
    inline constinit FarmHash64WithSeedsWrapper farm_hash_64_with_seeds;

    inline constinit FastHash32Wrapper fast_hash_32;
    inline constinit FastHash64Wrapper fast_hash_64;

    inline constinit FNV1aHash32Wrapper fnv_1a_hash_32;
    inline constinit FNV1aHash64Wrapper fnv_1a_hash_64;

    inline constinit Lookup3LittleWrapper lookup3_little_hash;
    inline constinit Lookup3BigWrapper lookup_big_hash;

    inline constinit MetroHash64_1_Wrapper metro_hash_64_1;
    inline constinit MetroHash64_2_Wrapper metro_hash_64_2;

    inline constinit MurmurHash1Wrapper murmur_hash1;
    inline constinit MurmurHash1AlignedWrapper murmur_hash1_aligned;
    inline constinit MurmurHash2Wrapper murmur_hash2;
    inline constinit MurmurHash64AWrapper murmur_hash_64a;
    inline constinit MurmurHash64BWrapper murmur_hash_64b;

    inline constinit OneTimeHashWrapper one_at_a_time_hash;

    inline const PearsonHash32Wrapper pearson_hash_32;
    inline const PearsonHash32Wrapper pearson_hash_64;

    inline constinit PJWHash32Wrapper pjw_hash_32;
    inline constinit PJWHash64Wrapper pjw_hash_64;

    inline constinit SuperFastHashWrapper super_fast_hash;

    inline constinit SDBMHash32Wrapper sdbm_hash_32;
    inline constinit SDBMHash64Wrapper sdbm_hash_64;

    inline constinit SpookyHash32Wrapper spooky_hash_32;
    inline constinit SpookyHash64Wrapper spooky_hash_64;

    inline constinit T1HA0_32leWrapper t1ha0_32le_hash;
    inline constinit T1HA0_32beWrapper t1ha0_32be_hash;
    inline constinit T1HA1LeWrapper t1ha1_le_hash;
    inline constinit T1HA1BeWrapper t1ha1_be_hash;
    inline constinit T1HA2AtonceWrapper t1ha2_atonce_hash;

    inline constinit xxHash32Wrapper xx_hash_32;
    inline constinit xxHash64Wrapper xx_hash_64;
    inline constinit XXH3_64BitsWrapper xxh3_64bits;
    inline constinit XXH3_64BitsWithSeedWrapper xxh3_64bits_with_seed;


    //----------- BuildHashes ----------
    std::vector<HashStruct> Build32bitsHashes();
    std::vector<HashStruct> Build64bitsHashes();

}

#endif //THESIS_WORK_HASHES_H