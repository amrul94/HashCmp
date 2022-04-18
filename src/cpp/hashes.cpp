#include "hashes.h"

// HFL = Hash function library
namespace hfl {
    //----------- BuildHashes ----------

    #define ADD_UNIVERSAL_HASHES(BITS)                                                  \
        hashes.reserve(8);                                                              \
        hashes.emplace_back("FNV-1a Hash"s, fnv_1a_hash_##BITS, BITS);                  \
        hashes.emplace_back("DJB2 Hash"s, djb2_hash_##BITS, BITS);                      \
        hashes.emplace_back("SDBM Hash"s, sdbm_hash_##BITS, BITS);                      \
        hashes.emplace_back("PJW Hash"s, pjw_hash_##BITS, BITS);                        \
        hashes.emplace_back("One at a time hash"s, one_at_a_time_hash_##BITS, BITS);    \
        hashes.emplace_back("SpookyHash"s, spooky_hash_##BITS, BITS);                   \
        hashes.emplace_back("Fast-Hash"s, fast_hash_##BITS, BITS)

    std::vector<Hash16Struct> Build16bitsHashes(BuildFlag bf) {
        using namespace std::literals;
        std::vector<Hash16Struct> hashes;

        hashes.emplace_back("PearsonHash"s, pearson_hash_16, 16);
        hashes.emplace_back("BuzHash"s, buz_hash_16, 16);                         \
        ADD_UNIVERSAL_HASHES(16);

        return hashes;
    }

    std::vector<Hash24Struct> Build24bitsHashes(BuildFlag bf) {
        using namespace std::literals;
        std::vector<Hash24Struct> hashes;

        hashes.emplace_back("PearsonHash"s, pearson_hash_24, 24);
        ADD_UNIVERSAL_HASHES(24);

        return hashes;
    }

    std::vector<Hash32Struct> Build32bitsHashes(BuildFlag bf) {
        using namespace std::literals;

        uint16_t bits = 32;
        std::vector<Hash32Struct> hashes;

        if (bf == BuildFlag::ALL) {
            hashes.emplace_back("PearsonHash"s, pearson_hash_32, bits);
            hashes.emplace_back("BuzHash"s, buz_hash_32, bits);                         \
            ADD_UNIVERSAL_HASHES(32);
        }

        hashes.emplace_back("SuperFastHash"s, super_fast_hash, bits);
        hashes.emplace_back("MurmurHash1"s, murmur_hash1, bits);
        hashes.emplace_back("MurmurHash2"s, murmur_hash2_32, bits);
        hashes.emplace_back("MurmurHash2A"s, murmur_hash2a_32, bits);
        hashes.emplace_back("MurmurHash3"s, murmur_hash3, bits);
        hashes.emplace_back("CityHash32"s, city_hash_32, bits);
        hashes.emplace_back("FarmHash32"s, farm_hash_32, bits);
        hashes.emplace_back("T1HA0 32le hash"s, t1ha0_32le_hash, bits);
        hashes.emplace_back("T1HA0 32be hash"s, t1ha0_32be_hash, bits);
        hashes.emplace_back("xxHash32"s, xx_hash_32, bits);

        hashes.emplace_back("wyhash32"s, wyhash32, bits);
        hashes.emplace_back("nmhash32"s, nmhash32, bits);
        hashes.emplace_back("nmhash32x"s, nmhash32x, bits);
        hashes.emplace_back("halfsiphash"s, halfsiphash, bits);

        return hashes;
    }

    std::vector<Hash48Struct> Build48bitsHashes(BuildFlag bf) {
        using namespace std::literals;

        std::vector<Hash48Struct> hashes;
        ADD_UNIVERSAL_HASHES(48);

        return hashes;
    }

    std::vector<Hash64Struct> Build64bitsHashes(BuildFlag bf) {
        using namespace std::literals;
        std::vector<Hash64Struct> hashes;

        uint16_t bits = 64;
        if (bf == BuildFlag::ALL) {
            hashes.emplace_back("PearsonHash"s, pearson_hash_64, bits);
            hashes.emplace_back("BuzHash"s, buz_hash_64, bits);                         \
            ADD_UNIVERSAL_HASHES(64);
        }

        hashes.emplace_back("MurmurHash2 64 bits"s, murmur_hash2_64, bits);
        hashes.emplace_back("CityHash64"s, city_hash_64, bits);
        hashes.emplace_back("CityHash64WithSeed"s, city_hash_64_with_seed, bits);
        hashes.emplace_back("CityHash64WithSeeds"s, city_hash_64_with_seeds, bits);
        hashes.emplace_back("FarmHash64"s, farm_hash_64, bits);
        hashes.emplace_back("FarmHash64WithSeed"s, farm_hash_64_with_seed, bits);
        hashes.emplace_back("FarmHash64WithSeeds"s, farm_hash_64_with_seeds, bits);
        hashes.emplace_back("MetroHash64"s, metro_hash_64, bits);
        hashes.emplace_back("T1HA0 AVX2 hash", t1ha0_avx2_hash, bits);
        hashes.emplace_back("T1HA1 le hash", t1ha1_le_hash, bits);
        hashes.emplace_back("T1HA1 be hash", t1ha1_be_hash, bits);
        hashes.emplace_back("T1HA2 atonce hash", t1ha2_atonce_hash, bits);
        hashes.emplace_back("xxHash64", xx_hash_64, bits);
        hashes.emplace_back("XXH3_64bits", xxh3_64bits, bits);
        hashes.emplace_back("XXH3_64bits_withSeed", xxh3_64bits_with_seed, bits);

        hashes.emplace_back("wyhash", wy_hash_64, bits);
        hashes.emplace_back("PengyHash", pengy_hash_64, bits);
        hashes.emplace_back("mx3", mx3, bits);
        hashes.emplace_back("SipHash", siphash, bits);
        hashes.emplace_back("SipHash13", siphash13, bits);
        hashes.emplace_back("SipHash (Google Impl)", siphash_avx2, bits);
        hashes.emplace_back("SipHash13 (Google Impl)", siphash13_avx2, bits);
        hashes.emplace_back("HighwayHash", highway_hash, bits);
        hashes.emplace_back("MUM hash", mum_hash, bits);
        hashes.emplace_back("mir hash", mir_hash, bits);
        return hashes;
    }
}