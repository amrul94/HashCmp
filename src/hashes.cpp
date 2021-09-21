#include "hashes.h"

namespace hashes {

    //----------- HashStructs ----------

    [[maybe_unused]] Hash32Struct::Hash32Struct(std::string hash_name, const BaseHash32Wrapper& hash_function)
            : hash_name(std::move(hash_name))
            , hash_function(hash_function) {
    }

    [[maybe_unused]] Hash64Struct::Hash64Struct(std::string hash_name, const BaseHash64Wrapper& hash_function)
            : hash_name(std::move(hash_name))
            , hash_function(hash_function) {
    }

    //----------- BuildHashes ----------

    std::vector<Hash32Struct> Build32bitsHashes() {
        using namespace std::literals;
        std::vector<Hash32Struct> hash_functions;

        hash_functions.emplace_back("RollingHash (BuzHash32)", buz_hash_32);
        hash_functions.emplace_back(CITY_HASH_32_NAME, city_hash_32);
        hash_functions.emplace_back(DJB2_HASH_32_NAME, djb2_hash_32);

        hash_functions.emplace_back("FarmHash32", farm_hash_32);
        hash_functions.emplace_back("FarmHash32WithSeed", farm_hash_32_with_seed);

        hash_functions.emplace_back("FastHash32", fast_hash_32);
        hash_functions.emplace_back("FNV-1a hash 32", fnv_1a_hash_32);

        hash_functions.emplace_back("Jenkins hash (One at a time)", one_at_a_time_hash);
        hash_functions.emplace_back("Jenkins hash (lookup3 - little)", lookup3_little_hash);
        hash_functions.emplace_back("Jenkins hash (lookup3 - big)", lookup_big_hash);

        hash_functions.emplace_back("MurmurHash1", murmur_hash1);
        hash_functions.emplace_back("MurmurHash1Aligned", murmur_hash1_aligned);
        hash_functions.emplace_back("MurmurHash2", murmur_hash2);

        hash_functions.emplace_back("PearsonHash32", pearson_hash_32);
        hash_functions.emplace_back(PJW_HASH_32_NAME, pjw_hash_32);
        hash_functions.emplace_back(SDBM_HASH_32_NAME, sdbm_hash_32);
        hash_functions.emplace_back(SUPER_FAST_HASH_NAME, super_fast_hash);
        hash_functions.emplace_back("Spooky hash 32", spooky_hash_32);

        hash_functions.emplace_back("T1HA0 32le", t1ha0_32le_hash);
        hash_functions.emplace_back("T1HA0 32be", t1ha0_32be_hash);

        hash_functions.emplace_back("xxHash32", xx_hash_32);

        return hash_functions;
    }

    std::vector<Hash64Struct> Build64bitsHashes() {
        using namespace std::literals;
        std::vector<Hash64Struct> hashes;

        hashes.emplace_back("RollingHash (BuzHash64)", buz_hash_64);

        hashes.emplace_back("CityHash64", city_hash_64);
        hashes.emplace_back("CityHash64WithSeed", city_hash_64_with_seed);
        hashes.emplace_back("CityHash64WithSeeds", city_hash_64_with_seeds);

        hashes.emplace_back(DJB2_HASH_64_NAME, djb2_hash_64);

        hashes.emplace_back("FarmHash64", farm_hash_64);
        hashes.emplace_back("FarmHash64WithSeed", farm_hash_64_with_seed);
        hashes.emplace_back("FarmHash64WithSeeds", farm_hash_64_with_seeds);

        hashes.emplace_back("FastHash64", fast_hash_64);
        hashes.emplace_back("FNV-1a hash 64", fnv_1a_hash_64);

        hashes.emplace_back("MetroHash64_1", metro_hash_64_1);
        hashes.emplace_back("MetroHash64_2", metro_hash_64_2);

        hashes.emplace_back("MurmurHash64A", murmur_hash_64a);
        hashes.emplace_back("MurmurHash64B", murmur_hash_64b);

        hashes.emplace_back(PJW_HASH_64_NAME, pjw_hash_64);
        hashes.emplace_back("PearsonHash64", pearson_hash_64);
        hashes.emplace_back(SDBM_HASH_64_NAME, sdbm_hash_64);
        hashes.emplace_back("Spooky hash 64", spooky_hash_64);

        hashes.emplace_back("T1HA1 le", t1ha1_le_hash);
        hashes.emplace_back("T1HA1 be", t1ha1_be_hash);
        hashes.emplace_back("T1HA2 atonce", t1ha2_atonce_hash);

        hashes.emplace_back("xxHash64", xx_hash_64);
        hashes.emplace_back("XXH3_64bits", xxh3_64bits);
        hashes.emplace_back("XXH3_64bits_withSeed", xxh3_64bits_with_seed);

        return hashes;
    }
}