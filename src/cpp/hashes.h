#ifndef THESIS_WORK_HASHES_H
#define THESIS_WORK_HASHES_H

#include <concepts>
#include <utility>

#include "hash_wrappers.h"

// HFL = Hash function library
namespace hfl {

    // Hashes
    [[maybe_unused]] inline const BuzHashWrapper<uint16_t> buz_hash_16;
    [[maybe_unused]] inline const BuzHashWrapper<uint32_t> buz_hash_32;
    [[maybe_unused]] inline const BuzHashWrapper<uint64_t> buz_hash_64;

    [[maybe_unused]] inline constinit CityHash32Wrapper city_hash_32;
    [[maybe_unused]] inline constinit CityHash64Wrapper city_hash_64;
    [[maybe_unused]] inline constinit CityHash64WithSeedWrapper city_hash_64_with_seed;
    [[maybe_unused]] inline constinit CityHash64WithSeedsWrapper city_hash_64_with_seeds;


    [[maybe_unused]] inline constinit DJB2HashWrapper<uint16_t> djb2_hash_16;
    [[maybe_unused]] inline constinit DJB2HashWrapper<uint24_t> djb2_hash_24;
    [[maybe_unused]] inline constinit DJB2HashWrapper<uint32_t> djb2_hash_32;
    [[maybe_unused]] inline constinit DJB2HashWrapper<uint48_t> djb2_hash_48;
    [[maybe_unused]] inline constinit DJB2HashWrapper<uint64_t> djb2_hash_64;

    [[maybe_unused]] inline constinit FarmHash32Wrapper farm_hash_32;
    [[maybe_unused]] inline constinit FarmHash32WithSeedWrapper farm_hash_32_with_seed;
    [[maybe_unused]] inline constinit FarmHash64Wrapper farm_hash_64;
    [[maybe_unused]] inline constinit FarmHash64WithSeedWrapper farm_hash_64_with_seed;
    [[maybe_unused]] inline constinit FarmHash64WithSeedsWrapper farm_hash_64_with_seeds;

    [[maybe_unused]] inline constinit FastHash16Wrapper fast_hash_16;
    [[maybe_unused]] inline constinit FastHash24Wrapper fast_hash_24;
    [[maybe_unused]] inline constinit FastHash32Wrapper fast_hash_32;
    [[maybe_unused]] inline constinit FastHash48Wrapper fast_hash_48;
    [[maybe_unused]] inline constinit FastHash64Wrapper fast_hash_64;

    [[maybe_unused]] inline constinit FNV1aHash16Wrapper fnv_1a_hash_16;
    [[maybe_unused]] inline constinit FNV1aHash24Wrapper fnv_1a_hash_24;
    [[maybe_unused]] inline constinit FNV1aHash32Wrapper fnv_1a_hash_32;
    [[maybe_unused]] inline constinit FNV1aHash48Wrapper fnv_1a_hash_48;
    [[maybe_unused]] inline constinit FNV1aHash64Wrapper fnv_1a_hash_64;

    [[maybe_unused]] inline constinit HighwayHashWrapper highway_hash;

    [[maybe_unused]] inline constinit MetroHash64_Wrapper metro_hash_64;

    [[maybe_unused]] inline constinit MurmurHash1Wrapper murmur_hash1;
    [[maybe_unused]] inline constinit MurmurHash2Wrapper murmur_hash2_32;
    [[maybe_unused]] inline constinit MurmurHash2AWrapper murmur_hash2a_32;
    [[maybe_unused]] inline constinit MurmurHash64AWrapper murmur_hash2_64;
    [[maybe_unused]] inline constinit MurmurHash3Wrapper murmur_hash3;

    [[maybe_unused]] inline constinit MumHashWrapper mum_hash;
    [[maybe_unused]] inline constinit MirHashWrapper mir_hash;

    [[maybe_unused]] inline constinit MX3HashWrapper mx3;

    [[maybe_unused]] inline constinit nmHash32Wrapper nmhash32;
    [[maybe_unused]] inline constinit nmHash32XWrapper nmhash32x;

    [[maybe_unused]] inline constinit OneTimeHashWrapper<uint16_t> one_at_a_time_hash_16;
    [[maybe_unused]] inline constinit OneTimeHashWrapper<uint24_t> one_at_a_time_hash_24;
    [[maybe_unused]] inline constinit OneTimeHashWrapper<uint32_t> one_at_a_time_hash_32;
    [[maybe_unused]] inline constinit OneTimeHashWrapper<uint48_t> one_at_a_time_hash_48;
    [[maybe_unused]] inline constinit OneTimeHashWrapper<uint64_t> one_at_a_time_hash_64;

