#include "hashes.h"

// HFL = Hash function library
namespace hfl {
    //----------- BuildHashes ----------

    std::vector<Hash16Struct> Build16bitsHashes(BuildFlag bf) {
        using namespace std::literals;
        std::vector<Hash16Struct> hashes;

        hashes.emplace_back("PearsonHash"s, pearson_hash_16);
        hashes.emplace_back("FNV-1a"s, fnv_1a_hash_16);
        hashes.emplace_back("DJB2"s, djb2_hash_16);
        hashes.emplace_back("SDBM"s, sdbm_hash_16);
        hashes.emplace_back("PJW"s, pjw_hash_16);
        hashes.emplace_back("BuzHash"s, buz_hash_16);
        hashes.emplace_back("One at a time"s, one_at_a_time_hash_16);
        hashes.emplace_back("SpookyHash"s, spooky_hash_16);
        hashes.emplace_back("Fast-Hash"s, fast_hash_16);
        return hashes;
    }

    std::vector<Hash24Struct> Build24bitsHashes(BuildFlag bf) {
        using namespace std::literals;
        std::vector<Hash24Struct> hashes;

        hashes.emplace_back("PearsonHash"s, pearson_hash_24);
        hashes.emplace_back("FNV-1a"s, fnv_1a_hash_24);
        hashes.emplace_back("DJB2"s, djb2_hash_24);
        hashes.emplace_back("SDBM"s, sdbm_hash_24);
        hashes.emplace_back("PJW"s, pjw_hash_24);
        hashes.emplace_back("BuzHash"s, buz_hash_24);
        hashes.emplace_back("One at a time"s, one_at_a_time_hash_24);
        hashes.emplace_back("SpookyHash"s, spooky_hash_24);
        hashes.emplace_back("Fast-Hash"s, fast_hash_24);

        return hashes;
    }

    std::vector<Hash32Struct> Build32bitsHashes(BuildFlag bf) {
        using namespace std::literals;
        std::vector<Hash32Struct> hashes;

        if (bf == BuildFlag::ALL) {
            hashes.emplace_back("PearsonHash"s, pearson_hash_32);
            hashes.emplace_back("FNV-1a"s, fnv_1a_hash_32);
            hashes.emplace_back("DJB2"s, djb2_hash_32);
            hashes.emplace_back("SDBM"s, sdbm_hash_32);
            hashes.emplace_back("PJW"s, pjw_hash_32);
            hashes.emplace_back("BuzHash"s, buz_hash_32);
            hashes.emplace_back("One at a time"s, one_at_a_time_hash_32);
            hashes.emplace_back("SpookyHash"s, spooky_hash_32);
            hashes.emplace_back("Fast-Hash"s, fast_hash_32);
        }

        hashes.emplace_back("SuperFastHash"s, super_fast_hash);
        hashes.emplace_back("MurmurHash1"s, murmur_hash1);
        hashes.emplace_back("MurmurHash2"s, murmur_hash2_32);
        hashes.emplace_back("MurmurHash2A"s, murmur_hash2a_32);
        hashes.emplace_back("MurmurHash3"s, murmur_hash3);
        hashes.emplace_back("CityHash32"s, city_hash_32);
        hashes.emplace_back("FarmHash32"s, farm_hash_32);
        hashes.emplace_back("T1HA0 32le"s, t1ha0_32le_hash);
        hashes.emplace_back("T1HA0 32be"s, t1ha0_32be_hash);
        hashes.emplace_back("xxHash32"s, xx_hash_32);

        return hashes;
    }

    std::vector<Hash64Struct> Build64bitsHashes(BuildFlag bf) {
        using namespace std::literals;
        std::vector<Hash64Struct> hashes;

        if (bf == BuildFlag::ALL) {
            hashes.emplace_back("PearsonHash"s, pearson_hash_64);
            hashes.emplace_back("FNV-1a"s, fnv_1a_hash_64);
            hashes.emplace_back("DJB2"s, djb2_hash_64);
            hashes.emplace_back("SDBM"s, sdbm_hash_64);
            hashes.emplace_back("PJW"s, pjw_hash_64);
            hashes.emplace_back("BuzHash"s, buz_hash_64);
            hashes.emplace_back("One at a time"s, one_at_a_time_hash_64);
            hashes.emplace_back("SpookyHash"s, spooky_hash_64);
            hashes.emplace_back("Fast-Hash"s, fast_hash_64);
        }

        hashes.emplace_back("MurmurHash2 64 bits"s, murmur_hash2_64);
        hashes.emplace_back("CityHash64"s, city_hash_64);
        hashes.emplace_back("FarmHash64"s, farm_hash_64);
        hashes.emplace_back("MetroHash64"s, metro_hash_64);
        hashes.emplace_back("T1HA1 le", t1ha1_le_hash);
        hashes.emplace_back("T1HA1 be", t1ha1_be_hash);
        hashes.emplace_back("T1HA2 atonce", t1ha2_atonce_hash);
        hashes.emplace_back("xxHash64", xx_hash_64);
        hashes.emplace_back("XXH3_64bits", xxh3_64bits);

        return hashes;
    }
}