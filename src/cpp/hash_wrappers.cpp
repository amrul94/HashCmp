#include <random>

#include <cityhash/city.h>
#include <farmhash/farmhash.h>
#include <fasthash/fasthash.h>
#include <highwayhash/sip_hash.h>
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
#include "generators.h"

namespace hfl::wrappers {

    constexpr uint8_t shift16 = 16;
    constexpr uint8_t shift24 = 24;
    constexpr uint8_t shift32 = 32;
    constexpr uint8_t shift48 = 32;

    //----------- CityHashes ----------

    uint32_t CityHash32Wrapper::HashImpl(const char *message, size_t length) const {
        return city::CityHash32(message, length);
    }

    uint64_t CityHash64Wrapper::HashImpl(const char *message, size_t length) const {
        return city::CityHash64(message, length);
    }

    uint64_t CityHash64WithSeedWrapper::HashImpl(const char *message, size_t length) const {
        return city::CityHash64WithSeed(message, length, SEED_64_1);
    }

    uint64_t CityHash64WithSeedsWrapper::HashImpl(const char *message, size_t length) const {
        return city::CityHash64WithSeeds(message, length, SEED_64_1, SEED_64_2);
    }

//----------- FarmHashes ----------

    uint32_t FarmHash32Wrapper::HashImpl(const char *message, size_t length) const {
        return NAMESPACE_FOR_HASH_FUNCTIONS::Hash32(message, length);
    }

    uint32_t FarmHash32WithSeedWrapper::HashImpl(const char *message, size_t length) const {
        return NAMESPACE_FOR_HASH_FUNCTIONS::Hash32WithSeed(message, length, SEED_32);
    }

    uint64_t FarmHash64Wrapper::HashImpl(const char *message, size_t length) const {
        return NAMESPACE_FOR_HASH_FUNCTIONS::Hash64(message, length);
    }

    uint64_t FarmHash64WithSeedWrapper::HashImpl(const char *message, size_t length) const {
        return NAMESPACE_FOR_HASH_FUNCTIONS::Hash64WithSeed(message, length, SEED_64_1);
    }

    uint64_t FarmHash64WithSeedsWrapper::HashImpl(const char *message, size_t length) const {
        return NAMESPACE_FOR_HASH_FUNCTIONS::Hash64WithSeeds(message, length, SEED_64_1, SEED_64_2);
    }

//------------ FastHash ------------

    uint16_t FastHash16Wrapper::HashImpl(const char *message, size_t length) const {
        uint32_t h = fasthash32(message, length, SEED_32);
        return h - (h >> shift16);
    }

    uint24_t FastHash24Wrapper::HashImpl(const char *message, size_t length) const {
        uint32_t h = fasthash32(message, length, SEED_32);
        return h - (h >> shift16);
    }

    uint32_t FastHash32Wrapper::HashImpl(const char *message, size_t length) const {
        return fasthash32(message, length, SEED_32);
    }

    uint48_t FastHash48Wrapper::HashImpl(const char *message, size_t length) const {
        uint64_t h = fasthash64(message, length, SEED_64_1);
        return h - (h >> shift32);
    }

    uint64_t FastHash64Wrapper::HashImpl(const char *message, size_t length) const {
        return fasthash64(message, length, SEED_64_1);
    }

//---------- FNV-1a hash -----------

    uint16_t FNV1aHash16Wrapper::HashImpl(const char *message, size_t length) const {
        static const uint32_t mask16 = (((uint32_t)1<<16)-1); /* i.e., (u_int32_t)0xffff */
        uint32_t hash = FNV32a(message, static_cast<int>(length), SEED_32);
        hash = (hash >> shift16) ^ (hash & mask16);
        return hash;
    }

    uint24_t FNV1aHash24Wrapper::HashImpl(const char *message, size_t length) const {
        static const uint32_t mask24 = (((uint32_t)1<<24)-1);
        uint32_t hash = FNV32a(message, static_cast<int>(length), SEED_32);
        hash = (hash >> shift24) ^ (hash & mask24);
        return hash;
    }

    uint32_t FNV1aHash32Wrapper::HashImpl(const char *message, size_t length) const {
        return FNV32a(message, static_cast<int>(length), SEED_32);
    }

