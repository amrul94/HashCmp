#include <array>
#include <random>

#include "hash_wrappers.h"
#include "hashes/cityhash/city.h"
#include "hashes/farmhash/farmhash.h"
#include "hashes/xx_hash/xxhash.h"
#include "hashes/xx_hash/xxh3.h"
#include "hashes/murmur_hash/MurmurHash1.h"
#include "hashes/murmur_hash/MurmurHash2.h"
#include "hashes/pearson/pearson.h"
#include "hashes/super_fast_hash/super_fast_hash.h"
#include "hashes/other_hash_functions.h"
#include "hashes/t1ha/t1ha.h"
#include "hashes/metro_hash/metrohash64.h"
#include "hashes/fasthash/fasthash.h"
#include "hashes/fnv_hash_32/const_fvn_hash.h"
#include "hashes/fnv_hash_64_128/fnv.h"
#include "hashes/spooky_hash/spooky.h"
#include "hashes/jenkins_hash/lookup3.h"


namespace hashes {
    namespace {
        std::pair<uint64_t, uint64_t> GenerateTwoSeeds() {
            std::random_device rd;
            std::default_random_engine dre{rd()};
            std::uniform_int_distribution<uint64_t> di;
            static uint64_t seed1 = di(dre);
            static uint64_t seed2 = di(dre);
            return {seed1, seed2};
        }

        uint64_t GenerateSeed() {
            return GenerateTwoSeeds().first;
        }
    }

//----------- CityHashes ----------

    uint32_t CityHash32Wrapper::operator()(const std::string &str) const {
        return city::CityHash32(str.data(), str.size());
    }

    uint64_t CityHash64Wrapper::operator()(const std::string &str) const {
        return city::CityHash64(str.data(), str.size());
    }

    uint64_t CityHash64WithSeedWrapper::operator()(const std::string &str) const {
        return city::CityHash64WithSeed(str.data(), str.size(), GenerateSeed());
    }

    uint64_t CityHash64WithSeedsWrapper::operator()(const std::string &str) const {
        const auto [seed0, seed1] = GenerateTwoSeeds();
        return city::CityHash64WithSeeds(str.data(), str.size(), seed0, seed1);
    }

//----- Bernstein's hash djb2 ------

    uint32_t DJB2Hash32Wrapper::operator()(const std::string &str) const {
        return DJB2Hash32(str);
    }

    uint64_t DJB2Hash64Wrapper::operator()(const std::string &str) const {
        return DJB2Hash64(str);
    }

//----------- FarmHashes ----------

    uint32_t FarmHash32Wrapper::operator()(const std::string &str) const {
        return util::Hash32(str);
    }

    uint32_t FarmHash32WithSeedWrapper::operator()(const std::string &str) const {
        return util::Hash32WithSeed(str, GenerateSeed());
    }

    uint64_t FarmHash64Wrapper::operator()(const std::string &str) const {
        return util::Hash64(str);
    }

    uint64_t FarmHash64WithSeedWrapper::operator()(const std::string &str) const {
        return util::Hash64WithSeed(str, GenerateSeed());
    }

    uint64_t FarmHash64WithSeedsWrapper::operator()(const std::string &str) const {
        const auto [seed0, seed1] = GenerateTwoSeeds();
        return util::Hash64WithSeeds(str, seed0, seed1);
    }

//------------ FastHash ------------

    uint32_t FastHash32Wrapper::operator()(const std::string &str) const {
        return fasthash32(str.data(), str.size(), GenerateSeed());
    }

    uint64_t FastHash64Wrapper::operator()(const std::string &str) const {
        return fasthash64(str.data(), str.size(), GenerateSeed());
    }

//---------- FNV-1a hash -----------

    uint32_t FNV1aHash32Wrapper::operator()(const std::string &str) const {
        return hardrock::FnvHash(str.data());
    }

    uint64_t FNV1aHash64Wrapper::operator()(const std::string &str) const {
        return net::QuicUtils::FNV1a_64_Hash(str.data(), static_cast<int>(str.size()));
    }

//--------- Jenkins hash -----------

    uint32_t OneTimeHashWrapper::operator()(const std::string &str) const {
        return jenkins_one_at_a_time_hash((uint8_t*)str.data(), str.size());
    }

    uint32_t Lookup3LittleWrapper::operator()(const std::string &str) const {
        return hashlittle(str.data(), str.size(), GenerateSeed());
    }

    uint32_t Lookup3BigWrapper::operator()(const std::string &str) const {
        return hashbig(str.data(), str.size(), GenerateSeed());
    }

//------------ MetroHash -----------

