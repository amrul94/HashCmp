//
// Created by amrulla on 02.03.2021.
//

#include <fstream>
#include "london_crime_tests.h"

void runLondonCrimeTests() {
    std::ofstream out("report.txt");
    LOG_DURATION_STREAM("All tests", out);

    //RunTest<BadHash>("bad_hash");
    RunTest<std::hash<std::string>>("std::hash", out);
    RunTest<boost::hash<std::string>>("boost::hash", out);

    RunTest<CityHash32Wrapper>("CityHash32", out);
    RunTest<CityHash64Wrapper>("CityHash64", out);
    RunTest<CityHash64WithSeedWrapper>("CityHash64WithSeed", out);
    RunTest<CityHash64WithSeedsWrapper>("CityHash64WithSeeds", out);
    RunTest<CityHash128Wrapper>("CityHash128", out);
    RunTest<CityHash128WithSeedWrapper>("CityHash128WithSeed", out);

    RunTest<FarmHash32Wrapper>("FarmHash32", out);
    RunTest<FarmHash32WithSeedWrapper>("FarmHash32WithSeed", out);
    RunTest<FarmHash64Wrapper>("FarmHash64", out);
    RunTest<FarmHash64WithSeedWrapper>("FarmHash64WithSeed", out);
    RunTest<FarmHash64WithSeedsWrapper>("FarmHash64WithSeeds", out);
    RunTest<FarmHash128Wrapper>("FarmHash128", out);
    RunTest<FarmHash128WithSeedWrapper>("FarmHash128WithSeed", out);

    RunTest<xxHash32Wrapper>("xxHash32", out);
    RunTest<xxHash64Wrapper>("xxHash64", out);
    RunTest<xxHash128Wrapper>("xxHash128", out);
    RunTest<XXH3_64bitsWrapper>("XXH3_64bits", out);
    RunTest<XXH3_64bits_withSeedWrapper>("XXH3_64bits_withSeed", out);
    //RunTest<xxH3WithSecretWrapper>("XXH3_64bits_withSecret");
    RunTest<XXH3_128bitsWrapper>("XXH3_128bits", out);
    RunTest<XXH3_128bits_withSeedWrapper>("XXH3_128bits_withSeed", out);

    RunTest<MurmurHash1Wrapper>("MurmurHash1", out);
    RunTest<MurmurHash1AlignedWrapper>("MurmurHash1Aligned", out);
    RunTest<MurmurHash2Wrapper>("MurmurHash2", out);
    RunTest<MurmurHash2Wrapper>("MurmurHash2", out);
    RunTest<MurmurHash64AWrapper>("MurmurHash64A", out);
    RunTest<MurmurHash64BWrapper>("MurmurHash64B", out);
    RunTest<MurmurHash3_x64_128Wrapper>("MurmurHash3_x64_128", out);

    out << std::endl;
}
