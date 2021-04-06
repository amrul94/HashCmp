#include <array>
#include <random>
#include <iostream>

#include "hash_wrappers.h"
#include "hashes/cityhash/city.h"
#include "hashes/farmhash/farmhash.h"
#include "hashes/xx_hash/xxhash.h"
#include "hashes/xx_hash/xxh3.h"
#include "hashes/murmur_hash/MurmurHash1.h"
#include "hashes/murmur_hash/MurmurHash2.h"
#include "hashes/pearson/pearson.h"
#include "hashes/super_fast_hash/super_fast_hash.h"


namespace hash_wrappers {
    namespace {
        std::pair<uint64_t, uint64_t> generateTwoSeeds() {
            std::random_device rd;
            std::default_random_engine dre{rd()};
            std::uniform_int_distribution<uint64_t> di;
            static uint64_t seed1 = di(dre);
            static uint64_t seed2 = di(dre);
            return {seed1, seed2};
        }

        uint64_t generateSeed() {
            return generateTwoSeeds().first;
        }
    }

//----------- CityHashes ----------

    size_t CityHash32Wrapper::operator()(const std::string &str) const {
        return CityHash32(str.c_str(), str.size());
    }

    size_t CityHash64Wrapper::operator()(const std::string &str) const {
        return CityHash64(str.c_str(), str.size());
    }

    size_t CityHash64WithSeedWrapper::operator()(const std::string &str) const {
        return CityHash64WithSeed(str.c_str(), str.size(), generateSeed());
    }


    size_t CityHash64WithSeedsWrapper::operator()(const std::string &str) const {
        const auto[seed0, seed1] = generateTwoSeeds();
        return CityHash64WithSeeds(str.c_str(), str.size(), seed0, seed1);
    }

//----------- FarmHashes ----------

    size_t FarmHash32Wrapper::operator()(const std::string &str) const {
        return util::Hash32(str);
    }

    size_t FarmHash32WithSeedWrapper::operator()(const std::string &str) const {
        return util::Hash32WithSeed(str, generateSeed());
    }

    size_t FarmHash64Wrapper::operator()(const std::string &str) const {
        return util::Hash64(str);
    }

    size_t FarmHash64WithSeedWrapper::operator()(const std::string &str) const {
        return util::Hash64WithSeed(str, generateSeed());
    }

    size_t FarmHash64WithSeedsWrapper::operator()(const std::string &str) const {
        const auto[seed0, seed1] = generateTwoSeeds();
        return util::Hash64WithSeeds(str, seed0, seed1);
    }


//------------ xxHashes -----------

    size_t xxHash32Wrapper::operator()(const std::string &str) const {
        return XXH32(str.data(), str.size(), generateSeed());
    }

    size_t xxHash64Wrapper::operator()(const std::string &str) const {
        return XXH64(str.data(), str.size(), generateSeed());
    }

    size_t XXH3_64bitsWrapper::operator()(const std::string &str) const {
        return XXH3_64bits(str.data(), str.size());
    }

    size_t XXH3_64bits_withSeedWrapper::operator()(const std::string &str) const {
        return XXH3_64bits_withSeed(str.data(), str.size(), generateSeed());
    }

//---------- MurmurHashes ---------

    size_t MurmurHash1Wrapper::operator()(const std::string &str) const {
        return MurmurHash1(str.c_str(), str.size(), generateSeed());
    }

    size_t MurmurHash1AlignedWrapper::operator()(const std::string &str) const {
        return MurmurHash1Aligned(str.c_str(), str.size(), generateSeed());
    }

    size_t MurmurHash2Wrapper::operator()(const std::string &str) const {
        return MurmurHash2(str.c_str(), str.size(), generateSeed());
    }

    size_t MurmurHash64AWrapper::operator()(const std::string &str) const {
        return MurmurHash64A(str.c_str(), str.size(), generateSeed());
    }

    size_t MurmurHash64BWrapper::operator()(const std::string &str) const {
        return MurmurHash64B(str.c_str(), str.size(), generateSeed());
    }

//---------- PearsonHashes ---------

    size_t PearsonHash32Wrapper::operator()(const std::string &str) const {
        void* in = (void*)str.data();
        return pearson_hash_32((const unsigned char*)str.c_str(), str.size(), 0);
    }

    size_t PearsonHash64Wrapper::operator()(const std::string &str) const {

        void* in = (void*)str.data();
        return pearson_hash_64((uint8_t*)in, str.size(), 0);
    }

//--- Paul Hsieh's SuperFastHash ---

    size_t SuperFastHashWrapper::operator()(const std::string &str) const {
        return SuperFastHash(str.data(), str.size());
    }

//----- Rolling Hash (BuzHash) -----

    size_t CyclicHash32Wrapper::operator()(const std::string &str) const {
        return hasher_.hash(str);
    }

    size_t CyclicHash64Wrapper::operator()(const std::string &str) const {
        return hasher_.hash(str);
    }

    size_t RabinKarpHash32Wrapper::operator()(const std::string &str) const {
        return hasher_.hash(str);
    }

    size_t RabinKarpHash64Wrapper::operator()(const std::string &str) const {
        return hasher_.hash(str);
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

        hashes.push_back({"RollingHash (CyclicHash32", CyclicHash32Wrapper{}});
        hashes.push_back({"RollingHash (RabinKarpHash32", RabinKarpHash32Wrapper{}});

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

        hashes.push_back({"RollingHash (CyclicHash64", CyclicHash64Wrapper{}});
        hashes.push_back({"RollingHash (RabinKarpHash64", RabinKarpHash64Wrapper{}});

        return hashes;
    }
}