    uint48_t FNV1aHash48Wrapper::HashImpl(const char *message, size_t length) const {
        static const uint64_t mask48 = (((uint64_t)1<<48)-1);
        uint64_t hash = FNV64a(message, static_cast<int>(length), SEED_64_1);
        hash = (hash >> shift48) ^ (hash & mask48);
        return hash;
    }

    uint64_t FNV1aHash64Wrapper::HashImpl(const char *message, size_t length) const {
        return FNV64a(message, static_cast<int>(length), SEED_64_1);
    }

//---------- HighwayHash -----------

    uint64_t HighwayHashWrapper::HashImpl(const char *message, size_t length) const {
        static const std::array key HH_ALIGNAS(32) = {SEED_64_1, SEED_64_2, SEED_64_3, SEED_64_4};
        highwayhash::HHStateT<HH_TARGET> state{key.data()};
        highwayhash::HHResult64 result = 0;
        highwayhash::HighwayHashT(&state, message, length, &result);
        return result;
    }


//--------- Jenkins hash -----------

    uint16_t SpookyHash16Wrapper::HashImpl(const char *message, size_t length) const {
        auto hash = spooky_hash32(message, length, SEED_32);
        return static_cast<uint32_t>(hash);
    }

    uint24_t SpookyHash24Wrapper::HashImpl(const char *message, size_t length) const {
        auto hash = spooky_hash32(message, length, SEED_32);
        return static_cast<uint24_t>(hash);
    }

    uint32_t SpookyHash32Wrapper::HashImpl(const char *message, size_t length) const {
        return spooky_hash32(message, length, SEED_32);
    }

    uint48_t SpookyHash48Wrapper::HashImpl(const char *message, size_t length) const {
        auto hash = spooky_hash64(message, length, SEED_64_1);
        return static_cast<uint48_t>(hash);
    }

    uint64_t SpookyHash64Wrapper::HashImpl(const char *message, size_t length) const {
        return spooky_hash64(message, length, SEED_64_1);
    }

//------------ MetroHash -----------

    uint64_t MetroHash64_Wrapper::HashImpl(const char *message, size_t length) const {
        uint64_t hash = 0;
        static const size_t bytes_in_hash = 8;
        std::vector<uint8_t> hash_array(bytes_in_hash, 0);
        MetroHash64::Hash(reinterpret_cast<const uint8_t*>(message), length, hash_array.data(), SEED_64_1);
        memcpy(&hash, hash_array.data(), bytes_in_hash);
        return hash;
    }

//---------- MurmurHashes ---------

    uint32_t MurmurHash1Wrapper::HashImpl(const char *message, size_t length) const {
        return MurmurHash1(message, static_cast<int>(length), SEED_32);
    }

    uint32_t MurmurHash2Wrapper::HashImpl(const char *message, size_t length) const {
        return MurmurHash2(message, static_cast<int>(length), SEED_32);
    }

    uint32_t MurmurHash2AWrapper::HashImpl(const char *message, size_t length) const {
        return MurmurHash2A(message, static_cast<int>(length), SEED_32);
    }

    uint64_t MurmurHash64AWrapper::HashImpl(const char *message, size_t length) const {
        return MurmurHash64A(message, static_cast<int>(length), SEED_64_1);
    }

    uint32_t MurmurHash3Wrapper::HashImpl(const char *message, size_t length) const {
        uint32_t hash = 0;
        MurmurHash3_x86_32(message, static_cast<int>(length), SEED_32, &hash);
        return hash;
    }


//------------ MUM/mir -------------

    uint64_t MumHashWrapper::HashImpl(const char *message, size_t length) const {
        return mum_hash(message, length, SEED_64_1);
    }

    uint64_t MirHashWrapper::HashImpl(const char *message, size_t length) const {
        return mir_hash(message, length, SEED_64_1);
    }

//-------------- MX3 ---------------

    uint64_t MX3HashWrapper::HashImpl(const char *message, size_t length) const {
        const auto* key = reinterpret_cast<const uint8_t*>(message);
        return mx3::hash(key, length, SEED_64_1);
    }

//------------- NMHASH -------------

    uint32_t nmHash32Wrapper::HashImpl(const char *message, size_t length) const {
        return NMHASH32(message, length, SEED_32);
    }

    uint32_t nmHash32XWrapper::HashImpl(const char *message, size_t length) const {
        return NMHASH32X(message, length, SEED_32);
    }

//--- Paul Hsieh's SuperFastHash ---

