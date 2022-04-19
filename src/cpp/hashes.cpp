#include "hashes.h"

// HFL = Hash function library
namespace hfl {
    //----------- BuildHashes ----------

    #define ADD_UNIVERSAL_HASHES(BITS)                                            \
        hashes.emplace_back("FNV-1a Hash"s, fnv_1a_hash_##BITS);                  \
        hashes.emplace_back("DJB2 Hash"s, djb2_hash_##BITS);                      \
        hashes.emplace_back("SDBM Hash"s, sdbm_hash_##BITS);                      \
        hashes.emplace_back("PJW Hash"s, pjw_hash_##BITS);                        \
        hashes.emplace_back("One at a time hash"s, one_at_a_time_hash_##BITS);    \
        hashes.emplace_back("SpookyHash"s, spooky_hash_##BITS);                   \
        hashes.emplace_back("Fast-Hash"s, fast_hash_##BITS)

    std::vector<Hasher16> Build16bitsHashes() {
        using namespace std::literals;
        std::vector<Hasher16> hashes;
        hashes.reserve(9);

        ADD_UNIVERSAL_HASHES(16);
        hashes.emplace_back("PearsonHash"s, pearson_hash_16);
        hashes.emplace_back("BuzHash"s, buz_hash_16);

        return hashes;
    }

    std::vector<Hasher24> Build24bitsHashes() {
        using namespace std::literals;
        std::vector<Hasher24> hashes;
        hashes.reserve(8);

        ADD_UNIVERSAL_HASHES(24);
        hashes.emplace_back("PearsonHash"s, pearson_hash_24);

        return hashes;
    }

    std::vector<Hasher32> Build32bitsHashes() {
        using namespace std::literals;
        std::vector<Hasher32> hashes;
        hashes.reserve(23);

        ADD_UNIVERSAL_HASHES(32);
        hashes.emplace_back("PearsonHash"s, pearson_hash_32);
        hashes.emplace_back("BuzHash"s, buz_hash_32);

        hashes.emplace_back("SuperFastHash"s, super_fast_hash);
        hashes.emplace_back("MurmurHash1"s, murmur_hash1);
        hashes.emplace_back("MurmurHash2"s, murmur_hash2_32);
        hashes.emplace_back("MurmurHash2A"s, murmur_hash2a_32);
        hashes.emplace_back("MurmurHash3"s, murmur_hash3);
        hashes.emplace_back("CityHash32"s, city_hash_32);
        hashes.emplace_back("FarmHash32"s, farm_hash_32);
        hashes.emplace_back("T1HA0 32le hash"s, t1ha0_32le_hash);
        hashes.emplace_back("T1HA0 32be hash"s, t1ha0_32be_hash);
        hashes.emplace_back("xxHash32"s, xx_hash_32);

        hashes.emplace_back("wyhash32"s, wyhash32);
        hashes.emplace_back("nmhash32"s, nmhash32);
        hashes.emplace_back("nmhash32x"s, nmhash32x);
        hashes.emplace_back("halfsiphash"s, halfsiphash);

        return hashes;
    }

    std::vector<Hasher48> Build48bitsHashes() {
        using namespace std::literals;

        std::vector<Hasher48> hashes;
        hashes.reserve(7);

        ADD_UNIVERSAL_HASHES(48);
        return hashes;
    }

    std::vector<Hasher64> Build64bitsHashes() {
        using namespace std::literals;
        std::vector<Hasher64> hashes;
        hashes.reserve(34);

        ADD_UNIVERSAL_HASHES(64);
        hashes.emplace_back("PearsonHash"s, pearson_hash_64);
        hashes.emplace_back("BuzHash"s, buz_hash_64);


        hashes.emplace_back("MurmurHash2 64 bits"s, murmur_hash2_64);
        hashes.emplace_back("CityHash64"s, city_hash_64);
        hashes.emplace_back("CityHash64WithSeed"s, city_hash_64_with_seed);
        hashes.emplace_back("CityHash64WithSeeds"s, city_hash_64_with_seeds);
        hashes.emplace_back("FarmHash64"s, farm_hash_64);
        hashes.emplace_back("FarmHash64WithSeed"s, farm_hash_64_with_seed);
        hashes.emplace_back("FarmHash64WithSeeds"s, farm_hash_64_with_seeds);
        hashes.emplace_back("MetroHash64"s, metro_hash_64);
        hashes.emplace_back("T1HA0 AVX2 hash", t1ha0_avx2_hash);
        hashes.emplace_back("T1HA1 le hash", t1ha1_le_hash);
        hashes.emplace_back("T1HA1 be hash", t1ha1_be_hash);
        hashes.emplace_back("T1HA2 atonce hash", t1ha2_atonce_hash);
        hashes.emplace_back("xxHash64", xx_hash_64);
        hashes.emplace_back("XXH3_64bits", xxh3_64bits);
        hashes.emplace_back("XXH3_64bits_withSeed", xxh3_64bits_with_seed);

        hashes.emplace_back("wyhash", wy_hash_64);
        hashes.emplace_back("PengyHash", pengy_hash_64);
        hashes.emplace_back("mx3", mx3);
        hashes.emplace_back("SipHash", siphash);
        hashes.emplace_back("SipHash13", siphash13);
        hashes.emplace_back("SipHash (Google Impl)", siphash_avx2);
        hashes.emplace_back("SipHash13 (Google Impl)", siphash13_avx2);
        hashes.emplace_back("HighwayHash", highway_hash);
        hashes.emplace_back("MUM hash", mum_hash);
        hashes.emplace_back("mir hash", mir_hash);
        return hashes;
    }
}