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
    RunTest<hashes::CityHash32Wrapper>("CityHash32", out);
    RunTest<hashes::CityHash64Wrapper>("CityHash64", out);
    RunTest<hashes::CityHash64WithSeedWrapper>("CityHash64WithSeed", out);
    RunTest<hashes::CityHash64WithSeedsWrapper>("CityHash64WithSeeds", out);

    RunTest<hashes::FarmHash32Wrapper>("FarmHash32", out);
    RunTest<hashes::FarmHash32WithSeedWrapper>("FarmHash32WithSeed", out);
    RunTest<hashes::FarmHash64Wrapper>("FarmHash64", out);
    RunTest<hashes::FarmHash64WithSeedWrapper>("FarmHash64WithSeed", out);
    RunTest<hashes::FarmHash64WithSeedsWrapper>("FarmHash64WithSeeds", out);

    RunTest<hashes::xxHash32Wrapper>("xxHash32", out);
    RunTest<hashes::xxHash64Wrapper>("xxHash64", out);
    RunTest<hashes::XXH3_64BitsWrapper>("XXH3_64bits", out);
    RunTest<hashes::XXH3_64BitsWithSeedWrapper>("XXH3_64bits_withSeed", out);

    RunTest<hashes::MurmurHash1Wrapper>("MurmurHash1", out);
    RunTest<hashes::MurmurHash1AlignedWrapper>("MurmurHash1Aligned", out);
    RunTest<hashes::MurmurHash2Wrapper>("MurmurHash2", out);
    RunTest<hashes::MurmurHash2Wrapper>("MurmurHash2", out);
    RunTest<hashes::MurmurHash64AWrapper>("MurmurHash64A", out);
    RunTest<hashes::MurmurHash64BWrapper>("MurmurHash64B", out);
*/
    out << std::endl;
}
