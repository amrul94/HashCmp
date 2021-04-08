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


namespace hash_wrappers {
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

    size_t CityHash32Wrapper::operator()(const std::string &str) const {
        return city::CityHash32(str.c_str(), str.size());
    }

    size_t CityHash64Wrapper::operator()(const std::string &str) const {
        return city::CityHash64(str.c_str(), str.size());
    }

    size_t CityHash64WithSeedWrapper::operator()(const std::string &str) const {
        return city::CityHash64WithSeed(str.c_str(), str.size(), GenerateSeed());
    }


    size_t CityHash64WithSeedsWrapper::operator()(const std::string &str) const {
        const auto[seed0, seed1] = GenerateTwoSeeds();
        return city::CityHash64WithSeeds(str.c_str(), str.size(), seed0, seed1);
    }

//----------- FarmHashes ----------

    size_t FarmHash32Wrapper::operator()(const std::string &str) const {
        return util::Hash32(str);
    }

    size_t FarmHash32WithSeedWrapper::operator()(const std::string &str) const {
        return util::Hash32WithSeed(str, GenerateSeed());
    }

    size_t FarmHash64Wrapper::operator()(const std::string &str) const {
        return util::Hash64(str);
    }

    size_t FarmHash64WithSeedWrapper::operator()(const std::string &str) const {
        return util::Hash64WithSeed(str, GenerateSeed());
    }

    size_t FarmHash64WithSeedsWrapper::operator()(const std::string &str) const {
        const auto [seed0, seed1] = GenerateTwoSeeds();
        return util::Hash64WithSeeds(str, seed0, seed1);
    }


//------------ xxHashes -----------

    size_t xxHash32Wrapper::operator()(const std::string &str) const {
        return XXH32(str.data(), str.size(), GenerateSeed());
    }

    size_t xxHash64Wrapper::operator()(const std::string &str) const {
        return XXH64(str.data(), str.size(), GenerateSeed());
    }

    size_t XXH3_64bitsWrapper::operator()(const std::string &str) const {
        return XXH3_64bits(str.data(), str.size());
    }

    size_t XXH3_64bits_withSeedWrapper::operator()(const std::string &str) const {
        return XXH3_64bits_withSeed(str.data(), str.size(), GenerateSeed());
    }

//---------- MurmurHashes ---------

