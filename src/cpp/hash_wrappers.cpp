#include <array>
#include <random>

#include <cityhash/city.h>
#include <farmhash/farmhash.h>
#include <fasthash/fasthash.h>
#include <highwayhash/highwayhash.h>
#include <murmur_hash/MurmurHash1.h>
#include <murmur_hash/MurmurHash2.h>
#include <murmur_hash/MurmurHash3.h>
#include <mum-mir/mir-hash.h>
#include <mum-mir/mum.h>
#include <mx3/mx3.h>
#include <pearson/pearson.h>
#include <super_fast_hash/super_fast_hash.h>
#include <t1ha/t1ha.h>
#include <metro_hash/metrohash64.h>
#include <nmhash/nmhash.h>
#include <pengyhash/pengyhash.h>
#include <siphash/siphash_impl.h>
#include <spooky_hash/spooky.h>
#include <xx_hash/xxhash.h>
#include <wyhash/wyhash.h>
#include <wyhash/wyhash32.h>

#include <pcg_random.hpp>

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

        std::string ReadFile(std::ifstream& file) {
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

//----------- FarmHashes ----------

    uint32_t FarmHash32Wrapper::Hash(std::string_view str) const {
        return NAMESPACE_FOR_HASH_FUNCTIONS::Hash32(str);
    }

    uint64_t FarmHash64Wrapper::Hash(std::string_view str) const {
        return NAMESPACE_FOR_HASH_FUNCTIONS::Hash64(str);
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

    uint48_t FastHash48Wrapper::Hash(std::string_view str) const {
        uint64_t h = fasthash64(str.data(), str.size(), 0);
        return h - (h >> 32);
    }

    uint64_t FastHash64Wrapper::Hash(std::string_view str) const {
        return fasthash64(str.data(), str.size(), 0);
    }

//---------- FNV-1a hash -----------

    uint16_t FNV1aHash16Wrapper::Hash(std::string_view str) const {
        static const uint32_t mask16 = (((uint32_t)1<<16)-1); /* i.e., (u_int32_t)0xffff */
        const auto len = static_cast<int>(str.size());
        uint32_t hash = FNV32a(str.data(), len, 0);
        hash = (hash>>16) ^ (hash & mask16);
        return hash;
    }

    uint24_t FNV1aHash24Wrapper::Hash(std::string_view str) const {
        static const uint32_t mask24 = (((uint32_t)1<<24)-1); /* i.e., (u_int32_t)0xffffff */
        const auto len = static_cast<int>(str.size());
        uint32_t hash = FNV32a(str.data(), len, 0);
        hash = (hash>>24) ^ (hash & mask24);
        return hash;
    }

    uint32_t FNV1aHash32Wrapper::Hash(std::string_view str) const {
        const auto len = static_cast<int>(str.size());
        return FNV32a(str.data(), len, 0);
    }

    uint48_t FNV1aHash48Wrapper::Hash(std::string_view str) const {
        static const uint64_t mask48 = (((uint64_t)1<<48)-1); /* i.e., (u_int32_t)0xffffff */
        const auto len = static_cast<int>(str.size());
        uint64_t hash = FNV64a(str.data(), len, 0);
        hash = (hash>>48) ^ (hash & mask48);
        return hash;
    }

    uint64_t FNV1aHash64Wrapper::Hash(std::string_view str) const {
        const auto len = static_cast<int>(str.size());
        return FNV64a(str.data(), len, 0);
    }

//---------- HighwayHash -----------

    uint64_t HighwayHashWrapper::Hash(std::string_view str) const {
        using namespace highwayhash;

        static const HHKey key HH_ALIGNAS(32) = {1, 2, 3, 4};
        HHStateT<HH_TARGET> state(key);
        HHResult64 result;
        HighwayHashT(&state, str.data(), str.size(), &result);

        return result;
    }


//--------- Jenkins hash -----------

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

    uint48_t SpookyHash48Wrapper::Hash(std::string_view str) const {
        auto hash = spooky_hash64(str.data(), str.size(), 0);
        return static_cast<uint48_t>(hash);
    }

    uint64_t SpookyHash64Wrapper::Hash(std::string_view str) const {
        return spooky_hash64(str.data(), str.size(), 0);
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


//------------ MUM/mir -------------

    uint64_t MumHashWrapper::Hash(std::string_view str) const {
        return mum_hash(str.data(), str.size(), 0);
    }

    uint64_t MirHashWrapper::Hash(std::string_view str) const {
        return mir_hash(str.data(), str.size(), 0);
    }

//-------------- MX3 ---------------

    uint64_t MX3HashWrapper::Hash(std::string_view str) const {
        const auto* key = reinterpret_cast<const uint8_t*>(str.data());
        return mx3::hash(key, str.size(), 0);
    }

//------------- NMHASH -------------

    uint32_t nmHash32Wrapper::Hash(std::string_view str) const {
        return NMHASH32(str.data(), str.size(), 0);
    }

    uint32_t nmHash32XWrapper::Hash(std::string_view str) const {
        return NMHASH32X(str.data(), str.size(), 0);
    }

//--- Paul Hsieh's SuperFastHash ---

    uint32_t SuperFastHashWrapper::Hash(std::string_view str) const {
        return SuperFastHash(str.data(), static_cast<int>(str.size()));
    }

//---------- PearsonHashes ---------
    void PearsonHash16::Init() const {
        t16_.resize(table_size_);
        iota(t16_.begin(), t16_.end(), uint16_t(0));
        shuffle(t16_.begin(), t16_.end(), pcg64{});
    }

    uint16_t PearsonHash16::operator()(std::string_view str) const {
        uint16_t hash;
        for (auto c : str) {
            hash = t16_[hash ^ (65535 & c)];
        }
        return hash;
    }

    uint16_t PearsonHash16Wrapper::Hash(std::string_view str) const {
        std::call_once(init_flag_, &PearsonHash16::Init, &hash_);
        return hash_(str);
    }

    void PearsonHash24::Init() const {
        t24_.resize(table_size_);
        iota(t24_.begin(), t24_.end(), uint32_t(0));
        shuffle(t24_.begin(), t24_.end(), pcg64{});
    }

    uint24_t PearsonHash24::operator()(std::string_view str) const {
        uint24_t hash;
        for (auto c : str) {
            uint24_t index = hash ^ (mask_ & c);
            hash = t24_[static_cast<size_t>(index)];
        }
        return hash;
    }

    uint24_t PearsonHash24Wrapper::Hash(std::string_view str) const {
        std::call_once(init_flag_, &PearsonHash24::Init, &hash_);
        return hash_(str);
    }

    uint32_t PearsonHash32Wrapper::Hash(std::string_view str) const {
        std::call_once(init_flag_, pearson_hash_init);
        return pearson_hash_32(reinterpret_cast<const uint8_t*>(str.data()), str.size(), 0);
    }

    uint64_t PearsonHash64Wrapper::Hash(std::string_view str) const {
        std::call_once(init_flag_, pearson_hash_init);
        return pearson_hash_64(reinterpret_cast<const uint8_t*>(str.data()), str.size(), 0);
    }

//----------- PengyHash ------------

    uint64_t PengyHash64Wrapper::Hash(std::string_view str) const {
        return pengyhash(str.data(), str.size(), 0);
    }

//------------- SIPHASH ------------

    uint64_t SipHashWrapper::Hash(std::string_view str) const {
        return siphash(key_, (const unsigned char *)str.data(), str.size());
    }

    uint64_t SipHash13Wrapper::Hash(std::string_view str) const {
        return siphash13(key_, (const unsigned char *)str.data(), str.size());
    }

    uint64_t SipHashAVX2Wrapper::Hash(std::string_view str) const {
        return highwayhash::SipHash(key_, str.data(), str.size());
    }

    uint64_t SipHash13AVX2Wrapper::Hash(std::string_view str) const {
        return highwayhash::SipHash13(key_, str.data(), str.size());
    }

    uint32_t HalfSipHashWrapper::Hash(std::string_view str) const {
        return halfsiphash(key_, (const unsigned char *)str.data(), str.size());
    }

//-------------- T1HA --------------

    uint32_t T1HA0_32leWrapper::Hash(std::string_view str) const {
        return t1ha0_32le(str.data(), str.size(), 0);
    }

    uint32_t T1HA0_32beWrapper::Hash(std::string_view str) const {
        return t1ha0_32be(str.data(), str.size(), 0);
    }

    uint64_t T1HA0_AVX2_Wrapper::Hash(std::string_view str) const {
        return t1ha0_ia32aes_avx2(str.data(), str.size(), 0);
    }


    uint64_t T1HA1LeWrapper::Hash(std::string_view str) const {
        return t1ha1_le(str.data(), str.size(), 0);
    }

    uint64_t T1HA1BeWrapper::Hash(std::string_view str) const {
        return t1ha1_be(str.data(), str.size(), 0);
    }

    uint64_t T1HA2AtonceWrapper::Hash(std::string_view str) const {
        return t1ha2_atonce(str.data(), str.size(), 0);
    }

//------------ wyHashes -----------

    uint32_t wyHash32Wrapper::Hash(std::string_view str) const {
        return wyhash32(str.data(), str.size(), 0);
    }

    uint64_t wyHash64Wrapper::Hash(std::string_view str) const {
        return wyhash(str.data(), str.size(), 0, _wyp);
    }

//------------ xxHashes -----------

    uint32_t xxHash32Wrapper::Hash(std::string_view str) const {
        return XXH32(str.data(), str.size(), 0);
    }

    uint64_t xxHash64Wrapper::Hash(std::string_view str) const {
        return XXH64(str.data(), str.size(), 0);
    }

    uint64_t XXH3_64BitsWrapper::Hash(std::string_view str) const {
        return XXH3_64bits(str.data(), str.size());
    }

    uint64_t XXH3_64BitsWithSeedWrapper::Hash(std::string_view str) const {
        return XXH3_64bits_withSeed(str.data(), str.size(), GenerateSeed());
    }
}