    namespace {
        template <typename HashFunction>
        uint64_t MetroHashConvert(HashFunction func, const std::string &str) {
            uint64_t hash = 0;
            auto* hash_array =  new uint8_t[8];
            func((uint8_t*)str.data(), str.size(), 0, hash_array);
            memcpy(&hash, hash_array, 8);
            return hash;
        }
    }

    uint64_t MetroHash64_1_Wrapper::operator()(const std::string &str) const {
        return MetroHashConvert(metrohash64_1, str);
    }

    uint64_t MetroHash64_2_Wrapper::operator()(const std::string &str) const {
        return MetroHashConvert(metrohash64_2, str);
    }

//---------- MurmurHashes ---------

    uint32_t MurmurHash1Wrapper::operator()(const std::string &str) const {
        return MurmurHash1(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

    uint32_t MurmurHash1AlignedWrapper::operator()(const std::string &str) const {
        return MurmurHash1Aligned(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

    uint32_t MurmurHash2Wrapper::operator()(const std::string &str) const {
        return MurmurHash2(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

    uint64_t MurmurHash64AWrapper::operator()(const std::string &str) const {
        return MurmurHash64A(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

    uint64_t MurmurHash64BWrapper::operator()(const std::string &str) const {
        return MurmurHash64B(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

//--- Paul Hsieh's SuperFastHash ---

    uint32_t SuperFastHashWrapper::operator()(const std::string &str) const {
        return SuperFastHash(str.data(), static_cast<int>(str.size()));
    }

//---------- PearsonHashes ---------

    PearsonHash32Wrapper::PearsonHash32Wrapper() noexcept{
        pearson_hash_init();
    }

    uint32_t PearsonHash32Wrapper::operator()(const std::string &str) const {
        return pearson_hash_32((const unsigned char*)str.c_str(), str.size(), 0);
    }

    PearsonHash64Wrapper::PearsonHash64Wrapper() noexcept{
        pearson_hash_init();
    }

    uint64_t PearsonHash64Wrapper::operator()(const std::string &str) const {
        return pearson_hash_64((uint8_t*)str.c_str(), str.size(), 0);
    }

//------------ PJW Hash ------------

    uint32_t PJWHash32Wrapper::operator()(const std::string &str) const {
        return PJWHash32(str);
    }

    uint64_t PJWHash64Wrapper::operator()(const std::string &str) const {
        return PJWHash64(str);
    }

//----- Rolling Hash (BuzHash) -----

    uint32_t BuzHash32Wrapper::operator()(const std::string &str) const {
        return hash_function_.hash(str);
    }

    uint64_t BuzHash64Wrapper::operator()(const std::string &str) const {
        return hash_function_.hash(str);
    }

//-------------- SDBM --------------

    uint32_t SDBMHash32Wrapper::operator()(const std::string &str) const {
        return SDBMHash32(str);
    }

    uint64_t SDBMHash64Wrapper::operator()(const std::string &str) const {
        return SDBMHash64(str);
    }

//---------- Spooky hash -----------

    uint32_t SpookyHash32Wrapper::operator()(const std::string &str) const {
        return spooky_hash32(str.data(), str.size(), GenerateSeed());
    }

    uint64_t SpookyHash64Wrapper::operator()(const std::string &str) const {
        return spooky_hash64(str.data(), str.size(), GenerateSeed());
    }

//-------------- T1HA --------------

    uint32_t T1HA0_32leWrapper::operator()(const std::string &str) const {
        return t1ha0_32le(str.data(), str.size(), GenerateSeed());
    }

    uint32_t T1HA0_32beWrapper::operator()(const std::string &str) const {
        return t1ha0_32be(str.data(), str.size(), GenerateSeed());
    }

    uint64_t T1HA1LeWrapper::operator()(const std::string &str) const {
        return t1ha1_le(str.data(), str.size(), GenerateSeed());
    }

    uint64_t T1HA1BeWrapper::operator()(const std::string &str) const {
        return t1ha1_be(str.data(), str.size(), GenerateSeed());
    }

    uint64_t T1HA2AtonceWrapper::operator()(const std::string &str) const {
        return t1ha2_atonce(str.data(), str.size(), GenerateSeed());
    }

//------------ xxHashes -----------

    uint32_t xxHash32Wrapper::operator()(const std::string &str) const {
        return XXH32(str.data(), str.size(), GenerateSeed());
    }

    uint64_t xxHash64Wrapper::operator()(const std::string &str) const {
        return XXH64(str.data(), str.size(), GenerateSeed());
    }

    uint64_t XXH3_64BitsWrapper::operator()(const std::string &str) const {
        return XXH3_64bits(str.data(), str.size());
    }

    uint64_t XXH3_64BitsWithSeedWrapper::operator()(const std::string &str) const {
        return XXH3_64bits_withSeed(str.data(), str.size(), GenerateSeed());
    }
}