//
// Created by amrulla on 02.03.2021.
//

#include <fstream>
#include "london_crime_tests.h"

void RunLondonCrimeTests() {

    std::ofstream out("data/report.txt");
    LOG_DURATION_STREAM("All tests (1'000'000 limit)", out);
    const auto hashes = hashes::Build32bitsHashes();

    RunTest<std::hash<std::string>>("std::hash", out);
    RunTest<boost::hash<std::string>>("boost::hash", out);
/*
    RunTest<HashLib::CityHash32Wrapper>("CityHash32", out);
    RunTest<HashLib::CityHash64Wrapper>("CityHash64", out);
    RunTest<HashLib::CityHash64WithSeedWrapper>("CityHash64WithSeed", out);
    RunTest<HashLib::CityHash64WithSeedsWrapper>("CityHash64WithSeeds", out);

    RunTest<HashLib::FarmHash32Wrapper>("FarmHash32", out);
    RunTest<HashLib::FarmHash32WithSeedWrapper>("FarmHash32WithSeed", out);
    RunTest<HashLib::FarmHash64Wrapper>("FarmHash64", out);
    RunTest<HashLib::FarmHash64WithSeedWrapper>("FarmHash64WithSeed", out);
    RunTest<HashLib::FarmHash64WithSeedsWrapper>("FarmHash64WithSeeds", out);

    RunTest<HashLib::xxHash32Wrapper>("xxHash32", out);
    RunTest<HashLib::xxHash64Wrapper>("xxHash64", out);
    RunTest<HashLib::XXH3_64BitsWrapper>("XXH3_64bits", out);
    RunTest<HashLib::XXH3_64BitsWithSeedWrapper>("XXH3_64bits_withSeed", out);

    RunTest<HashLib::MurmurHash1Wrapper>("MurmurHash1", out);
    RunTest<HashLib::MurmurHash1AlignedWrapper>("MurmurHash1Aligned", out);
    RunTest<HashLib::MurmurHash2Wrapper>("MurmurHash2", out);
    RunTest<HashLib::MurmurHash2Wrapper>("MurmurHash2", out);
    RunTest<HashLib::MurmurHash64AWrapper>("MurmurHash64A", out);
    RunTest<HashLib::MurmurHash64BWrapper>("MurmurHash64B", out);
*/
    out << std::endl;
}