    [[maybe_unused]] inline const PearsonHash16Wrapper pearson_hash_16;
    [[maybe_unused]] inline const PearsonHash24Wrapper pearson_hash_24;
    [[maybe_unused]] inline const PearsonHash32Wrapper pearson_hash_32;
    [[maybe_unused]] inline const PearsonHash64Wrapper pearson_hash_64;

    [[maybe_unused]] inline const PengyHash64Wrapper pengy_hash_64;

    [[maybe_unused]] inline constinit PJWHashWrapper<uint16_t> pjw_hash_16;
    [[maybe_unused]] inline constinit PJWHashWrapper<uint24_t> pjw_hash_24;
    [[maybe_unused]] inline constinit PJWHashWrapper<uint32_t> pjw_hash_32;
    [[maybe_unused]] inline constinit PJWHashWrapper<uint48_t> pjw_hash_48;
    [[maybe_unused]] inline constinit PJWHashWrapper<uint64_t> pjw_hash_64;

    [[maybe_unused]] inline constinit SuperFastHashWrapper super_fast_hash;

    [[maybe_unused]] inline constinit SDBMHashWrapper<uint16_t> sdbm_hash_16;
    [[maybe_unused]] inline constinit SDBMHashWrapper<uint24_t> sdbm_hash_24;
    [[maybe_unused]] inline constinit SDBMHashWrapper<uint32_t> sdbm_hash_32;
    [[maybe_unused]] inline constinit SDBMHashWrapper<uint48_t> sdbm_hash_48;
    [[maybe_unused]] inline constinit SDBMHashWrapper<uint64_t> sdbm_hash_64;

    [[maybe_unused]] inline constinit SipHashWrapper siphash;
    [[maybe_unused]] inline constinit SipHash13Wrapper siphash13;
    [[maybe_unused]] inline constinit SipHashAVX2Wrapper siphash_avx2;
    [[maybe_unused]] inline constinit SipHash13AVX2Wrapper siphash13_avx2;
    [[maybe_unused]] inline constinit HalfSipHashWrapper halfsiphash;

    [[maybe_unused]] inline constinit SpookyHash16Wrapper spooky_hash_16;
    [[maybe_unused]] inline constinit SpookyHash24Wrapper spooky_hash_24;
    [[maybe_unused]] inline constinit SpookyHash32Wrapper spooky_hash_32;
    [[maybe_unused]] inline constinit SpookyHash48Wrapper spooky_hash_48;
    [[maybe_unused]] inline constinit SpookyHash64Wrapper spooky_hash_64;

    [[maybe_unused]] inline constinit T1HA0_32leWrapper t1ha0_32le_hash;
    [[maybe_unused]] inline constinit T1HA0_32beWrapper t1ha0_32be_hash;
    [[maybe_unused]] inline constinit T1HA0_AVX2_Wrapper t1ha0_avx2_hash;
    [[maybe_unused]] inline constinit T1HA1LeWrapper t1ha1_le_hash;
    [[maybe_unused]] inline constinit T1HA1BeWrapper t1ha1_be_hash;
    [[maybe_unused]] inline constinit T1HA2AtonceWrapper t1ha2_atonce_hash;

    [[maybe_unused]] inline constinit xxHash32Wrapper xx_hash_32;
    [[maybe_unused]] inline constinit xxHash64Wrapper xx_hash_64;
    [[maybe_unused]] inline constinit XXH3_64BitsWrapper xxh3_64bits;
    [[maybe_unused]] inline constinit XXH3_64bits_withSeedWrapper xxh3_64bits_with_seed;

    [[maybe_unused]] inline constinit wyHash32Wrapper wyhash32;
    [[maybe_unused]] inline constinit wyHash64Wrapper wy_hash_64;

    namespace detail {
        template<typename UintT>
        struct Hasher {
            using HashType = UintT;
            using HashFunction = const BaseHashWrapper<UintT>&;

            Hasher(std::string name, HashFunction function)
                : name(std::move(name))
                , hash(function) {
            }

            std::string name{};
            HashFunction hash{};
            uint16_t bits {std::numeric_limits<UintT>::digits};
        };
    }

    using Hasher16 = detail::Hasher<uint16_t>;
    using Hasher24 = detail::Hasher<uint24_t>;
    using Hasher32 = detail::Hasher<uint32_t>;
    using Hasher48 = detail::Hasher<uint48_t>;
    using Hasher64 = detail::Hasher<uint64_t>;

    //----------- BuildHashes ----------

    std::vector<Hasher16> Build16bitsHashes();
    std::vector<Hasher24> Build24bitsHashes();
    std::vector<Hasher32> Build32bitsHashes();
    std::vector<Hasher48> Build48bitsHashes();
    std::vector<Hasher64> Build64bitsHashes();

    //uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift = 0);
}

#endif //THESIS_WORK_HASHES_H