    uint32_t SuperFastHashWrapper::HashImpl(const char *message, size_t length) const {
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
        std::string_view str(message, length);
        for (size_t i = 0; i < length; ++i) {
            hash = t16_[hash ^ (mask_ & str[i])];
        }
        return hash;
    }

    uint16_t PearsonHash16::operator()(const std::string& message) const {
        return operator()(message.data(), message.size());
    }


    uint16_t PearsonHash16Wrapper::HashImpl(const char *message, size_t length) const {
        std::call_once(init_flag_, &PearsonHash16::Init, &hash_);
        return hash_(message, length);
    }

    void PearsonHash24::Init() const {
        t12_.resize(table_size_);
        iota(t12_.begin(), t12_.end(), uint16_t(0));
        shuffle(t12_.begin(), t12_.end(), pcg32{});
    }

    uint24_t PearsonHash24::operator()(const char *message, size_t length) const {
        uint24_t hash = 0;
        std::string_view str(message, length);
        for (size_t i = 0; i < length; ++i) {
            uint24_t c = bits_mask_ & str[i];
            c |= c <<  shift6_;
            c |= c << shift12_;
            hash ^= c ^ hash_mask_;
            uint32_t upper_index = static_cast<uint32_t>(hash >> shift12_);
            uint32_t lower_index = static_cast<uint32_t>(bits_mask_ & hash);
            hash = (t12_[upper_index] << shift12_) + t12_[lower_index];
        }
        return hash;
    }

    uint24_t PearsonHash24::operator()(const std::string& message) const {
        return operator()(message.data(), message.size());
    }

    uint24_t PearsonHash24Wrapper::HashImpl(const char *message, size_t length) const {
        std::call_once(init_flag_, &PearsonHash24::Init, &hash_);
        return hash_(message, length);
    }

    uint32_t PearsonHash32Wrapper::HashImpl(const char *message, size_t length) const {
        std::call_once(init_flag_, pearson_hash_init);
        return pearson_hash_32(reinterpret_cast<const uint8_t*>(message), length, SEED_32);
    }

    void PearsonHash48::Init() const {
        t12_.resize(table_size_);
        iota(t12_.begin(), t12_.end(), uint16_t(0));
        shuffle(t12_.begin(), t12_.end(), pcg32{});
    }

    uint48_t PearsonHash48::ROR48(const uint48_t& h) const {
        constexpr uint16_t inv_shift12 = 48;
        return (h >> shift12_) | (h << (inv_shift12 - shift12_));
    }

    uint48_t PearsonHash48::operator()(const char *message, size_t length) const {
        uint48_t hash = 0;
        std::string_view str(message, length);
        for (size_t i = 0; i < length; ++i) {
            uint48_t c = bits_mask_ & str[i];
            c |= c <<  shift6_;
            c |= c << shift12_;
            c |= c << shift24_;
            hash ^= c ^ hash_mask_;
            uint48_t h{};
            uint12_t x{};

            x = static_cast<uint12_t>(hash);
            x = t12_[static_cast<uint16_t>(x)];
            hash >>= shift12_;
            h = x;
            h = ROR48(h);

            x = static_cast<uint12_t>(hash);
            x = t12_[static_cast<uint16_t>(x)];
            hash >>= shift12_;
            h |= x;
            h = ROR48(h);

            x = static_cast<uint12_t>(hash);
            x = t12_[static_cast<uint16_t>(x)];
            hash >>= shift12_;
            h |= x;
            h = ROR48(h);

            x = static_cast<uint12_t>(hash);
            x = t12_[static_cast<uint16_t>(x)];
            h |= x;
            h = ROR48(h);
            hash = h;
        }
        return hash;
    }

    uint48_t PearsonHash48::operator()(const std::string& message) const {
        return operator()(message.data(), message.size());
    }

    uint48_t PearsonHash48Wrapper::HashImpl(const char *message, size_t length) const {
        std::call_once(init_flag_, &PearsonHash48::Init, &hash_);
        return hash_(message, length);
    }

    uint64_t PearsonHash64Wrapper::HashImpl(const char *message, size_t length) const {
        std::call_once(init_flag_, pearson_hash_init);
        return pearson_hash_64(reinterpret_cast<const uint8_t*>(message), length, SEED_64_1);
    }

//----------- PengyHash ------------

