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


    }

//----------- CityHashes ----------

    uint32_t CityHash32Wrapper::Hash(const char *message, size_t length) const {
        return city::CityHash32(message, length);
    }

    uint64_t CityHash64Wrapper::Hash(const char *message, size_t length) const {
        return city::CityHash64(message, length);
    }

//----------- FarmHashes ----------

    uint32_t FarmHash32Wrapper::Hash(const char *message, size_t length) const {
        return NAMESPACE_FOR_HASH_FUNCTIONS::Hash32(message, length);
    }

    uint64_t FarmHash64Wrapper::Hash(const char *message, size_t length) const {
        return NAMESPACE_FOR_HASH_FUNCTIONS::Hash64(message, length);
    }

//------------ FastHash ------------

    uint16_t FastHash16Wrapper::Hash(const char *message, size_t length) const {
        uint32_t h = fasthash32(message, length, 0);
        return h - (h >> 16);
    }

    uint24_t FastHash24Wrapper::Hash(const char *message, size_t length) const {
        uint32_t h = fasthash32(message, length, 0);
        return h - (h >> 16);
    }

    uint32_t FastHash32Wrapper::Hash(const char *message, size_t length) const {
        return fasthash32(message, length, 0);
    }

    uint48_t FastHash48Wrapper::Hash(const char *message, size_t length) const {
        uint64_t h = fasthash64(message, length, 0);
        return h - (h >> 32);
    }

    uint64_t FastHash64Wrapper::Hash(const char *message, size_t length) const {
        return fasthash64(message, length, 0);
    }

//---------- FNV-1a hash -----------

    uint16_t FNV1aHash16Wrapper::Hash(const char *message, size_t length) const {
        static const uint32_t mask16 = (((uint32_t)1<<16)-1); /* i.e., (u_int32_t)0xffff */
        uint32_t hash = FNV32a(message, static_cast<int>(length), 0);
        hash = (hash>>16) ^ (hash & mask16);
        return hash;
    }

    uint24_t FNV1aHash24Wrapper::Hash(const char *message, size_t length) const {
        static const uint32_t mask24 = (((uint32_t)1<<24)-1); /* i.e., (u_int32_t)0xffffff */
        uint32_t hash = FNV32a(message, static_cast<int>(length), 0);
        hash = (hash>>24) ^ (hash & mask24);
        return hash;
    }

    uint32_t FNV1aHash32Wrapper::Hash(const char *message, size_t length) const {
        return FNV32a(message, static_cast<int>(length), 0);
    }

    uint48_t FNV1aHash48Wrapper::Hash(const char *message, size_t length) const {
        static const uint64_t mask48 = (((uint64_t)1<<48)-1); /* i.e., (u_int32_t)0xffffff */
        uint64_t hash = FNV64a(message, static_cast<int>(length), 0);
        hash = (hash>>48) ^ (hash & mask48);
        return hash;
    }

    uint64_t FNV1aHash64Wrapper::Hash(const char *message, size_t length) const {
        return FNV64a(message, static_cast<int>(length), 0);
    }

//---------- HighwayHash -----------

    uint64_t HighwayHashWrapper::Hash(const char *message, size_t length) const {
        using namespace highwayhash;

        static const HHKey key HH_ALIGNAS(32) = {1, 2, 3, 4};
        HHStateT<HH_TARGET> state(key);
        HHResult64 result;
        HighwayHashT(&state, message, length, &result);

        return result;
    }


