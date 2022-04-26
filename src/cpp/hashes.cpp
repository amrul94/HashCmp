#include <gsl/gsl>

#include "hashes.h"

// HFL = Hash function library
namespace hfl {
    //----------- BuildHashes ----------


    template<hfl::UnsignedIntegral UintT>
    static void AddTemplateHashes(std::vector<Hash<UintT>>& hashes) {
        using namespace std::literals;
        using namespace hfl::wrappers;
        hashes.emplace_back("DJB2 Hash"s, std::make_unique<DJB2HashWrapper<UintT>>());         \
        hashes.emplace_back("SDBM Hash"s, std::make_unique<SDBMHashWrapper<UintT>>());                      \
        hashes.emplace_back("PJW Hash"s, std::make_unique<PJWHashWrapper<UintT>>());                        \
        hashes.emplace_back("One at a time hash"s, std::make_unique<OneTimeHashWrapper<UintT>>());    \
    }
        
    std::vector<Hash<uint16_t>> Build16bitsHashes() {
        using namespace std::literals;
        using namespace hfl::wrappers;

        const size_t num_hashes = 9;
        std::vector<Hash<uint16_t>> hashes;
        hashes.reserve(num_hashes);

        AddTemplateHashes(hashes);
        hashes.emplace_back("FNV-1a Hash"s, std::make_unique<FNV1aHash16Wrapper>());
        hashes.emplace_back("SpookyHash"s, std::make_unique<SpookyHash16Wrapper>());
        hashes.emplace_back("Fast-Hash"s, std::make_unique<FastHash16Wrapper>());
        hashes.emplace_back("PearsonHash"s, std::make_unique<PearsonHash16Wrapper>());
        hashes.emplace_back("BuzHash"s, std::make_unique<BuzHashWrapper<uint16_t>>());

        return hashes;
    }

    std::vector<Hash<uint24_t>> Build24bitsHashes() {
        using namespace std::literals;
        using namespace hfl::wrappers;

        const size_t num_hashes = 8;
        std::vector<Hash<uint24_t>> hashes;
        hashes.reserve(num_hashes);

        AddTemplateHashes(hashes);
        hashes.emplace_back("FNV-1a Hash"s, std::make_unique<FNV1aHash24Wrapper>());
        hashes.emplace_back("SpookyHash"s, std::make_unique<SpookyHash24Wrapper>());
        hashes.emplace_back("Fast-Hash"s, std::make_unique<FastHash24Wrapper>());
        hashes.emplace_back("PearsonHash"s, std::make_unique<PearsonHash24Wrapper>());

        return hashes;
    }

    std::vector<Hash<uint32_t>> Build32bitsHashes() {
        using namespace std::literals;
        using namespace hfl::wrappers;

        const size_t num_hashes = 23;
        std::vector<Hash<uint32_t>> hashes;
        hashes.reserve(num_hashes);

        AddTemplateHashes(hashes);
        hashes.emplace_back("FNV-1a Hash"s, std::make_unique<FNV1aHash32Wrapper>());
        hashes.emplace_back("SpookyHash"s, std::make_unique<SpookyHash32Wrapper>());
        hashes.emplace_back("Fast-Hash"s, std::make_unique<FastHash32Wrapper>());
        hashes.emplace_back("PearsonHash"s, std::make_unique<PearsonHash32Wrapper>());
        hashes.emplace_back("BuzHash"s, std::make_unique<BuzHashWrapper<uint32_t>>());

        hashes.emplace_back("SuperFastHash"s, std::make_unique<SuperFastHashWrapper>());
        hashes.emplace_back("MurmurHash1"s, std::make_unique<MurmurHash1Wrapper>());
        hashes.emplace_back("MurmurHash2"s, std::make_unique<MurmurHash2Wrapper>());
        hashes.emplace_back("MurmurHash2A"s, std::make_unique<MurmurHash2AWrapper>());
        hashes.emplace_back("MurmurHash3"s, std::make_unique<MurmurHash3Wrapper>());
        hashes.emplace_back("CityHash32"s, std::make_unique<CityHash32Wrapper>());
        hashes.emplace_back("FarmHash32"s, std::make_unique<FarmHash32Wrapper>());
        hashes.emplace_back("FarmHash32 with seed"s, std::make_unique<FarmHash32WithSeedWrapper>());
        hashes.emplace_back("T1HA0 32le hash"s, std::make_unique<T1HA0_32leWrapper>());
        hashes.emplace_back("T1HA0 32be hash"s, std::make_unique<T1HA0_32beWrapper>());
        hashes.emplace_back("xxHash32"s, std::make_unique<xxHash32Wrapper>());

        hashes.emplace_back("wyhash32"s, std::make_unique<wyHash32Wrapper>());
        hashes.emplace_back("NMHASH32"s, std::make_unique<nmHash32Wrapper>());
        hashes.emplace_back("NMHASH32x"s, std::make_unique<nmHash32XWrapper>());
        hashes.emplace_back("HalfSipHash"s, std::make_unique<HalfSipHashWrapper>());

        return hashes;
    }

