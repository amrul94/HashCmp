//
// Created by amrulla on 02.03.2021.
//

#include "london_crime_tests.h"

void runLondonCrimeTests() {

    //RunTest<BadHash>("bad_hash");
    RunTest<std::hash<std::string>>("std::hash");
    RunTest<boost::hash<std::string>>("boost::hash");

    RunTest<CityHash32Wrapper>("CityHash32");
    RunTest<CityHash64Wrapper>("CityHash64");
    RunTest<CityHash64WithSeedWrapper>("CityHash64WithSeed");
    RunTest<CityHash64WithSeedsWrapper>("CityHash64WithSeeds");
    RunTest<CityHash128Wrapper>("CityHash128");
    RunTest<CityHash128WithSeedWrapper>("CityHash128WithSeed");

    RunTest<FarmHash32Wrapper>("FarmHash32");
    RunTest<FarmHash32WithSeedWrapper>("FarmHash32WithSeed");
    RunTest<FarmHash64Wrapper>("FarmHash64");
    RunTest<FarmHash64WithSeedWrapper>("FarmHash64WithSeed");
    RunTest<FarmHash64WithSeedsWrapper>("FarmHash64WithSeeds");
    RunTest<FarmHash128Wrapper>("FarmHash128");
    RunTest<FarmHash128WithSeedWrapper>("FarmHash128WithSeed");

    RunTest<xxHash32Wrapper>("xxHash32");
    RunTest<xxHash64Wrapper>("xxHash64");
    RunTest<xxHash128Wrapper>("xxHash128");
    RunTest<XXH3_64bitsWrapper>("XXH3_64bits");
    RunTest<XXH3_64bits_withSeedWrapper>("XXH3_64bits_withSeed");
    //RunTest<xxH3WithSecretWrapper>("XXH3_64bits_withSecret");
    RunTest<XXH3_128bitsWrapper>("XXH3_128bits");
    RunTest<XXH3_128bits_withSeedWrapper>("XXH3_128bits_withSeed");

    RunTest<MurmurHash1Wrapper>("MurmurHash1");
    RunTest<MurmurHash1AlignedWrapper>("MurmurHash1Aligned");
    RunTest<MurmurHash2Wrapper>("MurmurHash2");
    RunTest<MurmurHash2Wrapper>("MurmurHash2");
    RunTest<MurmurHash64AWrapper>("MurmurHash64A");
    RunTest<MurmurHash64BWrapper>("MurmurHash64B");
    RunTest<MurmurHash3_x64_128Wrapper>("MurmurHash3_x64_128");

}
