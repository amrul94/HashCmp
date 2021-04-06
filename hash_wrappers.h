//
// Created by amrulla on 02.03.2021.
//

#ifndef THESIS_WORK_HASH_WRAPPERS_H
#define THESIS_WORK_HASH_WRAPPERS_H

#include "hashes/rolling_hash/cyclichash.h"
#include "hashes/rolling_hash/rabinkarphash.h"

#include <functional>
#include <string>


namespace hash_wrappers {

    constexpr int WORD_SIZE = 3'000;

    struct Hasher {
        std::string hash_name;
        std::function<size_t(const std::string&)> hash_function;
    };

//----------- CityHashes ----------

    class CityHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class CityHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class CityHash64WithSeedWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class CityHash64WithSeedsWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//----------- FarmHashes ----------

    class FarmHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class FarmHash32WithSeedWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class FarmHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class FarmHash64WithSeedWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };


    class FarmHash64WithSeedsWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//------------ xxHashes -----------

    class xxHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class xxHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class XXH3_64bitsWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class XXH3_64bits_withSeedWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//---------- MurmurHashes ---------

    class MurmurHash1Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class MurmurHash1AlignedWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class MurmurHash2Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class MurmurHash64AWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class MurmurHash64BWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//---------- PearsonHashes ---------

    class PearsonHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class PearsonHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//--- Paul Hsieh's SuperFastHash ---

    class SuperFastHashWrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//----- Rolling Hash (BuzHash) -----

    class CyclicHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;

    private:
        mutable CyclicHash<uint32_t, char> hasher_{WORD_SIZE, 32};
    };

    class CyclicHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;

    private:
        mutable CyclicHash<uint64_t, char> hasher_{WORD_SIZE, 64};
    };

    class RabinKarpHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;

    private:
        mutable KarpRabinHash<uint32_t, char> hasher_{WORD_SIZE, 32};
    };

    class RabinKarpHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;

    private:
        mutable KarpRabinHash<uint64_t, char> hasher_{WORD_SIZE, 64};
    };

//-------------- SDBM --------------

    class SDBMHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class SDBMHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };


//-------------- T1HA --------------

    class t1ha0_32le_wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class t1ha0_32be_wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class t1ha1_le_wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };


    class t1ha1_be_wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };


    class t1ha2_atonce_wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };



//------------ MetroHash -----------

    class MetroHash64_1_Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class MetroHash64_2_Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//----------- BuildHashes ----------

    std::vector<Hasher> Build32bitsHashes();
    std::vector<Hasher> Build64bitsHashes();
}

#endif //THESIS_WORK_HASH_WRAPPERS_H
