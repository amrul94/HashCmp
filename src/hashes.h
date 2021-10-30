#ifndef THESIS_WORK_HASHES_H
#define THESIS_WORK_HASHES_H

#include <concepts>
#include <utility>

#include "hash_wrappers.h"

// HFL = Hash function library
namespace hfl {


    // Hashes
    [[maybe_unused]] inline const BuzHash16Wrapper buz_hash_16;
    [[maybe_unused]] inline const BuzHash24Wrapper buz_hash_24;
    [[maybe_unused]] inline const BuzHash32Wrapper buz_hash_32;
    [[maybe_unused]] inline const BuzHash64Wrapper buz_hash_64;

    [[maybe_unused]] inline constinit CityHash32Wrapper city_hash_32;
    [[maybe_unused]] inline constinit CityHash64Wrapper city_hash_64;

    [[maybe_unused]] inline constinit DJB2Hash16Wrapper djb2_hash_16;
    [[maybe_unused]] inline constinit DJB2Hash24Wrapper djb2_hash_24;
    [[maybe_unused]] inline constinit DJB2Hash32Wrapper djb2_hash_32;
    [[maybe_unused]] inline constinit DJB2Hash64Wrapper djb2_hash_64;

    [[maybe_unused]] inline constinit FarmHash32Wrapper farm_hash_32;
    [[maybe_unused]] inline constinit FarmHash64Wrapper farm_hash_64;

    [[maybe_unused]] inline constinit FastHash16Wrapper fast_hash_16;
    [[maybe_unused]] inline constinit FastHash24Wrapper fast_hash_24;
    [[maybe_unused]] inline constinit FastHash32Wrapper fast_hash_32;
    [[maybe_unused]] inline constinit FastHash64Wrapper fast_hash_64;

    [[maybe_unused]] inline constinit FNV1aHash16Wrapper fnv_1a_hash_16;
    [[maybe_unused]] inline constinit FNV1aHash24Wrapper fnv_1a_hash_24;
    [[maybe_unused]] inline constinit FNV1aHash32Wrapper fnv_1a_hash_32;
    [[maybe_unused]] inline constinit FNV1aHash64Wrapper fnv_1a_hash_64;

    [[maybe_unused]] inline constinit Lookup3LittleWrapper lookup3_little_hash;
    [[maybe_unused]] inline constinit Lookup3BigWrapper lookup_big_hash;

    [[maybe_unused]] inline constinit MetroHash64_Wrapper metro_hash_64;

    [[maybe_unused]] inline constinit MurmurHash1Wrapper murmur_hash1;
    [[maybe_unused]] inline constinit MurmurHash2Wrapper murmur_hash2_32;
    [[maybe_unused]] inline constinit MurmurHash2AWrapper murmur_hash2a_32;
    [[maybe_unused]] inline constinit MurmurHash64AWrapper murmur_hash2_64;
    [[maybe_unused]] inline constinit MurmurHash3Wrapper murmur_hash3;

    [[maybe_unused]] inline constinit OneTimeHash16Wrapper one_at_a_time_hash_16;
    [[maybe_unused]] inline constinit OneTimeHash24Wrapper one_at_a_time_hash_24;
    [[maybe_unused]] inline constinit OneTimeHash32Wrapper one_at_a_time_hash_32;
    [[maybe_unused]] inline constinit OneTimeHash64Wrapper one_at_a_time_hash_64;

    [[maybe_unused]] inline const PearsonHash16Wrapper pearson_hash_16;
    [[maybe_unused]] inline const PearsonHash24Wrapper pearson_hash_24;
    [[maybe_unused]] inline const PearsonHash32Wrapper pearson_hash_32;
    [[maybe_unused]] inline const PearsonHash64Wrapper pearson_hash_64;

    [[maybe_unused]] inline constinit PJWHash16Wrapper pjw_hash_16;
    [[maybe_unused]] inline constinit PJWHash24Wrapper pjw_hash_24;
    [[maybe_unused]] inline constinit PJWHash32Wrapper pjw_hash_32;
    [[maybe_unused]] inline constinit PJWHash64Wrapper pjw_hash_64;

    [[maybe_unused]] inline constinit SuperFastHashWrapper super_fast_hash;

    [[maybe_unused]] inline constinit SDBMHash16Wrapper sdbm_hash_16;
    [[maybe_unused]] inline constinit SDBMHash24Wrapper sdbm_hash_24;
    [[maybe_unused]] inline constinit SDBMHash32Wrapper sdbm_hash_32;
    [[maybe_unused]] inline constinit SDBMHash64Wrapper sdbm_hash_64;

    [[maybe_unused]] inline constinit SpookyHash16Wrapper spooky_hash_16;
    [[maybe_unused]] inline constinit SpookyHash24Wrapper spooky_hash_24;
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

    namespace detail {
        template<typename HashType>
        struct HashStruct {
            using HashFunction = const BaseHashWrapper<HashType>&;
            HashStruct(std::string hash_name, HashFunction hash_function)
                    : hash_name(std::move(hash_name))
                    , hash_function(hash_function) {
            }

            std::string hash_name;
            HashFunction hash_function;
        };

        template<typename SourceUintT, typename DestUintT>
        DestUintT ConvertUint(SourceUintT source) {
            return static_cast<DestUintT>(source);
        }
    }

    using Hash16Struct = detail::HashStruct<uint16_t>;
    using Hash24Struct = detail::HashStruct<uint24_t>;
    using Hash32Struct = detail::HashStruct<uint32_t>;
    using Hash64Struct = detail::HashStruct<uint64_t>;

    inline const auto Uint64ToUint32 = [](uint64_t source) { return detail::ConvertUint<uint64_t, uint32_t>(source); };
    inline const auto Uint64ToUint24 = [](uint64_t source) { return detail::ConvertUint<uint64_t, hfl::uint24_t>(source); };
    inline const auto Uint64ToUint16 = [](uint64_t source) { return detail::ConvertUint<uint64_t, uint16_t>(source); };
    inline const auto Uint32ToUint24 = [](uint32_t source) { return detail::ConvertUint<uint32_t, hfl::uint24_t>(source); };
    inline const auto Uint32ToUint16 = [](uint32_t source) { return detail::ConvertUint<uint32_t, uint16_t>(source); };

    enum class BuildFlag {
        ALL,
        MASK
    };
    //----------- BuildHashes ----------

    std::vector<Hash16Struct> Build16bitsHashes(BuildFlag bf = BuildFlag::ALL);
    std::vector<Hash24Struct> Build24bitsHashes(BuildFlag bf = BuildFlag::ALL);
    std::vector<Hash32Struct> Build32bitsHashes(BuildFlag bf = BuildFlag::ALL);
    std::vector<Hash64Struct> Build64bitsHashes(BuildFlag bf = BuildFlag::ALL);

    uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift = 0);
}

#endif //THESIS_WORK_HASHES_H