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
#include <murmur_hash/MurmurHash3.h>
#include <pearson/pearson.h>
#include <super_fast_hash/super_fast_hash.h>
#include <t1ha/t1ha.h>
#include <metro_hash/metrohash64.h>
#include <spooky_hash/spooky.h>
#include <xx_hash/xxhash.h>
#include <hash_functions.h>

#include "hash_wrappers.h"


namespace hfl {
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

        std::string ReadFile(ifstream& file) {
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
    }

//----------- CityHashes ----------

    uint32_t CityHash32Wrapper::Hash(std::string_view str) const {
        return city::CityHash32(str.data(), str.size());
    }

    uint64_t CityHash64Wrapper::Hash(std::string_view str) const {
        return city::CityHash64(str.data(), str.size());
    }

//----- Bernstein's hash djb2 ------

    uint16_t DJB2Hash16Wrapper::Hash(std::string_view str) const {
        return DJB2Hash<uint16_t>(str);
    }

    uint24_t DJB2Hash24Wrapper::Hash(std::string_view str) const {
        return DJB2Hash<uint24_t>(str);
    }

    uint32_t DJB2Hash32Wrapper::Hash(std::string_view str) const {
        return DJB2Hash<uint32_t>(str);
    }

    uint64_t DJB2Hash64Wrapper::Hash(std::string_view str) const {
        return DJB2Hash<uint64_t>(str);
    }

//----------- FarmHashes ----------

    uint32_t FarmHash32Wrapper::Hash(std::string_view str) const {
        return util::Hash32(str);
    }

    uint64_t FarmHash64Wrapper::Hash(std::string_view str) const {
        return util::Hash64(str);
    }

//------------ FastHash ------------

    uint16_t FastHash16Wrapper::Hash(std::string_view str) const {
        uint32_t h = fasthash32(str.data(), str.size(), 0);
        return h - (h >> 16);
    }

    uint24_t FastHash24Wrapper::Hash(std::string_view str) const {
        uint32_t h = fasthash32(str.data(), str.size(), 0);
        return h - (h >> 16);
    }

    uint32_t FastHash32Wrapper::Hash(std::string_view str) const {
        return fasthash32(str.data(), str.size(), 0);
    }

    uint64_t FastHash64Wrapper::Hash(std::string_view str) const {
        return fasthash64(str.data(), str.size(), 0);
    }

//---------- FNV-1a hash -----------

    uint16_t FNV1aHash16Wrapper::Hash(std::string_view str) const {
        const uint32_t mask16 = (((u_int32_t)1<<16)-1); /* i.e., (u_int32_t)0xffff */
        uint32_t hash = hardrock::FnvHash(str.data());
        hash = (hash>>16) ^ (hash & mask16);
        return hash;
    }

    uint24_t FNV1aHash24Wrapper::Hash(std::string_view str) const {
        const uint32_t mask24 = (((u_int32_t)1<<24)-1); /* i.e., (u_int32_t)0xffffff */
        uint32_t hash = hardrock::FnvHash(str.data());
        hash = (hash>>24) ^ (hash & mask24);
        return hash;
    }

    uint32_t FNV1aHash32Wrapper::Hash(std::string_view str) const {
        return hardrock::FnvHash(str.data());
    }

    uint64_t FNV1aHash64Wrapper::Hash(std::string_view str) const {
        return net::QuicUtils::FNV1a_64_Hash(str.data(), static_cast<int>(str.size()));
    }

//--------- Jenkins hash -----------

