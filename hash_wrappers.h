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

    constexpr int WORD_SIZE = 1000;

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

    class Buzhash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;

    private:
        mutable CyclicHash<uint32_t, char> hasher_{WORD_SIZE, 32};
    };

    class Buzhash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;

    private:
        mutable CyclicHash<uint64_t, char> hasher_{WORD_SIZE, 64};
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


//------------ FastHash ------------
    class FastHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class FastHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//----- Bernstein's hash djb2 ------

    class DJB2Hash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class DJB2Hash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//------------ PJW Hash ------------

    class PJWHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class PJWHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//---------- FNV-1a hash -----------

    class FNV_1a_32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class FNV_1a_64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//---------- Spooky hash -----------

    class SpookyHash32Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

    class SpookyHash64Wrapper {
    public:
        size_t operator()(const std::string &str) const;
    };

//--------- Jenkins hash -----------

    class JenkinsOneTimeHashWrapper {
    public:
        uint32_t operator()(const std::string &str) const;
    };

    class Lookup3LittleWrapper {
    public:
        uint32_t operator()(const std::string &str) const;
    };

    class Lookup3BigWrapper {
    public:
        uint32_t operator()(const std::string &str) const;
    };

//----------- BuildHashes ----------

    std::vector<Hasher> Build32bitsHashes();
    std::vector<Hasher> Build64bitsHashes();
}

#endif //THESIS_WORK_HASH_WRAPPERS_H
