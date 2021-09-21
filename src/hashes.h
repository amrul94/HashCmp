#ifndef THESIS_WORK_HASHES_H
#define THESIS_WORK_HASHES_H

#include <utility>

#include "hash_wrappers.h"

namespace hashes {

    //----------- HashStructs ----------

    struct [[maybe_unused]] HashStruct {
        template<typename HashType>
        HashStruct(std::string hash_name, HashType hash_function)
            : hash_name(std::move(hash_name))
            , hash_function(hash_function) {
        }

        std::string hash_name;
        std::function<size_t(const std::string&)> hash_function;
    };

    struct Hash32Struct {
        [[maybe_unused]] Hash32Struct(std::string hash_name, const BaseHash32Wrapper& hash_function);

        std::string hash_name;
        const BaseHash32Wrapper& hash_function;
    };

    struct Hash64Struct {
        [[maybe_unused]] Hash64Struct(std::string hash_name, const BaseHash64Wrapper& hash_function);

        std::string hash_name;
        const BaseHash64Wrapper& hash_function;
    };


    // Hashes

    [[maybe_unused]] inline const BuzHash32Wrapper buz_hash_32;
    [[maybe_unused]] inline const BuzHash64Wrapper buz_hash_64;

    [[maybe_unused]] inline constinit CityHash32Wrapper city_hash_32;
    [[maybe_unused]] inline constinit CityHash64Wrapper city_hash_64;
    [[maybe_unused]] inline constinit CityHash64WithSeedWrapper city_hash_64_with_seed;
    [[maybe_unused]] inline constinit CityHash64WithSeedsWrapper city_hash_64_with_seeds;

    inline const char* CITY_HASH_32_NAME = "CityHash32";

    [[maybe_unused]] inline constinit DJB2Hash32Wrapper djb2_hash_32;
    [[maybe_unused]] inline constinit DJB2Hash64Wrapper djb2_hash_64;

    inline const char* DJB2_HASH_32_NAME = "Bernstein's hash djb2 32";
    inline const char* DJB2_HASH_64_NAME = "Bernstein's hash djb2 64";

    [[maybe_unused]] inline constinit FarmHash32Wrapper farm_hash_32;
    [[maybe_unused]] inline constinit FarmHash32WithSeedWrapper farm_hash_32_with_seed;
    [[maybe_unused]] inline constinit FarmHash64Wrapper farm_hash_64;
    [[maybe_unused]] inline constinit FarmHash64WithSeedWrapper farm_hash_64_with_seed;
    [[maybe_unused]] inline constinit FarmHash64WithSeedsWrapper farm_hash_64_with_seeds;

    [[maybe_unused]] inline constinit FastHash32Wrapper fast_hash_32;
    [[maybe_unused]] inline constinit FastHash64Wrapper fast_hash_64;

    [[maybe_unused]] inline constinit FNV1aHash32Wrapper fnv_1a_hash_32;
    [[maybe_unused]] inline constinit FNV1aHash64Wrapper fnv_1a_hash_64;

    [[maybe_unused]] inline constinit Lookup3LittleWrapper lookup3_little_hash;
    [[maybe_unused]] inline constinit Lookup3BigWrapper lookup_big_hash;

    [[maybe_unused]] inline constinit MetroHash64_1_Wrapper metro_hash_64_1;
    [[maybe_unused]] inline constinit MetroHash64_2_Wrapper metro_hash_64_2;

    [[maybe_unused]] inline constinit MurmurHash1Wrapper murmur_hash1;
    [[maybe_unused]] inline constinit MurmurHash1AlignedWrapper murmur_hash1_aligned;
    [[maybe_unused]] inline constinit MurmurHash2Wrapper murmur_hash2;
    [[maybe_unused]] inline constinit MurmurHash64AWrapper murmur_hash_64a;
    [[maybe_unused]] inline constinit MurmurHash64BWrapper murmur_hash_64b;

    [[maybe_unused]] inline constinit OneTimeHashWrapper one_at_a_time_hash;

    [[maybe_unused]] inline const PearsonHash32Wrapper pearson_hash_32;
    [[maybe_unused]] inline const PearsonHash64Wrapper pearson_hash_64;

    [[maybe_unused]] inline constinit PJWHash32Wrapper pjw_hash_32;
    [[maybe_unused]] inline constinit PJWHash64Wrapper pjw_hash_64;

    inline const char* PJW_HASH_32_NAME = "PJW Hash 32";
    inline const char* PJW_HASH_64_NAME = "PJW Hash 64";

    [[maybe_unused]] inline constinit SuperFastHashWrapper super_fast_hash;

    inline const char* SUPER_FAST_HASH_NAME = "SuperFastHash";

    [[maybe_unused]] inline constinit SDBMHash32Wrapper sdbm_hash_32;
    [[maybe_unused]] inline constinit SDBMHash64Wrapper sdbm_hash_64;

    inline const char* SDBM_HASH_32_NAME = "SDBM Hash 32";
    inline const char* SDBM_HASH_64_NAME = "SDBM hash 64";

    [[maybe_unused]] inline constinit SpookyHash32Wrapper spooky_hash_32;
    [[maybe_unused]] inline constinit SpookyHash64Wrapper spooky_hash_64;

    [[maybe_unused]] inline constinit T1HA0_32leWrapper t1ha0_32le_hash;
    [[maybe_unused]] inline constinit T1HA0_32beWrapper t1ha0_32be_hash;
    [[maybe_unused]] inline constinit T1HA1LeWrapper t1ha1_le_hash;
    [[maybe_unused]] inline constinit T1HA1BeWrapper t1ha1_be_hash;
    [[maybe_unused]] inline constinit T1HA2AtonceWrapper t1ha2_atonce_hash;

    [[maybe_unused]] inline constinit xxHash32Wrapper xx_hash_32;
    [[maybe_unused]] inline constinit xxHash64Wrapper xx_hash_64;
    [[maybe_unused]] inline constinit XXH3_64BitsWrapper xxh3_64bits;
    [[maybe_unused]] inline constinit XXH3_64BitsWithSeedWrapper xxh3_64bits_with_seed;


    //----------- BuildHashes ----------
    std::vector<Hash32Struct> Build32bitsHashes();
    std::vector<Hash64Struct> Build64bitsHashes();

}

#endif //THESIS_WORK_HASHES_H