    uint16_t OneTimeHash16Wrapper::Hash(std::string_view str) const {
        return one_at_a_time_hash<uint16_t>(reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }

    uint24_t OneTimeHash24Wrapper::Hash(std::string_view str) const {
        return one_at_a_time_hash<uint24_t>(reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }

    uint32_t OneTimeHash32Wrapper::Hash(std::string_view str) const {
        return one_at_a_time_hash<uint32_t>(reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }

    uint64_t OneTimeHash64Wrapper::Hash(std::string_view str) const {
        return one_at_a_time_hash<uint64_t>(reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }

    uint32_t Lookup3LittleWrapper::Hash(std::string_view str) const {
        return hashlittle(str.data(), str.size(), GenerateSeed());
    }

    uint32_t Lookup3BigWrapper::Hash(std::string_view str) const {
        return hashbig(str.data(), str.size(), GenerateSeed());
    }

//------------ MetroHash -----------

    uint64_t MetroHash64_Wrapper::Hash(std::string_view str) const {
        uint64_t hash = 0;
        std::vector<uint8_t> hash_array(8, 0);
        MetroHash64::Hash(reinterpret_cast<const uint8_t*>(str.data()), str.size(), hash_array.data(), 0);
        memcpy(&hash, hash_array.data(), 8);
        return hash;
    }

//---------- MurmurHashes ---------

    uint32_t MurmurHash1Wrapper::Hash(std::string_view str) const {
        return MurmurHash1(str.data(), static_cast<int>(str.size()), 0);
    }

    uint32_t MurmurHash2Wrapper::Hash(std::string_view str) const {
        return MurmurHash2(str.data(), static_cast<int>(str.size()), 0);
    }

    uint32_t MurmurHash2AWrapper::Hash(std::string_view str) const {
        return MurmurHash2A(str.data(), static_cast<int>(str.size()), 0);
    }

    uint64_t MurmurHash64AWrapper::Hash(std::string_view str) const {
        return MurmurHash64A(str.data(), static_cast<int>(str.size()), 0);
    }

    uint32_t MurmurHash3Wrapper::Hash(std::string_view str) const {
        uint32_t hash = 0;
        MurmurHash3_x86_32(str.data(), static_cast<int>(str.size()), 0, &hash);
        return hash;
    }

//--- Paul Hsieh's SuperFastHash ---

    uint32_t SuperFastHashWrapper::Hash(std::string_view str) const {
        return SuperFastHash(str.data(), static_cast<int>(str.size()));
    }

//---------- PearsonHashes ---------

    PearsonHash16Wrapper::PearsonHash16Wrapper() noexcept{
        FillT16();
    }

    void PearsonHash16Wrapper::FillT16() {
        static const uint8_t t[256] ={
                0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
                0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
                0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
                0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
                0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
                0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
                0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
                0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
                0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

        for (size_t i = 0; i < 65536; i++)
            t16[i] = (t[i >> 8] << 8) + t[(uint8_t)i];
    }

    uint16_t PearsonHash16Wrapper::Hash(std::string_view str) const {
        uint16_t hash;
        for (auto c : str) {
            hash = t16[hash ^ (65535 & c)];
        }
        return hash;
    }

    PearsonHash24Wrapper::PearsonHash24Wrapper() noexcept{
        FillT24();
    }

    void PearsonHash24Wrapper::FillT24() {
        iota(t24.begin(), t24.end(), uint32_t(0));
        shuffle(t24.begin(), t24.end(), std::mt19937(std::random_device()()));
    }

    uint24_t PearsonHash24Wrapper::Hash(std::string_view str) const {
        uint24_t hash;
        for (auto c : str) {
            uint32_t index = uint32_t((hash ^ (16'777'215 & c)));
            hash = t24[index];
        }
        return hash;
    }

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

    uint16_t PJWHash16Wrapper::Hash(std::string_view str) const {
        return PJWHash<uint16_t>(str);
    }

    uint24_t PJWHash24Wrapper::Hash(std::string_view str) const {
        return PJWHash<uint32_t>(str);
    }

    uint32_t PJWHash32Wrapper::Hash(std::string_view str) const {
        return PJWHash<uint32_t>(str);
    }

    uint64_t PJWHash64Wrapper::Hash(std::string_view str) const {
        return PJWHash<uint64_t>(str);
    }

//----- Rolling Hash (BuzHash) -----

    uint16_t BuzHash16Wrapper::Hash(std::string_view str) const {
        return hasher_.hash(str);
    }

    uint24_t BuzHash24Wrapper::Hash(std::string_view str) const {
        return hasher_.hash(str);
    }

    uint32_t BuzHash32Wrapper::Hash(std::string_view str) const {
        return hasher_.hash(str);
    }

    uint64_t BuzHash64Wrapper::Hash(std::string_view str) const {
        return hasher_.hash(str);
    }

//-------------- SDBM --------------

    uint16_t SDBMHash16Wrapper::Hash(std::string_view str) const {
        return SDBMHash<uint16_t>(str);
    }

    uint24_t SDBMHash24Wrapper::Hash(std::string_view str) const {
        return SDBMHash<uint24_t>(str);
    }

    uint32_t SDBMHash32Wrapper::Hash(std::string_view str) const {
        return SDBMHash<uint32_t>(str);
    }

    uint64_t SDBMHash64Wrapper::Hash(std::string_view str) const {
        return SDBMHash<uint64_t>(str);
    }

//---------- Spooky hash -----------uint24_t

    uint16_t SpookyHash16Wrapper::Hash(std::string_view str) const {
        auto hash = spooky_hash32(str.data(), str.size(), 0);
        return static_cast<uint32_t>(hash);
    }

    uint24_t SpookyHash24Wrapper::Hash(std::string_view str) const {
        auto hash = spooky_hash32(str.data(), str.size(), 0);
        return static_cast<uint24_t>(hash);
    }

    uint32_t SpookyHash32Wrapper::Hash(std::string_view str) const {
        return spooky_hash32(str.data(), str.size(), 0);
    }

    uint64_t SpookyHash64Wrapper::Hash(std::string_view str) const {
        return spooky_hash64(str.data(), str.size(), 0);
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