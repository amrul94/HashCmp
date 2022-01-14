#include "generated_tests.h"
#include "distribution_tests.h"
#include "english_tests.h"
#include "log_duration.h"
#include "speed_tests.h"

#include <boost/format.hpp>
#include <boost/container/static_vector.hpp>

#include <pcg_random.hpp>
#include <random>
#include <algorithm>
#include <execution>
#include <set>

#include "hashes.h"

#include <pengyhash/pengyhash.h>

#include "siphash/siphash.h"

#include <highwayhash/sip_hash.h>

void LocalSpeedTest(tests::ReportsRoot& report_root) {
    pcg64 rng;
    const auto words = GenerateRandomDataBlocks(rng, 1'000'000, FOUR_KILOBYTES);

    hfl::Hash32Struct city_hash_32s {"CityHash32", hfl::city_hash_32, 32};
    tests::HashSpeedTest(city_hash_32s, words, report_root);

    const auto hashes32 = hfl::Build32bitsHashes();
    tests::HashSpeedTest(hashes32.back(), words, report_root);

    const auto hashes64 = hfl::Build64bitsHashes();
    tests::HashSpeedTest(hashes64[hashes64.size() - 4], words, report_root);
    tests::HashSpeedTest(hashes64[hashes64.size() - 3], words, report_root);
    tests::HashSpeedTest(hashes64[hashes64.size() - 2], words, report_root);
    tests::HashSpeedTest(hashes64.back(), words, report_root);
}

void TempTests(tests::ReportsRoot& report_root) {
    std::string str = "test_string";

    using namespace highwayhash;
    const highwayhash::HH_U64 key2[2] = {1234, 5678};
    char in[8] = {1};
    std::cout << SipHash(key2, in, 8) << std::endl;
    LocalSpeedTest(report_root);
}

void RunTests(const std::vector<int>& test_numbers, tests::ReportsRoot& reports_root) {
    LOG_DURATION_STREAM("Full time", reports_root.logger);
    for (int test_number : test_numbers) {
        switch (test_number) {
            case 1:
                reports_root.logger << "start DistributionTest\n\n";
                tests::RunDistributionTests(reports_root);
                reports_root.logger << "end DistributionTest\n\n\n";
                break;
            case 2:
                reports_root.logger << "start generated blocks test (length = 16)\n\n";
                tests::RunTestWithGeneratedBlocks(16, reports_root);
                reports_root.logger << "end generated blocks test (length = 16)\n\n\n";
                break;
            case 3:
                reports_root.logger << "start generated blocks test (length = 4096)\n\n";
                tests::RunTestWithGeneratedBlocks(FOUR_KILOBYTES, reports_root);
                reports_root.logger << "end generated blocks test (length = 4096)\n\n\n";
                break;
            case 4:
                reports_root.logger << "start english words test\n\n";
                tests::RunTestWithEnglishWords(reports_root);
                reports_root.logger << "end english words test\n\n\n";
                break;
            case 5:
                reports_root.logger << "start speed test\n\n";
                tests::RunSpeedTests(200'000, FOUR_KILOBYTES, reports_root);
                reports_root.logger << "end speed test\n\n\n";
                break;
            default:
                TempTests(reports_root);
                break;
        }
    }
}

tests::ReportsRoot CreateReportsRoot() {
    const auto current_path = std::filesystem::current_path();
    const std::filesystem::path reports_dir{"reports"};
    const auto reports_path = current_path / reports_dir / CurrentTime() / "cpp";
    std::filesystem::create_directories(reports_path);
    return tests::ReportsRoot{reports_path};
}

int main() {
    const std::vector test_numbers{50};
    tests::ReportsRoot reports_root = CreateReportsRoot();

    RunTests(test_numbers, reports_root);
}

