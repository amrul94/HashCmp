#include <array>
#include <random>

#include <cityhash/city.h>
#include <farmhash/farmhash.h>
#include <fasthash/fasthash.h>
#include <fnv_hash_32/const_fvn_hash.h>
#include <fnv_hash_64_128/fnv.h>
#include <jenkins_hash/lookup3.h>
#include <murmur_hash/MurmurHash1.h>
#include <murmur_hash/MurmurHash2.h>
#include <pearson/pearson.h>
#include <super_fast_hash/super_fast_hash.h>
#include <t1ha/t1ha.h>
#include <metro_hash/metrohash64.h>

#include <spooky_hash/spooky.h>

#include <xx_hash/xxhash.h>
#include <other_hash_functions.h>

#include "hash_wrappers.h"


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

    [[maybe_unused]] std::string BaseHashWrapper::ReadFile(ifstream& file) {
        std::string result;
        size_t source_size = 0;
        do {
            char buff[1024];
            file.read(buff, sizeof buff);
            size_t read_size = file.gcount();
            source_size += read_size;
            result.append(buff, read_size);
        } while (file);
        return result;
    }

    uint32_t BaseHash32Wrapper::operator()(std::string_view str) const {
        return Hash(str);
    }

    uint32_t BaseHash32Wrapper::operator()(ifstream& file) const {
        std::string binary_file = ReadFile(file);
        assert(!binary_file.empty());
        return Hash(binary_file);
    }

    uint32_t BaseHash32Wrapper::operator()(const img::Image &image) const {
        const char* bytes = reinterpret_cast<const char*>(image.GetLine(0));
        return Hash(bytes);
    }

    uint64_t BaseHash64Wrapper::operator()(std::string_view str) const {
        return Hash(str);
    }
    uint64_t BaseHash64Wrapper::operator()(ifstream& file) const {
        std::string binary_file = ReadFile(file);
        assert(!binary_file.empty());
        return Hash(binary_file);
    }

    uint64_t BaseHash64Wrapper::operator()(const img::Image &image) const {
        const char* bytes = reinterpret_cast<const char*>(image.GetLine(0));
        return Hash(bytes);
    }


//----------- CityHashes ----------

    uint32_t CityHash32Wrapper::Hash(std::string_view str) const {
        return city::CityHash32(str.data(), str.size());
    }

    uint64_t CityHash64Wrapper::Hash(std::string_view str) const {
        return city::CityHash64(str.data(), str.size());
    }

    uint64_t CityHash64WithSeedWrapper::Hash(std::string_view str) const {
        return city::CityHash64WithSeed(str.data(), str.size(), GenerateSeed());
    }

    uint64_t CityHash64WithSeedsWrapper::Hash(std::string_view str) const {
        const auto [seed0, seed1] = GenerateTwoSeeds();
        return city::CityHash64WithSeeds(str.data(), str.size(), seed0, seed1);
    }

//----- Bernstein's hash djb2 ------

    uint32_t DJB2Hash32Wrapper::Hash(std::string_view str) const {
        return DJB2Hash32(str);
    }

    uint64_t DJB2Hash64Wrapper::Hash(std::string_view str) const {
        return DJB2Hash64(str);
    }

//----------- FarmHashes ----------

    uint32_t FarmHash32Wrapper::Hash(std::string_view str) const {
        return util::Hash32(str);
    }

    uint32_t FarmHash32WithSeedWrapper::Hash(std::string_view str) const {
        return util::Hash32WithSeed(str, GenerateSeed());
    }

    uint64_t FarmHash64Wrapper::Hash(std::string_view str) const {
        return util::Hash64(str);
    }

    uint64_t FarmHash64WithSeedWrapper::Hash(std::string_view str) const {
        return util::Hash64WithSeed(str, GenerateSeed());
    }

    uint64_t FarmHash64WithSeedsWrapper::Hash(std::string_view str) const {
        const auto [seed0, seed1] = GenerateTwoSeeds();
        return util::Hash64WithSeeds(str, seed0, seed1);
    }

//------------ FastHash ------------

    uint32_t FastHash32Wrapper::Hash(std::string_view str) const {
        return fasthash32(str.data(), str.size(), GenerateSeed());
    }

    uint64_t FastHash64Wrapper::Hash(std::string_view str) const {
        return fasthash64(str.data(), str.size(), GenerateSeed());
    }

//---------- FNV-1a hash -----------

    uint32_t FNV1aHash32Wrapper::Hash(std::string_view str) const {
        return hardrock::FnvHash(str.data());
    }

    uint64_t FNV1aHash64Wrapper::Hash(std::string_view str) const {
        return net::QuicUtils::FNV1a_64_Hash(str.data(), static_cast<int>(str.size()));
    }

