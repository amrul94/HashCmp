//
// Created by amrulla on 02.03.2021.
//

#ifndef THESIS_WORK_HASH_WRAPPERS_H
#define THESIS_WORK_HASH_WRAPPERS_H

#include "hashes/rolling_hash/cyclichash.h"
#include "hashes/rolling_hash/rabinkarphash.h"

#include <functional>
#include <string>


namespace hashes {

    constexpr int WORD_SIZE = 1000;

    class [[maybe_unused]] BaseHash32Wrapper {
    public:
        virtual uint32_t operator()(const std::string &str) const = 0;
    };

    class [[maybe_unused]] BaseHash64Wrapper {
    public:
        virtual uint64_t operator()(const std::string &str) const = 0;
    };

//----- Bernstein's hash djb2 ------

    class [[maybe_unused]] DJB2Hash32Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] DJB2Hash64Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//----------- CityHashes ----------

    class [[maybe_unused]] CityHash32Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] CityHash64Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] CityHash64WithSeedWrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] CityHash64WithSeedsWrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//----------- FarmHashes ----------

    class [[maybe_unused]] FarmHash32Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] FarmHash32WithSeedWrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] FarmHash64Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] FarmHash64WithSeedWrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };


    class [[maybe_unused]] FarmHash64WithSeedsWrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//------------ FastHash ------------

    class [[maybe_unused]] FastHash32Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] FastHash64Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//---------- FNV-1a hash -----------

    class [[maybe_unused]] FNV1aHash32Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] FNV1aHash64Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };


//--------- Jenkins hash -----------

    class [[maybe_unused]] OneTimeHashWrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] Lookup3LittleWrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] Lookup3BigWrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };


//------------ MetroHash -----------

    class [[maybe_unused]] MetroHash64_1_Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] MetroHash64_2_Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//---------- MurmurHashes ---------

    class [[maybe_unused]] MurmurHash1Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] MurmurHash1AlignedWrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] MurmurHash2Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] MurmurHash64AWrapper: public BaseHash64Wrapper  {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] MurmurHash64BWrapper: public BaseHash64Wrapper  {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//--- Paul Hsieh's SuperFastHash ---

    class [[maybe_unused]] SuperFastHashWrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

//---------- PearsonHashes ---------

    class [[maybe_unused]] PearsonHash32Wrapper : public BaseHash32Wrapper {
    public:
        PearsonHash32Wrapper() noexcept;
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] PearsonHash64Wrapper : public BaseHash64Wrapper {
    public:
        PearsonHash64Wrapper() noexcept;
        uint64_t operator()(const std::string &str) const override;
    };

//------------ PJW Hash ------------

    class [[maybe_unused]] PJWHash32Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] PJWHash64Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//----- Rolling Hash (BuzHash) -----

    class [[maybe_unused]] BuzHash32Wrapper : public BaseHash32Wrapper {
    public:
        BuzHash32Wrapper() noexcept = default;
        uint32_t operator()(const std::string &str) const override;

    private:
        mutable CyclicHash<uint32_t, char> hash_function_{WORD_SIZE, 32};
    };

    class [[maybe_unused]] BuzHash64Wrapper : public BaseHash64Wrapper {
    public:
        BuzHash64Wrapper() noexcept = default;
        uint64_t operator()(const std::string &str) const override;

    private:
        mutable CyclicHash<uint64_t, char> hash_function_{WORD_SIZE, 64};
    };

//-------------- SDBM --------------

    class [[maybe_unused]] SDBMHash32Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] SDBMHash64Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//---------- Spooky hash -----------

    class [[maybe_unused]] SpookyHash32Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] SpookyHash64Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//-------------- T1HA --------------

    class [[maybe_unused]] T1HA0_32leWrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] T1HA0_32beWrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] T1HA1LeWrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] T1HA1BeWrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };


    class [[maybe_unused]] T1HA2AtonceWrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

//------------ xxHashes -----------

    class [[maybe_unused]] xxHash32Wrapper : public BaseHash32Wrapper {
    public:
        uint32_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] xxHash64Wrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] XXH3_64BitsWrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };

    class [[maybe_unused]] XXH3_64BitsWithSeedWrapper : public BaseHash64Wrapper {
    public:
        uint64_t operator()(const std::string &str) const override;
    };
}

#endif //THESIS_WORK_HASH_WRAPPERS_H