    size_t MurmurHash1Wrapper::operator()(const std::string &str) const {
        return MurmurHash1(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

    size_t MurmurHash1AlignedWrapper::operator()(const std::string &str) const {
        return MurmurHash1Aligned(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

    size_t MurmurHash2Wrapper::operator()(const std::string &str) const {
        return MurmurHash2(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

    size_t MurmurHash64AWrapper::operator()(const std::string &str) const {
        return MurmurHash64A(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

    size_t MurmurHash64BWrapper::operator()(const std::string &str) const {
        return MurmurHash64B(str.c_str(), static_cast<int>(str.size()), GenerateSeed());
    }

//---------- PearsonHashes ---------

    size_t PearsonHash32Wrapper::operator()(const std::string &str) const {
        return pearson_hash_32((const unsigned char*)str.c_str(), str.size(), 0);
    }

    size_t PearsonHash64Wrapper::operator()(const std::string &str) const {
        return pearson_hash_64((uint8_t*)str.c_str(), str.size(), 0);
    }

//--- Paul Hsieh's SuperFastHash ---

    size_t SuperFastHashWrapper::operator()(const std::string &str) const {
        return SuperFastHash(str.data(), static_cast<int>(str.size()));
    }

//----- Rolling Hash (BuzHash) -----

    size_t Buzhash32Wrapper::operator()(const std::string &str) const {
        return hasher_.hash(str);
    }

    size_t Buzhash64Wrapper::operator()(const std::string &str) const {
        return hasher_.hash(str);
    }

//-------------- SDBM --------------

    size_t SDBMHash32Wrapper::operator()(const std::string &str) const {
        return SDBMHash32(str);
    }

    size_t SDBMHash64Wrapper::operator()(const std::string &str) const {
        return SDBMHash64(str);
    }

//-------------- T1HA --------------

    size_t t1ha0_32le_wrapper::operator()(const std::string &str) const {
        return t1ha0_32le(str.data(), str.size(), GenerateSeed());
    }

    size_t t1ha0_32be_wrapper::operator()(const std::string &str) const {
        return t1ha0_32be(str.data(), str.size(), GenerateSeed());
    }

    size_t t1ha1_le_wrapper::operator()(const std::string &str) const {
        return t1ha1_le(str.data(), str.size(), GenerateSeed());
    }

    size_t t1ha1_be_wrapper::operator()(const std::string &str) const {
        return t1ha1_be(str.data(), str.size(), GenerateSeed());
    }

    size_t t1ha2_atonce_wrapper::operator()(const std::string &str) const {
        return t1ha2_atonce(str.data(), str.size(), GenerateSeed());
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

    size_t MetroHash64_1_Wrapper::operator()(const std::string &str) const {
        return MetroHashConvert(metrohash64_1, str);
    }

    size_t MetroHash64_2_Wrapper::operator()(const std::string &str) const {
        return MetroHashConvert(metrohash64_2, str);
    }


//------------ FastHash ------------

    size_t FastHash32Wrapper::operator()(const std::string &str) const {
        return fasthash32(str.data(), str.size(), GenerateSeed());
    }

    size_t FastHash64Wrapper::operator()(const std::string &str) const {
        return fasthash64(str.data(), str.size(), GenerateSeed());
    }

//----- Bernstein's hash djb2 ------

    size_t DJB2Hash32Wrapper::operator()(const std::string &str) const {
        return DJB2Hash32(str);
    }

    size_t DJB2Hash64Wrapper::operator()(const std::string &str) const {
        return DJB2Hash64(str);
    }

//------------ PJW Hash ------------

    size_t PJWHash32Wrapper::operator()(const std::string &str) const {
        return PJWHash32(str);
    }

    size_t PJWHash64Wrapper::operator()(const std::string &str) const {
        return PJWHash64(str);
    }

//---------- FNV-1a hash -----------

    size_t FNV_1a_32Wrapper::operator()(const std::string &str) const {
        return hardrock::FnvHash(str.data());
    }

    size_t FNV_1a_64Wrapper::operator()(const std::string &str) const {
        return net::QuicUtils::FNV1a_64_Hash(str.data(), static_cast<int>(str.size()));
    }

//---------- Spooky hash -----------

    size_t SpookyHash32Wrapper::operator()(const std::string &str) const {
        return spooky_hash32(str.data(), str.size(), GenerateSeed());
    }

    size_t SpookyHash64Wrapper::operator()(const std::string &str) const {
        return spooky_hash64(str.data(), str.size(), GenerateSeed());
    }

    //--------- Jenkins hash -----------

    uint32_t JenkinsOneTimeHashWrapper::operator()(const std::string &str) const {
        return jenkins_one_at_a_time_hash((uint8_t*)str.data(), str.size());
    }

    uint32_t Lookup3LittleWrapper::operator()(const std::string &str) const {
        return hashlittle(str.data(), str.size(), GenerateSeed());
    }

    uint32_t Lookup3BigWrapper::operator()(const std::string &str) const {
        return hashbig(str.data(), str.size(), GenerateSeed());
    }

//----------- BuildHashes ----------

    std::vector<Hasher> Build32bitsHashes() {
        using namespace std::literals;
        std::vector<Hasher> hashes;

        hashes.push_back({"CityHash32", CityHash32Wrapper{}});

        hashes.push_back({"FarmHash32", FarmHash32Wrapper{}});
        hashes.push_back({"FarmHash32WithSeed", FarmHash32WithSeedWrapper{}});

        hashes.push_back({"xxHash32", xxHash32Wrapper{}});


        hashes.push_back({"MurmurHash1", MurmurHash1Wrapper{}});
        hashes.push_back({"MurmurHash1Aligned", MurmurHash1AlignedWrapper{}});
        hashes.push_back({"MurmurHash2", MurmurHash2Wrapper{}});

        pearson_hash_init();
        hashes.push_back({"PearsonHash32", PearsonHash32Wrapper{}});

        hashes.push_back({"Paul Hsieh's SuperFastHash", SuperFastHashWrapper{}});

        hashes.push_back({"RollingHash (BuzHash32)", Buzhash32Wrapper{}});

        hashes.push_back({"SDBMHash32", SDBMHash32Wrapper{}});

        hashes.push_back({"t1ha0_32le", t1ha0_32le_wrapper{}});
        hashes.push_back({"t1ha0_32be", t1ha0_32be_wrapper{}});

        hashes.push_back({"fasthash32", FastHash32Wrapper{}});

        hashes.push_back({"Bernstein's hash djb2 32", DJB2Hash32Wrapper{}});

        hashes.push_back({"PJW Hash 32", PJWHash32Wrapper{}});

        hashes.push_back({"FNV-1a hash 32", FNV_1a_32Wrapper{}});

        hashes.push_back({"Spooky hash 32", SpookyHash32Wrapper{}});

        hashes.push_back({"Jenkins hash (One at a time)", JenkinsOneTimeHashWrapper{}});
        hashes.push_back({"Jenkins hash (lookup3 - little)", Lookup3LittleWrapper{}});
        hashes.push_back({"Jenkins hash (lookup3 - big)", Lookup3BigWrapper{}});
        return hashes;
    }

    std::vector<Hasher> Build64bitsHashes() {
        using namespace std::literals;
        std::vector<Hasher> hashes;

        hashes.push_back({"CityHash64", CityHash64Wrapper{}});
        hashes.push_back({"CityHash64WithSeed", CityHash64WithSeedWrapper{}});
        hashes.push_back({"CityHash64WithSeeds", CityHash64WithSeedsWrapper{}});

        hashes.push_back({"FarmHash64", FarmHash64Wrapper{}});
        hashes.push_back({"FarmHash64WithSeed", FarmHash64WithSeedWrapper{}});
        hashes.push_back({"FarmHash64WithSeeds", FarmHash64WithSeedsWrapper{}});

        hashes.push_back({"xxHash64", xxHash64Wrapper{}});
        hashes.push_back({"XXH3_64bits", XXH3_64bitsWrapper{}});
        hashes.push_back({"XXH3_64bits_withSeed", XXH3_64bits_withSeedWrapper{}});

        hashes.push_back({"MurmurHash64A", MurmurHash64AWrapper{}});
        hashes.push_back({"MurmurHash64B", MurmurHash64BWrapper{}});

        pearson_hash_init();
        hashes.push_back({"PearsonHash64", PearsonHash64Wrapper{}});

        hashes.push_back({"RollingHash (BuzHash64)", Buzhash64Wrapper{}});

        hashes.push_back({"SDBMHash64", SDBMHash64Wrapper{}});

        hashes.push_back({"t1ha1_le", t1ha1_le_wrapper{}});
        hashes.push_back({"t1ha1_be", t1ha1_be_wrapper{}});
        hashes.push_back({"t1ha2_atonce", t1ha2_atonce_wrapper{}});

        hashes.push_back({"metrohash64_1", MetroHash64_1_Wrapper{}});
        hashes.push_back({"metrohash64_2", MetroHash64_2_Wrapper{}});

        hashes.push_back({"fasthash64", FastHash64Wrapper{}});

        hashes.push_back({"Bernstein's hash djb2 64", DJB2Hash64Wrapper{}});

        hashes.push_back({"PJW Hash 64", PJWHash64Wrapper{}});

        hashes.push_back({"FNV-1a hash 64", FNV_1a_64Wrapper{}});

        hashes.push_back({"Spooky hash 64", SpookyHash64Wrapper{}});
        return hashes;
    }
}