//--------- Jenkins hash -----------

    uint32_t OneTimeHashWrapper::Hash(std::string_view str) const {
        return jenkins_one_at_a_time_hash(reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }

    uint32_t Lookup3LittleWrapper::Hash(std::string_view str) const {
        return hashlittle(str.data(), str.size(), GenerateSeed());
    }

    uint32_t Lookup3BigWrapper::Hash(std::string_view str) const {
        return hashbig(str.data(), str.size(), GenerateSeed());
    }

//------------ MetroHash -----------

    namespace {
        template <typename HashFunction>
        uint64_t MetroHashConvert(HashFunction func, std::string_view str) {
            uint64_t hash = 0;
            auto* hash_array =  new uint8_t[8];
            func(reinterpret_cast<const uint8_t*>(str.data()), str.size(), 0, hash_array);
            memcpy(&hash, hash_array, 8);
            return hash;
        }
    }

    uint64_t MetroHash64_1_Wrapper::Hash(std::string_view str) const {
        return MetroHashConvert(metrohash64_1, str);
    }

    uint64_t MetroHash64_2_Wrapper::Hash(std::string_view str) const {
        return MetroHashConvert(metrohash64_2, str);
    }

//---------- MurmurHashes ---------

    uint32_t MurmurHash1Wrapper::Hash(std::string_view str) const {
        return MurmurHash1(str.data(), static_cast<int>(str.size()), GenerateSeed());
    }

    uint32_t MurmurHash1AlignedWrapper::Hash(std::string_view str) const {
        return MurmurHash1Aligned(str.data(), static_cast<int>(str.size()), GenerateSeed());
    }

    uint32_t MurmurHash2Wrapper::Hash(std::string_view str) const {
        return MurmurHash2(str.data(), static_cast<int>(str.size()), GenerateSeed());
    }

    uint64_t MurmurHash64AWrapper::Hash(std::string_view str) const {
        return MurmurHash64A(str.data(), static_cast<int>(str.size()), GenerateSeed());
    }

    uint64_t MurmurHash64BWrapper::Hash(std::string_view str) const {
        return MurmurHash64B(str.data(), static_cast<int>(str.size()), GenerateSeed());
    }

//--- Paul Hsieh's SuperFastHash ---

    uint32_t SuperFastHashWrapper::Hash(std::string_view str) const {
        return SuperFastHash(str.data(), static_cast<int>(str.size()));
    }

//---------- PearsonHashes ---------

    PearsonHash32Wrapper::PearsonHash32Wrapper() noexcept{
        pearson_hash_init();
    }

    uint32_t PearsonHash32Wrapper::Hash(std::string_view str) const {
        return pearson_hash_32(reinterpret_cast<const uint8_t*>(str.data()), str.size(), 0);
    }

    PearsonHash64Wrapper::PearsonHash64Wrapper() noexcept{
        pearson_hash_init();
    }

    uint64_t PearsonHash64Wrapper::Hash(std::string_view str) const {
        return pearson_hash_64(reinterpret_cast<const uint8_t*>(str.data()), str.size(), 0);
    }

//------------ PJW Hash ------------

    uint32_t PJWHash32Wrapper::Hash(std::string_view str) const {
        return PJWHash32(str);
    }

    uint64_t PJWHash64Wrapper::Hash(std::string_view str) const {
        return PJWHash64(str);
    }

//----- Rolling Hash (BuzHash) -----

    uint32_t BuzHash32Wrapper::Hash(std::string_view str) const {
        return hasher_.hash(str);
    }

    uint64_t BuzHash64Wrapper::Hash(std::string_view str) const {
        return hash_function_.hash(str);
    }

//-------------- SDBM --------------

    uint32_t SDBMHash32Wrapper::Hash(std::string_view str) const {
        return SDBMHash32(str);
    }

    uint64_t SDBMHash64Wrapper::Hash(std::string_view str) const {
        return SDBMHash64(str);
    }

//---------- Spooky hash -----------

    uint32_t SpookyHash32Wrapper::Hash(std::string_view str) const {
        return spooky_hash32(str.data(), str.size(), GenerateSeed());
    }

    uint64_t SpookyHash64Wrapper::Hash(std::string_view str) const {
        return spooky_hash64(str.data(), str.size(), GenerateSeed());
    }

//-------------- T1HA --------------

    uint32_t T1HA0_32leWrapper::Hash(std::string_view str) const {
        return t1ha0_32le(str.data(), str.size(), GenerateSeed());
    }

    uint32_t T1HA0_32beWrapper::Hash(std::string_view str) const {
        return t1ha0_32be(str.data(), str.size(), GenerateSeed());
    }

    uint64_t T1HA1LeWrapper::Hash(std::string_view str) const {
        return t1ha1_le(str.data(), str.size(), GenerateSeed());
    }

    uint64_t T1HA1BeWrapper::Hash(std::string_view str) const {
        return t1ha1_be(str.data(), str.size(), GenerateSeed());
    }

    uint64_t T1HA2AtonceWrapper::Hash(std::string_view str) const {
        return t1ha2_atonce(str.data(), str.size(), GenerateSeed());
    }

//------------ xxHashes -----------

    uint32_t xxHash32Wrapper::Hash(std::string_view str) const {
        return XXH32(str.data(), str.size(), GenerateSeed());
    }

    uint64_t xxHash64Wrapper::Hash(std::string_view str) const {
        return XXH64(str.data(), str.size(), GenerateSeed());
    }

    uint64_t XXH3_64BitsWrapper::Hash(std::string_view str) const {
        return XXH3_64bits(str.data(), str.size());
    }

    uint64_t XXH3_64BitsWithSeedWrapper::Hash(std::string_view str) const {
        return XXH3_64bits_withSeed(str.data(), str.size(), GenerateSeed());
    }
}