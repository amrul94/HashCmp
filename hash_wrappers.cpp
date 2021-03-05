#include <random>
#include <iostream>
#include <array>

#include "hash_wrappers.h"
#include "hashes/cityhash/city.h"
#include "hashes/farmhash/farmhash.h"
#include "hashes/xx_hash/xxhash.h"
#include "hashes/xx_hash/xxh3.h"
#include "hashes/murmur_hash/MurmurHash1.h"
#include "hashes/murmur_hash/MurmurHash2.h"
#include "hashes/murmur_hash/MurmurHash3.h"


namespace {
    std::pair<uint64_t , uint64_t> generateTwoSeeds() {
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

size_t BadHash::operator()(const std::string& str) const {
    return str.size();
}

//----------- CityHashes ----------

size_t CityHash32Wrapper::operator()(const std::string& str) const {
    return CityHash32(str.c_str(), str.size());
}

size_t CityHash64Wrapper::operator()(const std::string& str) const {
    return CityHash64(str.c_str(), str.size());
}

size_t CityHash64WithSeedWrapper::operator()(const std::string& str) const {
    return CityHash64WithSeed(str.c_str(), str.size(), generateSeed());
}


size_t CityHash64WithSeedsWrapper::operator()(const std::string& str) const {
    const auto [seed0, seed1] = generateTwoSeeds();
    return CityHash64WithSeeds(str.c_str(), str.size(), seed0, seed1);
}

size_t CityHash128Wrapper::operator()(const std::string& str) const {
    return Hash128to64(CityHash128(str.c_str(), str.size()));
}


size_t CityHash128WithSeedWrapper::operator()(const std::string& str) const {
    return Hash128to64(CityHash128WithSeed(str.c_str(), str.size(), generateTwoSeeds()));
}

//----------- FarmHashes ----------

size_t FarmHash32Wrapper::operator()(const std::string& str) const {
    return util::Hash32(str);
}

size_t FarmHash32WithSeedWrapper::operator()(const std::string& str) const {
    return util::Hash32WithSeed(str, generateSeed());
}

size_t FarmHash64Wrapper::operator()(const std::string& str) const {
    return util::Hash64(str);
}

size_t FarmHash64WithSeedWrapper::operator()(const std::string& str) const {
    return util::Hash64WithSeed(str, generateSeed());
}

size_t FarmHash64WithSeedsWrapper::operator()(const std::string& str) const {
    const auto [seed0, seed1] = generateTwoSeeds();
    return util::Hash64WithSeeds(str, seed0, seed1);
}

size_t FarmHash128Wrapper::operator()(const std::string& str) const {
    return util::Hash128to64(util::Hash128(str));
}

size_t FarmHash128WithSeedWrapper::operator()(const std::string& str) const {
    return util::Hash128to64(util::Hash128WithSeed(str.c_str(), str.size(), generateTwoSeeds()));
}


//------------ xxHashes -----------

size_t xxHash32Wrapper::operator()(const std::string& str) const {
    return XXH32(str.data(), str.size(), generateSeed());
}

size_t xxHash64Wrapper::operator()(const std::string& str) const {
    return XXH64(str.data(), str.size(), generateSeed());
}

size_t xxHash128Wrapper::operator()(const std::string& str) const {
    const auto [low64, high64] = XXH128(str.c_str(), str.size(), static_cast<XXH64_hash_t>(generateSeed()));
    return util::Hash128to64({low64, high64});
}

size_t XXH3_64bitsWrapper::operator()(const std::string& str) const {
    return XXH3_64bits(str.data(), str.size());
}

size_t XXH3_64bits_withSeedWrapper::operator()(const std::string& str) const {
    return XXH3_64bits_withSeed(str.data(), str.size(), generateSeed());
}

/*
size_t xxH3WithSecretWrapper::operator()(const std::string& str) const {
    std::string secret_buf;
    std::string custom_seed = "custom_seed";
    XXH3_generateSecret(secret_buf.data(), custom_seed.c_str(), custom_seed.size());
    return XXH3_64bits_withSecret(str.c_str(), str.size(), secret_buf.data(), secret_buf.size());
}*/

size_t XXH3_128bitsWrapper::operator()(const std::string& str) const {
    const auto [low64, high64] = XXH3_128bits(str.c_str(), str.size());
    return util::Hash128to64({low64, high64});
}

size_t XXH3_128bits_withSeedWrapper::operator()(const std::string& str) const {
    const auto [low64, high64] = XXH3_128bits_withSeed(str.c_str(), str.size(), generateSeed());
    return util::Hash128to64({low64, high64});
}

//---------- MurmurHashes ---------

size_t MurmurHash1Wrapper::operator()(const std::string& str) const {
    return MurmurHash1(str.c_str(), str.size(), generateSeed());
}

size_t MurmurHash1AlignedWrapper::operator()(const std::string& str) const {
    return MurmurHash1Aligned(str.c_str(), str.size(), generateSeed());
}

size_t MurmurHash2Wrapper::operator()(const std::string& str) const {
    return MurmurHash2(str.c_str(), str.size(), generateSeed());
}

size_t MurmurHash64AWrapper::operator()(const std::string& str) const {
    return MurmurHash64A(str.c_str(), str.size(), generateSeed());
}

size_t MurmurHash64BWrapper::operator()(const std::string& str) const {
    return MurmurHash64B(str.c_str(), str.size(), generateSeed());
}

size_t MurmurHash3_x64_128Wrapper::operator()(const std::string& str) const {
    std::array<uint64_t, 2> hash{};
    MurmurHash3_x64_128(str.c_str(), static_cast<int>(str.size()), generateSeed(), hash.data());
    return util::Hash128to64({hash[0], hash[1]});
}