    uint64_t PengyHash64Wrapper::HashImpl(const char *message, size_t length) const {
        return pengyhash(message, length, SEED_32);
    }

//------------- SIPHASH ------------

    uint64_t SipHashWrapper::HashImpl(const char *message, size_t length) const {
        static const std::array key = {SEED_8_1, SEED_8_2, SEED_8_3, SEED_8_4,
                                       SEED_8_5, SEED_8_6, SEED_8_7, SEED_8_8,
                                       SEED_8_9, SEED_8_10, SEED_8_11, SEED_8_12,
                                       SEED_8_13, SEED_8_14, SEED_8_15, SEED_8_16};
        return siphash(key.data(), reinterpret_cast<const uint8_t*>(message), length);
    }

    uint64_t SipHash13Wrapper::HashImpl(const char *message, size_t length) const {
        static const std::array key = {SEED_8_1, SEED_8_2, SEED_8_3, SEED_8_4,
                                       SEED_8_5, SEED_8_6, SEED_8_7, SEED_8_8,
                                       SEED_8_9, SEED_8_10, SEED_8_11, SEED_8_12,
                                       SEED_8_13, SEED_8_14, SEED_8_15, SEED_8_16};
        return siphash13(key.data(), reinterpret_cast<const uint8_t*>(message), length);
    }

    uint64_t SipHashAVX2Wrapper::HashImpl(const char *message, size_t length) const {
        static const highwayhash::HH_U64 key[2] = {SEED_64_1, SEED_64_2};
        return highwayhash::SipHash(key, message, length);
    }

    uint64_t SipHash13AVX2Wrapper::HashImpl(const char *message, size_t length) const {
        static const highwayhash::HH_U64 key[2] = {SEED_64_1, SEED_64_2};
        return highwayhash::SipHash13(key, message, length);
    }

    uint32_t HalfSipHashWrapper::HashImpl(const char *message, size_t length) const {
        static const std::array key = {SEED_8_1, SEED_8_2, SEED_8_3, SEED_8_4,
                                       SEED_8_5, SEED_8_6, SEED_8_7, SEED_8_8,
                                       SEED_8_9, SEED_8_10, SEED_8_11, SEED_8_12,
                                       SEED_8_13, SEED_8_14, SEED_8_15, SEED_8_16};
        return halfsiphash(key.data(), reinterpret_cast<const uint8_t*>(message), length);
    }

//-------------- T1HA --------------

    uint32_t T1HA0_32leWrapper::HashImpl(const char *message, size_t length) const {
        return t1ha0_32le(message, length, SEED_64_1);
    }

    uint32_t T1HA0_32beWrapper::HashImpl(const char *message, size_t length) const {
        return t1ha0_32be(message, length, SEED_64_1);
    }

    uint64_t T1HA0_AVX2_Wrapper::HashImpl(const char *message, size_t length) const {
        return t1ha0_ia32aes_avx2(message, length, SEED_64_1);
    }


    uint64_t T1HA1LeWrapper::HashImpl(const char *message, size_t length) const {
        return t1ha1_le(message, length, SEED_64_1);
    }

    uint64_t T1HA1BeWrapper::HashImpl(const char *message, size_t length) const {
        return t1ha1_be(message, length, SEED_64_1);
    }

    uint64_t T1HA2AtonceWrapper::HashImpl(const char *message, size_t length) const {
        return t1ha2_atonce(message, length, SEED_64_1);
    }

//------------ wyHashes -----------

    uint32_t wyHash32Wrapper::HashImpl(const char *message, size_t length) const {
        return wyhash32(message, length, SEED_32);
    }

    uint64_t wyHash64Wrapper::HashImpl(const char *message, size_t length) const {
        return wyhash(message, length, SEED_64_1, _wyp);
    }

//------------ xxHashes -----------

    uint32_t xxHash32Wrapper::HashImpl(const char *message, size_t length) const {
        return XXH32(message, length, 0);
    }

    uint64_t xxHash64Wrapper::HashImpl(const char *message, size_t length) const {
        return XXH64(message, length, 0);
    }

    uint64_t XXH3_64BitsWrapper::HashImpl(const char *message, size_t length) const {
        return XXH3_64bits(message, length);
    }

    uint64_t XXH3_64bits_withSeedWrapper::HashImpl(const char *message, size_t length) const {
        return XXH3_64bits_withSeed(message, length, SEED_64_1);
    }
}