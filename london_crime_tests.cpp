//
// Created by amrulla on 02.03.2021.
//

#include <fstream>
#include "london_crime_tests.h"

void RunLondonCrimeTests() {

    std::ofstream out("data/report.txt");
    LOG_DURATION_STREAM("All tests (1'000'000 limit)", out);
    const auto hashes = hash_wrappers::Build32bitsHashes();

    RunTest<std::hash<std::string>>("std::hash", out);
    RunTest<boost::hash<std::string>>("boost::hash", out);

    RunTest<hash_wrappers::CityHash32Wrapper>("CityHash32", out);
    RunTest<hash_wrappers::CityHash64Wrapper>("CityHash64", out);
    RunTest<hash_wrappers::CityHash64WithSeedWrapper>("CityHash64WithSeed", out);
    RunTest<hash_wrappers::CityHash64WithSeedsWrapper>("CityHash64WithSeeds", out);

    RunTest<hash_wrappers::FarmHash32Wrapper>("FarmHash32", out);
    RunTest<hash_wrappers::FarmHash32WithSeedWrapper>("FarmHash32WithSeed", out);
    RunTest<hash_wrappers::FarmHash64Wrapper>("FarmHash64", out);
    RunTest<hash_wrappers::FarmHash64WithSeedWrapper>("FarmHash64WithSeed", out);
    RunTest<hash_wrappers::FarmHash64WithSeedsWrapper>("FarmHash64WithSeeds", out);

    RunTest<hash_wrappers::xxHash32Wrapper>("xxHash32", out);
    RunTest<hash_wrappers::xxHash64Wrapper>("xxHash64", out);
    RunTest<hash_wrappers::XXH3_64bitsWrapper>("XXH3_64bits", out);
    RunTest<hash_wrappers::XXH3_64bits_withSeedWrapper>("XXH3_64bits_withSeed", out);

    RunTest<hash_wrappers::MurmurHash1Wrapper>("MurmurHash1", out);
    RunTest<hash_wrappers::MurmurHash1AlignedWrapper>("MurmurHash1Aligned", out);
    RunTest<hash_wrappers::MurmurHash2Wrapper>("MurmurHash2", out);
    RunTest<hash_wrappers::MurmurHash2Wrapper>("MurmurHash2", out);
    RunTest<hash_wrappers::MurmurHash64AWrapper>("MurmurHash64A", out);
    RunTest<hash_wrappers::MurmurHash64BWrapper>("MurmurHash64B", out);

    out << std::endl;
}