//--------- Jenkins hash -----------

    uint16_t SpookyHash16Wrapper::Hash(const char *message, size_t length) const {
        auto hash = spooky_hash32(message, length, 0);
        return static_cast<uint32_t>(hash);
    }

    uint24_t SpookyHash24Wrapper::Hash(const char *message, size_t length) const {
        auto hash = spooky_hash32(message, length, 0);
        return static_cast<uint24_t>(hash);
    }

    uint32_t SpookyHash32Wrapper::Hash(const char *message, size_t length) const {
        return spooky_hash32(message, length, 0);
    }

    uint48_t SpookyHash48Wrapper::Hash(const char *message, size_t length) const {
        auto hash = spooky_hash64(message, length, 0);
        return static_cast<uint48_t>(hash);
    }

    uint64_t SpookyHash64Wrapper::Hash(const char *message, size_t length) const {
        return spooky_hash64(message, length, 0);
    }

//------------ MetroHash -----------

    uint64_t MetroHash64_Wrapper::Hash(const char *message, size_t length) const {
        uint64_t hash = 0;
        std::vector<uint8_t> hash_array(8, 0);
        MetroHash64::Hash(reinterpret_cast<const uint8_t*>(message), length, hash_array.data(), 0);
        memcpy(&hash, hash_array.data(), 8);
        return hash;
    }

//---------- MurmurHashes ---------

    uint32_t MurmurHash1Wrapper::Hash(const char *message, size_t length) const {
        return MurmurHash1(message, static_cast<int>(length), 0);
    }

    uint32_t MurmurHash2Wrapper::Hash(const char *message, size_t length) const {
        return MurmurHash2(message, static_cast<int>(length), 0);
    }

    uint32_t MurmurHash2AWrapper::Hash(const char *message, size_t length) const {
        return MurmurHash2A(message, static_cast<int>(length), 0);
    }

    uint64_t MurmurHash64AWrapper::Hash(const char *message, size_t length) const {
        return MurmurHash64A(message, static_cast<int>(length), 0);
    }

    uint32_t MurmurHash3Wrapper::Hash(const char *message, size_t length) const {
        uint32_t hash = 0;
        MurmurHash3_x86_32(message, static_cast<int>(length), 0, &hash);
        return hash;
    }


//------------ MUM/mir -------------

    uint64_t MumHashWrapper::Hash(const char *message, size_t length) const {
        return mum_hash(message, length, 0);
    }

    uint64_t MirHashWrapper::Hash(const char *message, size_t length) const {
        return mir_hash(message, length, 0);
    }

//-------------- MX3 ---------------

    uint64_t MX3HashWrapper::Hash(const char *message, size_t length) const {
        const auto* key = reinterpret_cast<const uint8_t*>(message);
        return mx3::hash(key, length, 0);
    }

//------------- NMHASH -------------

    uint32_t nmHash32Wrapper::Hash(const char *message, size_t length) const {
        return NMHASH32(message, length, 0);
    }

    uint32_t nmHash32XWrapper::Hash(const char *message, size_t length) const {
        return NMHASH32X(message, length, 0);
    }

//--- Paul Hsieh's SuperFastHash ---

    uint32_t SuperFastHashWrapper::Hash(const char *message, size_t length) const {
        return SuperFastHash(message, static_cast<int>(length));
    }