    std::vector<Hash<uint48_t>> Build48bitsHashes() {
        using namespace std::literals;
        using namespace hfl::wrappers;

        const size_t num_hashes = 7;
        std::vector<Hash<uint48_t>> hashes;
        hashes.reserve(num_hashes);

        AddTemplateHashes(hashes);
        hashes.emplace_back("FNV-1a Hash"s, std::make_unique<FNV1aHash48Wrapper>());
        hashes.emplace_back("SpookyHash"s, std::make_unique<SpookyHash48Wrapper>());
        hashes.emplace_back("Fast-Hash"s, std::make_unique<FastHash48Wrapper>());
        hashes.emplace_back("PearsonHash"s, std::make_unique<PearsonHash48Wrapper>());

        return hashes;
    }

    std::vector<Hash<uint64_t>> Build64bitsHashes() {
        using namespace std::literals;
        using namespace hfl::wrappers;

        const size_t num_hashes = 34;
        std::vector<Hash<uint64_t>> hashes;
        hashes.reserve(num_hashes);

        AddTemplateHashes(hashes);
        hashes.emplace_back("FNV-1a Hash"s, std::make_unique<FNV1aHash64Wrapper>());
        hashes.emplace_back("SpookyHash"s, std::make_unique<SpookyHash64Wrapper>());
        hashes.emplace_back("Fast-Hash"s, std::make_unique<FastHash64Wrapper>());
        hashes.emplace_back("PearsonHash"s, std::make_unique<PearsonHash64Wrapper>());
        hashes.emplace_back("BuzHash"s, std::make_unique<BuzHashWrapper<uint64_t>>());

        hashes.emplace_back("MurmurHash2 64 bits"s, std::make_unique<MurmurHash64AWrapper>());
        hashes.emplace_back("CityHash64"s, std::make_unique<CityHash64Wrapper>());
        hashes.emplace_back("CityHash64WithSeed"s, std::make_unique<CityHash64WithSeedWrapper>());
        hashes.emplace_back("CityHash64WithSeeds"s, std::make_unique<CityHash64WithSeedsWrapper>());
        hashes.emplace_back("FarmHash64"s, std::make_unique<FarmHash64Wrapper>());
        hashes.emplace_back("FarmHash64WithSeed"s, std::make_unique<FarmHash64WithSeedWrapper>());
        hashes.emplace_back("FarmHash64WithSeeds"s, std::make_unique<FarmHash64WithSeedsWrapper>());
        hashes.emplace_back("MetroHash64"s, std::make_unique<MetroHash64_Wrapper>());
        hashes.emplace_back("T1HA0 AVX2 hash", std::make_unique<T1HA0_AVX2_Wrapper>());
        hashes.emplace_back("T1HA1 le hash", std::make_unique<T1HA1LeWrapper>());
        hashes.emplace_back("T1HA1 be hash", std::make_unique<T1HA1BeWrapper>());
        hashes.emplace_back("T1HA2 atonce hash", std::make_unique<T1HA2AtonceWrapper>());
        hashes.emplace_back("xxHash64", std::make_unique<xxHash64Wrapper>());
        hashes.emplace_back("XXH3 64 bits", std::make_unique<XXH3_64BitsWrapper>());
        hashes.emplace_back("XXH3 64 bits with seed", std::make_unique<XXH3_64bits_withSeedWrapper>());

        hashes.emplace_back("wyhash64", std::make_unique<wyHash64Wrapper>());
        hashes.emplace_back("PengyHash", std::make_unique<PengyHash64Wrapper>());
        hashes.emplace_back("MX3 hash", std::make_unique<MX3HashWrapper>());
        hashes.emplace_back("SipHash", std::make_unique<SipHashWrapper>());
        hashes.emplace_back("SipHash13", std::make_unique<SipHash13Wrapper>());
        hashes.emplace_back("SipHash (Google Impl)", std::make_unique<SipHashAVX2Wrapper>());
        hashes.emplace_back("SipHash13 (Google Impl)", std::make_unique<SipHash13AVX2Wrapper>());
        hashes.emplace_back("HighwayHash", std::make_unique<HighwayHashWrapper>());
        hashes.emplace_back("MUM hash", std::make_unique<MumHashWrapper>());
        hashes.emplace_back("mir hash", std::make_unique<MirHashWrapper>());

        return hashes;
    }
}