//---------- PearsonHashes ---------
    void PearsonHash16::Init() const {
        t16_.resize(table_size_);
        iota(t16_.begin(), t16_.end(), uint16_t(0));
        shuffle(t16_.begin(), t16_.end(), pcg64{});
    }

    uint16_t PearsonHash16::operator()(const char *message, size_t length) const {
        uint16_t hash = 0;
        for (size_t i = 0; i < length; ++i) {
            hash = t16_[hash ^ (mask_ & message[i])];
        }
        return hash;
    }

    uint16_t PearsonHash16::operator()(const std::string& message) const {
        return operator()(message.data(), message.size());
    }


    uint16_t PearsonHash16Wrapper::Hash(const char *message, size_t length) const {
        std::call_once(init_flag_, &PearsonHash16::Init, &hash_);
        return hash_(message, length);
    }

    void PearsonHash24::Init() const {
        t24_.resize(table_size_);
        iota(t24_.begin(), t24_.end(), uint32_t(0));
        shuffle(t24_.begin(), t24_.end(), pcg32{});
    }

    uint24_t PearsonHash24::operator()(const char *message, size_t length) const {
        uint24_t hash = 0;
        for (size_t i = 0; i < length; ++i) {
            uint24_t index = hash ^ (mask_ & message[i]);
            hash = t24_[static_cast<size_t>(index)];
        }
        return hash;
    }

    uint24_t PearsonHash24::operator()(const std::string& message) const {
        return operator()(message.data(), message.size());
    }

    uint24_t PearsonHash24Wrapper::Hash(const char *message, size_t length) const {
        std::call_once(init_flag_, &PearsonHash24::Init, &hash_);
        return hash_(message, length);
    }

    uint32_t PearsonHash32Wrapper::Hash(const char *message, size_t length) const {
        std::call_once(init_flag_, pearson_hash_init);
        return pearson_hash_32(reinterpret_cast<const uint8_t*>(message), length, 0);
    }

    uint64_t PearsonHash64Wrapper::Hash(const char *message, size_t length) const {
        std::call_once(init_flag_, pearson_hash_init);
        return pearson_hash_64(reinterpret_cast<const uint8_t*>(message), length, 0);
    }

//----------- PengyHash ------------

    uint64_t PengyHash64Wrapper::Hash(const char *message, size_t length) const {
        return pengyhash(message, length, 0);
    }

//------------- SIPHASH ------------

    uint64_t SipHashWrapper::Hash(const char *message, size_t length) const {
        return siphash(key_, reinterpret_cast<const uint8_t*>(message), length);
    }

    uint64_t SipHash13Wrapper::Hash(const char *message, size_t length) const {
        return siphash13(key_, reinterpret_cast<const uint8_t*>(message), length);
    }

    uint64_t SipHashAVX2Wrapper::Hash(const char *message, size_t length) const {
        return highwayhash::SipHash(key_, message, length);
    }

    uint64_t SipHash13AVX2Wrapper::Hash(const char *message, size_t length) const {
        return highwayhash::SipHash13(key_, message, length);
    }

    uint32_t HalfSipHashWrapper::Hash(const char *message, size_t length) const {
        return halfsiphash(key_, reinterpret_cast<const uint8_t*>(message), length);
    }

//-------------- T1HA --------------

    uint32_t T1HA0_32leWrapper::Hash(const char *message, size_t length) const {
        return t1ha0_32le(message, length, 0);
    }

    uint32_t T1HA0_32beWrapper::Hash(const char *message, size_t length) const {
        return t1ha0_32be(message, length, 0);
    }

    uint64_t T1HA0_AVX2_Wrapper::Hash(const char *message, size_t length) const {
        return t1ha0_ia32aes_avx2(message, length, 0);
    }


    uint64_t T1HA1LeWrapper::Hash(const char *message, size_t length) const {
        return t1ha1_le(message, length, 0);
    }

    uint64_t T1HA1BeWrapper::Hash(const char *message, size_t length) const {
        return t1ha1_be(message, length, 0);
    }

    uint64_t T1HA2AtonceWrapper::Hash(const char *message, size_t length) const {
        return t1ha2_atonce(message, length, 0);
    }

//------------ wyHashes -----------

    uint32_t wyHash32Wrapper::Hash(const char *message, size_t length) const {
        return wyhash32(message, length, 0);
    }

    uint64_t wyHash64Wrapper::Hash(const char *message, size_t length) const {
        return wyhash(message, length, 0, _wyp);
    }

//------------ xxHashes -----------

    uint32_t xxHash32Wrapper::Hash(const char *message, size_t length) const {
        return XXH32(message, length, 0);
    }

    uint64_t xxHash64Wrapper::Hash(const char *message, size_t length) const {
        return XXH64(message, length, 0);
    }

    uint64_t XXH3_64BitsWrapper::Hash(const char *message, size_t length) const {
        return XXH3_64bits(message, length);
    }
}